import socket
import threading
from collections import deque

# ── Configuration ──────────────────────────────────
HOST = ''
PORT = 5000
THRESHOLD_NEAR_CM = 30.0
THRESHOLD_MEDIUM_CM = 100.0
THRESHOLD_FAR_CM = 150.0
MAX_SEQ_CACHE = 500

lock = threading.Lock()
seen_seq = deque(maxlen=MAX_SEQ_CACHE)
actuator_conn = None

# ── Control Algorithm ──────────────────────────────
def control_algorithm(distance: float) -> str:
    if distance < THRESHOLD_NEAR_CM:
        return "CMD:RED\n"
    elif distance < THRESHOLD_MEDIUM_CM:
        return "CMD:YELLOW\n"
    elif distance < THRESHOLD_FAR_CM:
        return "CMD:GREEN\n"
    else:
        return "CMD:OFF\n"

# ── Message Processing ─────────────────────────────
def process_message(msg: str, conn: socket.socket) -> None:
    global actuator_conn

    # Basic message validation
    try:
        parts = dict(p.split(":", 1) for p in msg.strip().split("|") if ":" in p)
    except ValueError:
        print(f"[WARN] Malformed message ignored: {msg!r}")
        return

    if not parts:
        return

    # Actuator registration
    if parts.get("TYPE") == "ACTUATOR":
        with lock:
            actuator_conn = conn
        conn.sendall("ACK:REGISTERED\n".encode())
        print("[INFO] Actuator registered")
        return

    # Sensor message
    seq = parts.get("SEQ")
    dist_raw = parts.get("DIST")

    if seq is None or dist_raw is None:
        print(f"[WARN] Missing fields in message: {parts}")
        return

    # Duplicate prevention with bounded cache
    with lock:
        if seq in seen_seq:
            conn.sendall(f"ACK:{seq}\n".encode())
            return
        seen_seq.append(seq)
        act = actuator_conn  # local copy inside lock

    try:
        dist = float(dist_raw)
    except ValueError:
        print(f"[WARN] Invalid distance: {dist_raw!r}")
        return

    cmd = control_algorithm(dist)
    print(f"[INFO] SEQ={seq} DIST={dist:.1f}cm → {cmd.strip()}")

    conn.sendall(f"ACK:{seq}\n".encode())

    if act:
        try:
            act.sendall(cmd.encode())
        except OSError as e:
            print(f"[WARN] Actuator disconnected: {e}")
            with lock:
                actuator_conn = None

# ── Client Thread ──────────────────────────────────
def handle_client(conn: socket.socket, addr) -> None:
    global actuator_conn
    print(f"[INFO] Connected: {addr}")
    buffer = ""

    try:
        with conn:
            # Read first message to identify sensor or actuator
            data = conn.recv(1024).decode("utf-8", errors="replace")
            if not data:
                return

            buffer += data
            first_line = ""
            if "\n" in buffer:
                first_line, buffer = buffer.split("\n", 1)

            # Clear seen_seq only if sensor reconnects
            if "TYPE:SENSOR" in first_line or "SEQ" in first_line:
                with lock:
                    seen_seq.clear()
                print("[INFO] Sensor reconnected - seen_seq cleared")

            if first_line.strip():
                process_message(first_line, conn)

            # Continue processing remaining messages
            while True:
                data = conn.recv(1024).decode("utf-8", errors="replace")
                if not data:
                    break
                buffer += data
                while "\n" in buffer:
                    line, buffer = buffer.split("\n", 1)
                    if line.strip():
                        process_message(line, conn)

    except OSError as e:
        print(f"[INFO] Client {addr} disconnected: {e}")
    finally:
        print(f"[INFO] Closed: {addr}")

# ── Server Startup ─────────────────────────────────
if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()
        print(f"[INFO] Server listening on port {PORT}")

        while True:
            conn, addr = s.accept()
            threading.Thread(
                target=handle_client,
                args=(conn, addr),
                daemon=True
            ).start()
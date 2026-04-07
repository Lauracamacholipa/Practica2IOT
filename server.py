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

# ── Shared State ───────────────────────────────────
lock = threading.Lock()
seen_seq = deque(maxlen=MAX_SEQ_CACHE)  # Prevents duplicate processing
actuator_conn = None  # Only one actuator supported


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


# ── Message Parsing ────────────────────────────────
def parse_message(msg: str) -> dict:
    try:
        return dict(
            part.split(":", 1)
            for part in msg.strip().split("|")
            if ":" in part
        )
    except ValueError:
        return {}


# ── Actuator Handling ──────────────────────────────
def register_actuator(conn: socket.socket) -> None:
    global actuator_conn
    with lock:
        actuator_conn = conn

    conn.sendall("ACK:REGISTERED\n".encode())
    print("[INFO] Actuator registered")


def send_command_to_actuator(cmd: str) -> None:
    global actuator_conn

    with lock:
        act = actuator_conn

    if not act:
        return

    try:
        act.sendall(cmd.encode())
    except OSError as e:
        print(f"[WARN] Actuator disconnected: {e}")
        with lock:
            actuator_conn = None


# ── Sensor Handling ────────────────────────────────
def is_duplicate(seq: str) -> bool:
    with lock:
        if seq in seen_seq:
            return True
        seen_seq.append(seq)
        return False


def handle_sensor_message(parts: dict, conn: socket.socket) -> None:
    seq = parts.get("SEQ")
    dist_raw = parts.get("DIST")

    if seq is None or dist_raw is None:
        print(f"[WARN] Missing fields in message: {parts}")
        return

    if is_duplicate(seq):
        conn.sendall(f"ACK:{seq}\n".encode())
        return

    try:
        distance = float(dist_raw)
    except ValueError:
        print(f"[WARN] Invalid distance: {dist_raw!r}")
        return

    cmd = control_algorithm(distance)

    print(f"[INFO] SEQ={seq} DIST={distance:.1f}cm → {cmd.strip()}")

    conn.sendall(f"ACK:{seq}\n".encode())
    send_command_to_actuator(cmd)


# ── Message Dispatcher ─────────────────────────────
def process_message(msg: str, conn: socket.socket) -> None:
    parts = parse_message(msg)

    if not parts:
        print(f"[WARN] Malformed message ignored: {msg!r}")
        return

    if parts.get("TYPE") == "ACTUATOR":
        register_actuator(conn)
        return

    handle_sensor_message(parts, conn)


# ── Connection Handling ────────────────────────────
def read_line_from_buffer(buffer: str):
    if "\n" in buffer:
        return buffer.split("\n", 1)
    return None, buffer


def handle_client(conn: socket.socket, addr) -> None:
    print(f"[INFO] Connected: {addr}")
    buffer = ""

    try:
        with conn:
            data = conn.recv(1024).decode("utf-8", errors="replace")
            if not data:
                return

            buffer += data

            line, buffer = read_line_from_buffer(buffer)

            # Detect sensor reconnection
            if line and ("TYPE:SENSOR" in line or "SEQ" in line):
                with lock:
                    seen_seq.clear()
                print("[INFO] Sensor reconnected - seen_seq cleared")

            if line and line.strip():
                process_message(line, conn)

            while True:
                data = conn.recv(1024).decode("utf-8", errors="replace")
                if not data:
                    break

                buffer += data

                while True:
                    line, buffer = read_line_from_buffer(buffer)
                    if line is None:
                        break
                    if line.strip():
                        process_message(line, conn)

    except OSError as e:
        print(f"[INFO] Client {addr} disconnected: {e}")
    finally:
        print(f"[INFO] Closed: {addr}")


# ── Server Startup ─────────────────────────────────
def start_server():
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


if __name__ == "__main__":
    start_server()
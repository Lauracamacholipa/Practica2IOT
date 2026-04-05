import socket
import threading
from collections import deque

# ── Configuración ──────────────────────────────────
HOST = ''
PORT = 5000
THRESHOLD_NEAR_CM =  30.0
THRESHOLD_MEDIUM_CM = 100.0
THRESHOLD_FAR_CM = 150.0
MAX_SEQ_CACHE = 500     # límite para evitar crecimiento indefinido

lock = threading.Lock()          # protege actuador_conn y seen_seq
seen_seq = deque(maxlen=MAX_SEQ_CACHE)
actuador_conn = None

# ── Algoritmo de control ───────────────────────────
def algoritmo_control(distancia: float) -> str:
    if distancia < THRESHOLD_NEAR_CM:
        return "CMD:RED\n"
    elif distancia < THRESHOLD_MEDIUM_CM:
        return "CMD:YELLOW\n"
    elif distancia < THRESHOLD_FAR_CM:
        return "CMD:GREEN\n"
    else:
        return "CMD:OFF\n" 

# ── Procesamiento de cada mensaje recibido ─────────
def procesar_mensaje(msg: str, conn: socket.socket) -> None:
    global actuador_conn

    # Validación básica del mensaje
    try:
        partes = dict(p.split(":", 1) for p in msg.strip().split("|") if ":" in p)
    except ValueError:
        print(f"[WARN] Mensaje malformado ignorado: {msg!r}")
        return

    if not partes:
        return

    # Registro del actuador
    if partes.get("TYPE") == "ACTUATOR":
        with lock:
            actuador_conn = conn
        conn.sendall("ACK:REGISTRO\n".encode())   # confirmación al actuador
        print("[INFO] Actuador registrado")
        return

    # Mensaje del sensor
    seq = partes.get("SEQ")
    dist_raw = partes.get("DIST")

    if seq is None or dist_raw is None:
        print(f"[WARN] Campos faltantes en mensaje: {partes}")
        return

    # Anti-duplicados con tamaño acotado
    with lock:
        if seq in seen_seq:
            conn.sendall(f"ACK:{seq}\n".encode())
            return
        seen_seq.append(seq)
        act = actuador_conn          # captura local dentro del lock

    try:
        dist = float(dist_raw)
    except ValueError:
        print(f"[WARN] Distancia inválida: {dist_raw!r}")
        return

    cmd = algoritmo_control(dist)
    print(f"[INFO] SEQ={seq} DIST={dist:.1f}cm → {cmd.strip()}")

    conn.sendall(f"ACK:{seq}\n".encode())

    if act:
        try:
            act.sendall(cmd.encode())
        except OSError as e:
            print(f"[WARN] Actuador desconectado: {e}")
            with lock:
                actuador_conn = None

# ── Hilo por cliente ───────────────────────────────
def manejar_cliente(conn: socket.socket, addr) -> None:
    print(f"[INFO] Conectado: {addr}")
    buffer = ""
    with lock:
        seen_seq.clear() 
    try:
        with conn:
            while True:
                datos = conn.recv(1024).decode("utf-8", errors="replace")
                if not datos:
                    break
                buffer += datos
                while "\n" in buffer:
                    linea, buffer = buffer.split("\n", 1)
                    if linea.strip():
                        procesar_mensaje(linea, conn)
    except OSError as e:
        print(f"[INFO] Cliente {addr} desconectado: {e}")
    finally:
        print(f"[INFO] Cerrado: {addr}")

# ── Inicio del servidor ────────────────────────────
if __name__ == "__main__":
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()
        print(f"[INFO] Servidor escuchando en puerto {PORT}")
        while True:
            conn, addr = s.accept()
            threading.Thread(
                target=manejar_cliente,
                args=(conn, addr),
                daemon=True
            ).start()
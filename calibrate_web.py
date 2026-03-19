from flask import Flask, render_template, request, jsonify
import serial
import threading
import time
import logging
import os

app = Flask(__name__)
# Suppress routine network logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

serial_port = None
is_connected = False
latest_telemetry = {
    "error": 0.0,
    "line_lost": False,
    "kp": 15.0,
    "ki": 0.0,
    "kd": 5.0,
    "base_speed": 100,
    "running": False
}

def rx_loop():
    global is_connected, latest_telemetry, serial_port
    while True:
        if is_connected and serial_port and serial_port.in_waiting:
            try:
                line = serial_port.readline().decode('utf-8').strip()
                if line.startswith("T:"):
                    # Format -> T:error,baseSpeed,running,Kp,Ki,Kd
                    parts = line[2:].split(',')
                    if len(parts) == 6:
                        err = float(parts[0])
                        latest_telemetry["base_speed"] = int(parts[1])
                        latest_telemetry["running"] = bool(int(parts[2]))
                        latest_telemetry["kp"] = float(parts[3])
                        latest_telemetry["ki"] = float(parts[4])
                        latest_telemetry["kd"] = float(parts[5])
                        
                        if err >= 999.0:
                            latest_telemetry["line_lost"] = True
                        else:
                            latest_telemetry["line_lost"] = False
                            latest_telemetry["error"] = err
                            
            except Exception as e:
                print(f"Rx parsing error: {e}")
        time.sleep(0.01)

# Start background UART thread
threading.Thread(target=rx_loop, daemon=True).start()

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/api/connect", methods=["POST"])
def connect():
    global serial_port, is_connected
    data = request.json
    port = data.get("port", "/dev/serial0")
    try:
        if not is_connected:
            serial_port = serial.Serial(port, 115200, timeout=0.1)
            is_connected = True
        return jsonify({"status": "success"})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 400

@app.route("/api/disconnect", methods=["POST"])
def disconnect():
    global serial_port, is_connected
    if is_connected and serial_port:
        serial_port.close()
        is_connected = False
    return jsonify({"status": "success"})

@app.route("/api/status")
def status():
    return jsonify({
        "connected": is_connected,
        "telemetry": latest_telemetry
    })

@app.route("/api/command", methods=["POST"])
def command():
    global serial_port, is_connected
    cmd = request.json.get("cmd")
    if is_connected and serial_port and cmd in ["START", "STOP"]:
        serial_port.write(f"{cmd}\n".encode())
        return jsonify({"status": "success"})
    return jsonify({"status": "error", "message": "Not connected or invalid command"}), 400

@app.route("/api/pid", methods=["POST"])
def update_pid():
    global serial_port, is_connected
    data = request.json
    if is_connected and serial_port:
        p = data.get("p", 15.0)
        i = data.get("i", 0.0)
        d = data.get("d", 5.0)
        cmd = f"P:{p:.2f},I:{i:.2f},D:{d:.2f}\n"
        serial_port.write(cmd.encode())
        return jsonify({"status": "success"})
    return jsonify({"status": "error", "message": "Not connected"}), 400

if __name__ == "__main__":
    # Host on 0.0.0.0 so laptops on the same network can access it
    app.run(host="0.0.0.0", port=5000)

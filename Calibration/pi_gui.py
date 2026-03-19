import tkinter as tk
from tkinter import ttk, messagebox
import serial
import threading
import time

class PIDCalibratorGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Raspberry Pi 4B - Pico PID Calibrator")
        self.root.geometry("600x450")
        self.root.configure(bg="#2d2d2d")
        
        self.serial_port = None
        self.is_connected = False
        self.stop_thread = False
        
        self.setup_ui()
        
    def setup_ui(self):
        style = ttk.Style()
        style.theme_use('clam')
        style.configure("TFrame", background="#2d2d2d")
        style.configure("TLabel", background="#2d2d2d", foreground="#ffffff", font=("Helvetica", 11))
        style.configure("TButton", font=("Helvetica", 10, "bold"), padding=5)
        
        # Connection Frame
        conn_frame = ttk.LabelFrame(self.root, text="Connection", padding=(10, 5))
        conn_frame.pack(fill="x", padx=10, pady=5)
        
        ttk.Label(conn_frame, text="Port:").grid(row=0, column=0, padx=5, pady=5)
        self.port_entry = ttk.Entry(conn_frame, width=15)
        self.port_entry.insert(0, "/dev/serial0")
        self.port_entry.grid(row=0, column=1, padx=5, pady=5)
        
        self.connect_btn = ttk.Button(conn_frame, text="Connect", command=self.toggle_connection)
        self.connect_btn.grid(row=0, column=2, padx=5, pady=5)
        
        self.status_lbl = ttk.Label(conn_frame, text="Status: Disconnected", foreground="#ff6b6b")
        self.status_lbl.grid(row=0, column=3, padx=15, pady=5)

        # Controls Frame
        ctrl_frame = ttk.LabelFrame(self.root, text="Robot Control", padding=(10, 5))
        ctrl_frame.pack(fill="x", padx=10, pady=5)
        
        self.start_btn = ttk.Button(ctrl_frame, text="START ROBOT", command=lambda: self.send_cmd("START"))
        self.start_btn.pack(side="left", padx=5)
        
        self.stop_btn = ttk.Button(ctrl_frame, text="STOP ROBOT", command=lambda: self.send_cmd("STOP"))
        self.stop_btn.pack(side="left", padx=5)

        # PID Tuning Frame
        pid_frame = ttk.LabelFrame(self.root, text="PID Tuning", padding=(10, 10))
        pid_frame.pack(fill="x", padx=10, pady=5)
        
        self.p_var = tk.DoubleVar(value=15.0)
        self.i_var = tk.DoubleVar(value=0.0)
        self.d_var = tk.DoubleVar(value=5.0)

        self.create_slider(pid_frame, "Kp (Proportional)", self.p_var, 0, 50, 0)
        self.create_slider(pid_frame, "Ki (Integral)", self.i_var, 0, 20, 1)
        self.create_slider(pid_frame, "Kd (Derivative)", self.d_var, 0, 30, 2)
        
        update_btn = ttk.Button(pid_frame, text="Send PID Values", command=self.send_pid)
        update_btn.grid(row=3, column=0, columnspan=3, pady=15)

        # Telemetry Frame
        tele_frame = ttk.LabelFrame(self.root, text="Telemetry", padding=(10, 10))
        tele_frame.pack(fill="both", expand=True, padx=10, pady=5)

        self.error_lbl = ttk.Label(tele_frame, text="Line Error: --", font=("Helvetica", 14, "bold"), foreground="#4dabf7")
        self.error_lbl.pack(anchor="w", pady=2)
        
        self.current_pid_lbl = ttk.Label(tele_frame, text="Current Setup -> Kp: -- | Ki: -- | Kd: --")
        self.current_pid_lbl.pack(anchor="w", pady=2)
        
    def create_slider(self, parent, label_text, variable, from_, to, row):
        ttk.Label(parent, text=label_text, width=15).grid(row=row, column=0, sticky="w", pady=5)
        slider = ttk.Scale(parent, from_=from_, to=to, variable=variable, orient="horizontal", length=250)
        slider.grid(row=row, column=1, padx=10, pady=5)
        entry = ttk.Entry(parent, textvariable=variable, width=8)
        entry.grid(row=row, column=2, padx=5, pady=5)

    def toggle_connection(self):
        if self.is_connected:
            self.stop_thread = True
            if self.serial_port:
                self.serial_port.close()
            self.is_connected = False
            self.connect_btn.config(text="Connect")
            self.status_lbl.config(text="Status: Disconnected", foreground="#ff6b6b")
        else:
            try:
                port = self.port_entry.get()
                self.serial_port = serial.Serial(port, 115200, timeout=0.1)
                self.is_connected = True
                self.stop_thread = False
                self.connect_btn.config(text="Disconnect")
                self.status_lbl.config(text="Status: Connected", foreground="#51cf66")
                
                # Start rx thread
                threading.Thread(target=self.rx_loop, daemon=True).start()
            except Exception as e:
                messagebox.showerror("Connection Error", str(e))

    def send_cmd(self, cmd):
        if self.is_connected and self.serial_port:
            self.serial_port.write(f"{cmd}\n".encode())

    def send_pid(self):
        p = self.p_var.get()
        i = self.i_var.get()
        d = self.d_var.get()
        cmd = f"P:{p:.2f},I:{i:.2f},D:{d:.2f}"
        self.send_cmd(cmd)

    def rx_loop(self):
        while not self.stop_thread:
            if self.serial_port and self.serial_port.in_waiting:
                try:
                    line = self.serial_port.readline().decode('utf-8').strip()
                    if line.startswith("T:"):
                        # T:error,baseSpeed,running,Kp,Ki,Kd
                        parts = line[2:].split(',')
                        if len(parts) == 6:
                            err, base, running, kp, ki, kd = parts
                            self.root.after(0, self.update_telemetry, err, kp, ki, kd)
                    elif line.startswith("ACK:"):
                        print(f"PICO -> {line}")
                except Exception as e:
                    print(f"Rx Error: {e}")
            time.sleep(0.01)

    def update_telemetry(self, err, kp, ki, kd):
        err_val = float(err)
        if err_val >= 999.0:
            self.error_lbl.config(text="Line Error: LOST", foreground="#ff6b6b")
        else:
            self.error_lbl.config(text=f"Line Error: {err_val:.2f}", foreground="#4dabf7")
            
        self.current_pid_lbl.config(text=f"Current Setup -> Kp: {kp} | Ki: {ki} | Kd: {kd}")

if __name__ == "__main__":
    root = tk.Tk()
    app = PIDCalibratorGUI(root)
    root.mainloop()

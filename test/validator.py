import serial
import re
import time

class SolarNodeValidator:
    def __init__(self, port, baud=115200, timeout=5):
        self.port = port
        self.baud = baud
        self.timeout = timeout
        self.ser = None

    def connect(self):
        self.ser = serial.Serial(self.port, self.baud, timeout=self.timeout)
        time.sleep(2)
        return self.ser.is_open

    def read_voltage(self):
        raw = self.ser.readline().decode('utf-8').strip()
        match = re.search(r'CH1 Bus Voltage:\s+([\d.]+)', raw)
        if match:
            return float(match.group(1))
        return None

    def disconnect(self):
        if self.ser and self.ser.is_open:
            self.ser.close()

    def validate_voltage(self, voltage, min_v=0.0, max_v=26.0):
        if voltage is None:
            return False, "no reading received"
        if voltage < min_v or voltage > max_v:
            return False, f"voltage {voltage}V out of range [{min_v}-{max_v}]"
        return True, f"voltage {voltage}V ok"

if __name__ == "__main__":
    v = SolarNodeValidator(port="COM3")
    if v.connect():
        print("Connected to ESP32")
        for _ in range(5):
            voltage = v.read_voltage()
            ok, msg = v.validate_voltage(voltage)
            print(f"{'PASS' if ok else 'FAIL'} | {msg}")
        v.disconnect()
    else:
        print("Failed to connect")

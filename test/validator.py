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

    def disconnect(self):
        if self.ser and self.ser.is_open:
            self.ser.close()

    def _readline(self):
        return self.ser.readline().decode('utf-8', errors='replace').strip()

    def read_voltage(self, max_lines=20):
        for _ in range(max_lines):
            raw = self._readline()
            match = re.search(r'CH1 Bus Voltage:\s+([\d.]+)', raw)
            if match:
                return float(match.group(1))
        return None

    def read_light(self, max_lines=20):
        for _ in range(max_lines):
            raw = self._readline()
            match = re.search(r'Light:\s+([\d.]+)', raw)
            if match:
                return float(match.group(1))
        return None

    def validate_voltage(self, voltage, min_v=0.0, max_v=26.0):
        if voltage is None:
            return False, "no reading received"
        if voltage < min_v or voltage > max_v:
            return False, f"voltage {voltage}V out of range [{min_v}-{max_v}]"
        return True, f"voltage {voltage}V ok"

    def validate_light(self, light, min_pct=0.0, max_pct=100.0):
        if light is None:
            return False, "no light reading received"
        if light < min_pct or light > max_pct:
            return False, f"light {light}% out of range [{min_pct}-{max_pct}]"
        return True, f"light {light}% ok"

if __name__ == "__main__":
    v = SolarNodeValidator(port="COM3")
    if v.connect():
        print("Connected to ESP32")
        for _ in range(5):
            voltage = v.read_voltage()
            light   = v.read_light()
            ok_v, msg_v = v.validate_voltage(voltage)
            ok_l, msg_l = v.validate_light(light)
            print(f"{'PASS' if ok_v else 'FAIL'} | {msg_v}")
            print(f"{'PASS' if ok_l else 'FAIL'} | {msg_l}")
        v.disconnect()
    else:
        print("Failed to connect")

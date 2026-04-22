import pytest
from validator import SolarNodeValidator

@pytest.fixture
def validator():
    v = SolarNodeValidator(port="COM3")
    v.connect()
    yield v
    v.disconnect()

def test_connection(validator):
    assert validator.ser.is_open, "Serial connection is not open"

def test_voltage_not_none(validator):
    voltage = validator.read_voltage()
    assert voltage is not None, "No voltage reading received from serial"

def test_voltage_in_range(validator):
    voltage = validator.read_voltage()
    ok, msg = validator.validate_voltage(voltage, min_v=0.0, max_v=26.0)
    assert ok, msg

def test_light_not_none(validator):
    light = validator.read_light()
    assert light is not None, "No light reading received from serial"

def test_light_in_range(validator):
    light = validator.read_light()
    ok, msg = validator.validate_light(light, min_pct=0.0, max_pct=100.0)
    assert ok, msg

def test_multiple_readings(validator):
    readings = [validator.read_voltage() for _ in range(5)]
    valid = [v for v in readings if v is not None]
    assert len(valid) >= 3, f"Too many None readings: {readings}"
    assert max(valid) - min(valid) < 1.0, f"Readings unstable: {valid}"

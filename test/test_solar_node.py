import pytest
from validator import SolarNodeValidator

@pytest.fixture
def validator():
    v = SolarNodeValidator(port="COM3")
    v.connect()
    yield v
    v.disconnect()

def test_voltage_in_range(validator):
    voltage = validator.read_voltage()
    ok, msg = validator.validate_voltage(voltage, min_v=0.0, max_v=26.0)
    assert ok, msg

def test_voltage_not_none(validator):
    voltage = validator.read_voltage()
    assert voltage is not None, "No voltage reading received from serial"

def test_connection(validator):
    assert validator.ser.is_open, "Serial connection is not open"

def test_multiple_readings(validator):
    readings = [validator.read_voltage() for _ in range(5)]
    assert all(v is not None for v in readings), "Some readings return None"
    assert max(readings) - min(readings) < 1.0, "Readings are unstable"

    
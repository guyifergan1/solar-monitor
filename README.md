# Solar IoT Node — ESP32

> Modular solar monitoring system built with ESP32, OLED SSD1306, INA3221, and LDR.  
> Part of a 3-project embedded systems portfolio targeting real-world IoT applications.

---

## Hardware

| Component | Interface | Pin/Address |
|-----------|-----------|-------------|
| ESP32 DevKit | — | — |
| OLED SSD1306 (128×64) | I2C | 0x3C |
| INA3221 (3-channel voltage/current sensor) | I2C | 0x40 |
| LDR (light sensor) + 10kΩ resistor | ADC | GPIO34 |

---

## Wiring

### I2C (OLED + INA3221)

| ESP32 Pin | Signal | OLED | INA3221 |
|-----------|--------|------|---------|
| GPIO 21 | SDA | SDA | SDA |
| GPIO 22 | SCL | SCL | SCL |
| 3.3V | VCC | VCC | VCC |
| GND | GND | GND | GND |

### LDR Voltage Divider

```
3.3V → LDR → GPIO34 → 10kΩ → GND
```

Both LDR legs connect at GPIO34 — that junction point is the measurement node.

---

## Project Structure

```
src/
├── config.h        — All #defines: pins, addresses, sleep interval
├── display.h/.cpp  — OLED: init, voltage, light, error, off
├── sensors.h/.cpp  — INA3221: init, bus voltage read
├── ldr.h/.cpp      — LDR: init, read light as 0-100%
└── main.cpp        — setup() + loop() only
test/
├── validator.py        — Serial reader and voltage validator class
├── test_solar_node.py  — pytest tests (connection, range, stability)
└── requirements.txt    — pyserial, pytest
```

Each hardware module has its own `.h`/`.cpp` pair. Hardware objects are `static` inside their `.cpp` files — never exposed outside the module.

---

## Architecture Decisions

**Deep Sleep** — ESP32 reads sensors, displays, then sleeps. Wakes on timer. `setup()` runs on every wake cycle, `loop()` is empty.  
**Graceful degradation** — each peripheral returns `bool` from init. System continues if one device is missing.  
**`F()` macro** — string literals kept in Flash, not RAM.  
**Encapsulation** — hardware objects hidden inside translation units, exposed only via function calls.  
**`RTC_DATA_ATTR`** — boot counter survives Deep Sleep via RTC memory.  
**OLED off before sleep** — `displayOff()` clears screen before sleep to save power.

---

## Serial Output

```
=== Solar IoT Node - Stage 3 | Boot #1 ===
[OK] SSD1306 OLED initialized at 0x3C
[OK] INA3221 initialized at 0x40
[DATA] CH1 Bus Voltage: 3.300 V
[DATA] Light: 75.8 %
[INFO] Sleeping for 10 seconds...

=== Solar IoT Node - Stage 3 | Boot #2 ===
...
```

---

## Stages

### Stage 1 — OLED + INA3221 ✅
- I2C bus init with explicit pins (GPIO21/22)
- OLED displays live voltage reading
- INA3221 reads CH1 bus voltage
- Panic handler (LED blink) if no I2C devices found

### Stage 2 — LDR Light Sensor ✅
- LDR + 10kΩ voltage divider on GPIO34 (ADC1)
- Reads light level as 0-100%
- OLED alternates between voltage and light screens
- New module: `ldr.h` / `ldr.cpp`

### Stage 3 — Deep Sleep ✅
- ESP32 reads sensors → displays → sleeps 10 seconds → wakes
- `setup()` runs on every wake cycle
- Boot count tracked across sleep cycles via RTC memory
- OLED cleared before sleep to save power

### Stage 4 — WiFi + Data Logging 🔜
- Send readings to server / MQTT / Google Sheets
- Over-the-air updates

### Stage 5 — Full Solar Monitoring 🔜
- Solar panel (6V) via LM2596 buck converter
- INA3221 CH1: panel voltage, CH2: battery, CH3: load current
- Fully autonomous — no USB required

---

## Setup

1. Install [PlatformIO](https://platformio.org/)
2. Clone this repo
3. Open in VS Code — PlatformIO auto-installs all libraries
4. Upload to ESP32

**Dependencies** (auto-installed):
- `adafruit/Adafruit INA3221 Library @ ^1.0.1`
- `adafruit/Adafruit GFX Library @ ^1.11.9`
- `adafruit/Adafruit SSD1306 @ ^2.5.9`

## Running Python Validation Tests

```bash
cd test
pip install -r requirements.txt
pytest test_solar_node.py -v
```

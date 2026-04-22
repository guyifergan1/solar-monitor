# Solar IoT Node — ESP32

> Low-power solar monitoring node built on ESP32 with modular firmware architecture.  
> Reads voltage (INA3221) and ambient light (LDR), renders live data on an OLED display,  
> and enters Deep Sleep between cycles to minimise power consumption.  
> Part of an embedded systems portfolio targeting real-world IoT applications.

---

## Hardware

| Component | Interface | Pin / Address |
|-----------|-----------|---------------|
| ESP32 DevKit | — | — |
| OLED SSD1306 (128×64) | I2C | 0x3C |
| INA3221 (3-channel voltage/current sensor) | I2C | 0x40 |
| LDR + 10 kΩ pull-down resistor | ADC | GPIO34 |

---

## Wiring

### I2C Bus (OLED + INA3221)

| ESP32 Pin | Signal | OLED | INA3221 |
|-----------|--------|------|---------|
| GPIO 21 | SDA | SDA | SDA |
| GPIO 22 | SCL | SCL | SCL |
| 3.3 V | VCC | VCC | VCC |
| GND | GND | GND | GND |

### LDR Voltage Divider

```
3.3V → LDR → GPIO34 → 10kΩ → GND
```

The midpoint between the LDR and the pull-down resistor is connected to GPIO34, which is the ADC measurement node.

---

## Project Structure

```
src/
├── config.h        — Centralised #defines: pins, I2C addresses, timing constants
├── display.h/.cpp  — OLED driver: init, combined data screen, error screen, power-off
├── sensors.h/.cpp  — INA3221 driver: init, per-channel bus voltage read
├── ldr.h/.cpp      — LDR driver: init, ADC read with clamp, return as 0–100 %
└── main.cpp        — Entry point: wake → sense → display → sleep
test/
├── validator.py        — Serial port reader; validates voltage and light readings
├── test_solar_node.py  — pytest suite: connection, range, light, stability (7 tests)
└── requirements.txt    — pyserial, pytest
```

Each hardware driver is a self-contained `.h`/`.cpp` pair. The hardware object is `static` inside the `.cpp` file and never exposed beyond that translation unit — all interaction goes through the public API functions.

---

## Architecture Decisions

**Deep Sleep** — The node reads sensors, updates the display for 5 s, then sleeps for 25 s (30 s total cycle). `setup()` executes on every wake; `loop()` is intentionally empty.  
**Graceful degradation** — Every peripheral `init` function returns `bool`. The system logs the fault and continues if a device is absent.  
**`F()` macro** — String literals are stored in Flash rather than copied to RAM at runtime.  
**Encapsulation** — Hardware objects are hidden inside their translation units and accessed only through function calls, preventing unintended shared state.  
**`RTC_DATA_ATTR`** — The boot counter is placed in RTC-retained memory so it survives Deep Sleep without a battery-backed RTC chip.  
**OLED off before sleep** — `displayOff()` clears and blanks the display before the MCU sleeps, eliminating screen idle current.  
**Continuous polling** — Sensors are sampled every 300 ms during the wake window; both Serial and OLED reflect each reading in real time.

---

## Serial Output

Sensors are polled every 300 ms during the 5 s wake window, then the node sleeps for 25 s.

```
=== Solar IoT Node - Stage 3 | Boot #1 ===
[OK] SSD1306 OLED initialized at 0x3C
[OK] INA3221 initialized at 0x40
[DATA] CH1 Bus Voltage: 3.300 V
[DATA] Light: 75.8 %
[DATA] CH1 Bus Voltage: 3.301 V
[DATA] Light: 75.6 %
... (repeats every 300 ms for 5 seconds)
[INFO] Sleeping for 25 seconds...

=== Solar IoT Node - Stage 3 | Boot #2 ===
...
```

---

## Stages

### Stage 1 — OLED + INA3221 ✅
- I2C bus initialised with explicit SDA/SCL pins (GPIO21/22)
- INA3221 reads CH1 bus voltage; result rendered on OLED
- Fault logged to Serial if no I2C devices are detected

### Stage 2 — LDR Light Sensor ✅
- LDR + 10 kΩ voltage divider on GPIO34 (ADC1 — safe with WiFi)
- ADC value clamped to [0, 4095] before conversion to 0–100 %
- Isolated into its own `ldr.h` / `ldr.cpp` module

### Stage 3 — Deep Sleep ✅
- 30 s duty cycle: 5 s active (sense + display) → 25 s Deep Sleep
- Single OLED frame shows bus voltage, light percentage, and boot count
- Boot count persisted across sleep cycles via `RTC_DATA_ATTR`
- Sensors polled every 300 ms while awake; Serial and OLED update in real time

### Stage 4 — WiFi + Remote Logging 🔜
- Connect to WiFi on wake; publish readings via MQTT or HTTP
- Log data to a time-series store (InfluxDB, Google Sheets, or similar)
- Over-the-air (OTA) firmware updates

### Stage 5 — Autonomous Solar Operation 🔜
- 6 V solar panel stepped down via LM2596 buck converter
- INA3221 CH1: panel voltage / CH2: battery voltage / CH3: load current
- Fully off-grid — no USB power required

---

## Setup

1. Install [PlatformIO](https://platformio.org/)
2. Clone this repository
3. Open in VS Code — PlatformIO resolves and installs all libraries automatically
4. Upload to the ESP32 via the PlatformIO toolbar

**Library dependencies** (auto-installed via `platformio.ini`):
- `adafruit/Adafruit INA3221 Library @ ^1.0.1`
- `adafruit/Adafruit GFX Library @ ^1.11.9`
- `adafruit/Adafruit SSD1306 @ ^2.5.9`

---

## Validation Tests

```bash
cd test
pip install -r requirements.txt
pytest test_solar_node.py -v
```

Tests verify serial connectivity, voltage range (0–26 V), light range (0–100 %), and reading stability across five consecutive samples.

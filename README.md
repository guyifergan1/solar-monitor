# Solar IoT Node — ESP32

> Modular solar monitoring system built with ESP32, OLED SSD1306, and INA3221.  
> Part of a 3-project embedded systems portfolio targeting real-world IoT applications.

---

## Hardware

| Component | Interface | Address |
|-----------|-----------|---------|
| ESP32 DevKit | — | — |
| OLED SSD1306 (128×64) | I2C | 0x3C |
| INA3221 (3-channel voltage/current sensor) | I2C | 0x40 |

---

## Wiring

| ESP32 Pin | Signal | OLED | INA3221 |
|-----------|--------|------|---------|
| GPIO 21 | SDA | SDA | SDA |
| GPIO 22 | SCL | SCL | SCL |
| 3.3V | VCC | VCC | VCC |
| GND | GND | GND | GND |

> **Test connection:** 3.3V → V1+, GND → V1− on INA3221 to verify ~3.3V reading on CH1.

---

## Project Structure

```
src/
├── config.h        — Pin definitions, I2C addresses, timing constants
├── display.h/.cpp  — OLED init, voltage display, error screen
├── sensors.h/.cpp  — INA3221 init, bus voltage read per channel
└── main.cpp        — setup() and loop() only
```

Each hardware module has its own `.h`/`.cpp` pair. The hardware objects (`Adafruit_SSD1306`, `Adafruit_INA3221`) are `static` inside their `.cpp` files — never exposed outside the module.

---

## Architecture Decisions

**No `delay()`** — timing is handled with `millis()` so the MCU is never blocked.  
**Graceful degradation** — each peripheral returns `bool` from init. The system continues if one device is missing.  
**`F()` macro** — string literals kept in Flash, not RAM.  
**Encapsulation** — hardware objects are hidden inside their translation unit, exposed only via function calls.

---

## Serial Output

```
=== Solar IoT Node - Stage 1 ===
[OK] SSD1306 OLED initialized at 0x3C
[OK] INA3221 initialized at 0x40
[DATA] CH1 Bus Voltage: 3.300 V
```

---

## Stages

### Stage 1 — OLED + INA3221 ✅
- I2C bus init with explicit pins
- OLED displays live voltage reading
- INA3221 reads CH1 bus voltage every 2 seconds
- Panic handler (LED blink) if no I2C devices found

### Stage 2 — LDR Light Sensor 🔜
- Add LDR (photoresistor) on ADC pin with voltage divider
- New module: `ldr.h` / `ldr.cpp`
- Display light level alongside voltage on OLED

### Stage 3 — Deep Sleep + Power Optimization 🔜
- ESP32 Deep Sleep between readings
- Wake on timer
- Measure and minimize current consumption

### Stage 4 — Full Solar Monitoring 🔜
- Solar panel voltage (CH1), battery voltage (CH2), load current (CH3)
- GPS module (u-blox NEO) for location tagging
- Data logging

---

## Setup

1. Install [PlatformIO](https://platformio.org/)
2. Clone this repo
3. Open in VS Code — PlatformIO auto-installs all libraries via `platformio.ini`
4. Upload to ESP32

**Dependencies** (auto-installed):
- `adafruit/Adafruit INA3221 Library @ ^1.0.1`
- `adafruit/Adafruit GFX Library @ ^1.11.9`
- `adafruit/Adafruit SSD1306 @ ^2.5.9`

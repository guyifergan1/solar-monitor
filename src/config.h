#pragma once
#include <Arduino.h>

// ── I2C Pins ──────────────────────────────────────────────────────────────────
#define PIN_I2C_SDA     21
#define PIN_I2C_SCL     22

// ── Device Addresses ──────────────────────────────────────────────────────────
#define ADDR_OLED       0x3C
#define ADDR_INA3221    0x40    // A0 pin → GND on breakout board

// ── OLED Dimensions ───────────────────────────────────────────────────────────
#define SCREEN_W        128
#define SCREEN_H        64
#define OLED_RESET      -1      // No dedicated reset pin on most breakouts

// ── Onboard LED ───────────────────────────────────────────────────────────────
// ESP32 DevKit GPIO2 — not always defined by the framework
#ifndef LED_BUILTIN
#define LED_BUILTIN     2
#endif

// ── LDR ───────────────────────────────────────────────────────────────────────
#define LDR_PIN     34      // GPIO34 — ADC1, input-only, safe with WiFi
#define LDR_MAX     4095    // ESP32 ADC is 12-bit (0–4095)

// ── Deep Sleep ────────────────────────────────────────────────────────────────
#define DISPLAY_ON_MS       5000UL           // screen stays on 5 seconds before sleep
#define SLEEP_INTERVAL_US   25000000ULL      // sleep 25 seconds (total cycle ~30s)



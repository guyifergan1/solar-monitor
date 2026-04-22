/**
 * @file  main.cpp
 * @brief Solar IoT Node — Stage 3: Deep Sleep.
 *
 * On every wake cycle:
 *   1. Read sensors (INA3221 + LDR)
 *   2. Display on OLED
 *   3. Enter Deep Sleep for SLEEP_INTERVAL_US
 *
 * setup() runs on every wake. loop() is intentionally empty.
 */

#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "display.h"
#include "sensors.h"
#include "ldr.h"

RTC_DATA_ATTR static uint32_t bootCount = 0;

void setup() {
    bootCount++;

    Serial.begin(115200);
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    Serial.printf("\n=== Solar IoT Node - Stage 3 | Boot #%u ===\n", bootCount);

    bool oledOk = initOLED();
    bool inaOk  = initINA3221();
    initLDR();

    if (!oledOk && !inaOk) {
        Serial.println(F("[FATAL] No I2C peripherals. Check wiring!"));
    }

    float busVoltage = 0.0f;
    float light      = 0.0f;

    unsigned long wakeStart = millis();
    while (millis() - wakeStart < DISPLAY_ON_MS) {
        busVoltage = inaOk ? readBusVoltage(0) : 0.0f;
        light      = readLightPercentage();

        Serial.print(F("[DATA] CH1 Bus Voltage: "));
        Serial.print(busVoltage, 3);
        Serial.println(F(" V"));
        Serial.print(F("[DATA] Light: "));
        Serial.print(light, 1);
        Serial.println(F(" %"));

        if (oledOk) displayAll(busVoltage, light, bootCount);
        delay(300);
    }

    Serial.printf("[INFO] Sleeping for %llu seconds...\n", SLEEP_INTERVAL_US / 1000000ULL);
    Serial.flush();
    if (oledOk) displayOff();

    esp_sleep_enable_timer_wakeup(SLEEP_INTERVAL_US);
    esp_deep_sleep_start();
}

void loop() {}

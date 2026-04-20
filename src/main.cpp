/**
 * @file  main.cpp
 * @brief Solar IoT Node — Stage 1 entry point.
 *
 * setup() and loop() only. All hardware logic lives in:
 *   display.cpp  — OLED init and draw
 *   sensors.cpp  — INA3221 init and read
 *   config.h     — pins, addresses, timing constants
 */

#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "display.h"
#include "sensors.h"

static bool     oledOk     = false;
static bool     inaOk      = false;
static uint32_t lastReadMs = 0;

void setup() {
    Serial.begin(115200);
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    Serial.println(F("\n=== Solar IoT Node - Stage 1 ==="));

    oledOk = initOLED();
    inaOk  = initINA3221();

    // If nothing responds on I2C the wiring is wrong — blink LED as a panic indicator
    if (!oledOk && !inaOk) {
        Serial.println(F("[FATAL] No I2C peripherals detected. Check wiring!"));
        pinMode(LED_BUILTIN, OUTPUT);
        while (true) {
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            delay(150);
        }
    }
}

void loop() {
    // millis() subtraction handles the ~49-day rollover correctly
    if (millis() - lastReadMs < READ_INTERVAL_MS) return;
    lastReadMs = millis();

    if (!inaOk) {
        Serial.println(F("[WARN] INA3221 unavailable — skipping read"));
        if (oledOk) displayError("INA3221\nNot Found");
        return;
    }

    float busVoltage = readBusVoltage(0);   // CH1 = index 0

    Serial.print(F("[DATA] CH1 Bus Voltage: "));
    Serial.print(busVoltage, 3);
    Serial.println(F(" V"));

    if (oledOk) displayVoltage(busVoltage);
}

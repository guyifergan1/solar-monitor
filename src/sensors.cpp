#include "sensors.h"
#include "config.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA3221.h>

// The INA3221 object is private to this translation unit.
static Adafruit_INA3221 ina3221;

bool initINA3221() {
    if (!ina3221.begin(ADDR_INA3221, &Wire)) {
        Serial.println(F("[ERROR] INA3221 init failed — address 0x40 not found"));
        return false;
    }
    Serial.println(F("[OK] INA3221 initialized at 0x40"));
    return true;
}

float readBusVoltage(uint8_t channel) {
    // Library channels are 0-indexed: CH1=0, CH2=1, CH3=2.
    // getBusVoltage returns NaN if channel > 2, so the caller
    // should validate or check for NaN if needed.
    return ina3221.getBusVoltage(channel);
}

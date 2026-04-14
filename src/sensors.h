#pragma once
#include <Arduino.h>

/**
 * @file  sensors.h
 * @brief INA3221 interface — init and read functions.
 *
 * The INA3221 object lives inside sensors.cpp (translation-unit scope).
 * Callers only see these two functions — hardware details are hidden.
 */

bool  initINA3221();
float readBusVoltage(uint8_t channel);

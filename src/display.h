#pragma once
#include <Arduino.h>

/**
 * @file  display.h
 * @brief OLED SSD1306 interface — init and draw functions.
 *
 * The display object lives inside display.cpp (translation-unit scope).
 * All callers use only these three functions — the hardware object
 * is never exposed outside this module.
 */

bool initOLED();
void displayAll(float volts, float lightPct, uint32_t bootNum);
void displayError(const char* msg);
void displayOff();

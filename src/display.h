#pragma once

/**
 * @file  display.h
 * @brief OLED SSD1306 interface — init and draw functions.
 *
 * The display object lives inside display.cpp (translation-unit scope).
 * All callers use only these three functions — the hardware object
 * is never exposed outside this module.
 */

bool initOLED();
void displayVoltage(float volts);
void displayError(const char* msg);
void displayLight(float percentage);
void displayOff();

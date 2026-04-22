#include "display.h"
#include "config.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// The display object is private to this translation unit.
// No other file can access it directly — only through the functions below.
static Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, OLED_RESET);

bool initOLED() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, ADDR_OLED)) {
        Serial.println(F("[ERROR] SSD1306 init failed — address 0x3C not found"));
        return false;
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("Solar IoT Node"));
    display.println(F("Initializing..."));
    display.display();
    Serial.println(F("[OK] SSD1306 OLED initialized at 0x3C"));
    return true;
}

void displayAll(float volts, float lightPct, uint32_t bootNum) {
    char vStr[10];
    char lStr[8];
    // dtostrf: portable float->string, avoids printf float issues on some toolchains
    dtostrf(volts,   6, 3, vStr);
    dtostrf(lightPct, 5, 1, lStr);

    display.clearDisplay();

    // Header
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(F("Solar IoT Node"));
    display.drawFastHLine(0, 10, SCREEN_W, SSD1306_WHITE);

    // Voltage — large
    display.setTextSize(2);
    display.setCursor(0, 14);
    display.print(vStr);
    display.print(F("V"));

    // Light — large
    display.setTextSize(2);
    display.setCursor(0, 34);
    display.print(lStr);
    display.print(F("%"));

    // Footer — boot count
    display.setTextSize(1);
    display.setCursor(0, 56);
    display.printf("Boot #%u", bootNum);

    display.display();
}

void displayError(const char* msg) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("[ ERROR ]"));
    display.drawFastHLine(0, 10, SCREEN_W, SSD1306_WHITE);
    display.setCursor(0, 16);
    display.println(msg);
    display.display();
}

void displayOff() {
    display.clearDisplay();
    display.display();
}

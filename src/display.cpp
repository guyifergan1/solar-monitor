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
    display.println(F("Stage 1 - Ready"));
    display.display();
    Serial.println(F("[OK] SSD1306 OLED initialized at 0x3C"));
    return true;
}

void displayVoltage(float volts) {
    // dtostrf: portable float->string conversion.
    // Avoids printf float-support issues on some toolchains.
    char vStr[10];
    dtostrf(volts, 6, 3, vStr);

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(F("Solar IoT Node"));
    display.drawFastHLine(0, 10, SCREEN_W, SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(8, 20);
    display.print(vStr);
    display.print(F(" V"));

    display.setTextSize(1);
    display.setCursor(0, 52);
    display.print(F("INA3221 CH1 Bus"));

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

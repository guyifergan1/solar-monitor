#include "ldr.h"
#include "config.h"

void initLDR() {
    pinMode(LDR_PIN, INPUT);
}

float readLightPercentage() {
    int raw = analogRead(LDR_PIN);
    return (raw / (float)LDR_MAX) * 100.0f;
}
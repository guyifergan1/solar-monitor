#include "ldr.h"
#include "config.h"

void initLDR() {
    pinMode(LDR_PIN, INPUT);
}

float readLightPercentage() {
    int raw = analogRead(LDR_PIN);
    if (raw < 0)        raw = 0;
    if (raw > LDR_MAX)  raw = LDR_MAX;
    return (raw / (float)LDR_MAX) * 100.0f;
}
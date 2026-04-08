#include "SensorApp.h"

static const uint8_t TRIG_PIN = 26;
static const uint8_t ECHO_PIN = 25;

SensorApp app(TRIG_PIN, ECHO_PIN);

void setup() {
    app.begin();
}

void loop() {
    app.update();
}
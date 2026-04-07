#include "ActuatorApp.h"

static const uint8_t RED_LED_PIN    = 5;
static const uint8_t YELLOW_LED_PIN = 17; 
static const uint8_t GREEN_LED_PIN  = 16;

ActuatorApp app(RED_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN);

void setup() {
    app.begin();
}

void loop() {
    app.update();
}
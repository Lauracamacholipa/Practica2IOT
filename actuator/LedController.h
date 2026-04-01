#pragma once
#include <Arduino.h>

class LedController {
public:
    LedController(uint8_t redPin, uint8_t greenPin, uint8_t statusPin)
        : _redPin(redPin), _greenPin(greenPin), _statusPin(statusPin),
          _lastBlinkTime(0) {}

    void begin() {
        pinMode(_redPin,    OUTPUT);
        pinMode(_greenPin,  OUTPUT);
        pinMode(_statusPin, OUTPUT);
        turnAllOff();
    }

    void turnAllOff() {
        digitalWrite(_redPin,    LOW);
        digitalWrite(_greenPin,  LOW);
        digitalWrite(_statusPin, LOW);
    }

    void executeCommand(const String& command) {
        if      (command == "CMD:RED")   _activateRed();
        else if (command == "CMD:GREEN") _activateGreen();
        else Serial.println("[LED] Unknown command: " + command);
    }

    // Call on every loop iteration for non-blocking blink
    void updateStatusBlink() {
        if (millis() - _lastBlinkTime > STATUS_BLINK_INTERVAL_MS) {
            digitalWrite(_statusPin, !digitalRead(_statusPin));
            _lastBlinkTime = millis();
        }
    }

private:
    static constexpr unsigned long STATUS_BLINK_INTERVAL_MS = 800;

    uint8_t       _redPin;
    uint8_t       _greenPin;
    uint8_t       _statusPin;
    unsigned long _lastBlinkTime;

    void _activateRed() {
        digitalWrite(_redPin,   HIGH);
        digitalWrite(_greenPin, LOW);
        Serial.println("[LED] Red ON");
    }

    void _activateGreen() {
        digitalWrite(_redPin,   LOW);
        digitalWrite(_greenPin, HIGH);
        Serial.println("[LED] Green ON");
    }
};
#pragma once
#include <Arduino.h>

class LedController {
public:
    LedController(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin)
        : _redPin(redPin), _yellowPin(yellowPin), _greenPin(greenPin) {}

    void begin() {
        pinMode(_redPin,    OUTPUT);
        pinMode(_yellowPin, OUTPUT);
        pinMode(_greenPin,  OUTPUT);
        turnAllOff();
    }

    void turnAllOff() {
        digitalWrite(_redPin,    LOW);
        digitalWrite(_yellowPin, LOW);
        digitalWrite(_greenPin,  LOW);
    }

    void executeCommand(const String& command) {
        if      (command == "CMD:RED")    _activateRed();
        else if (command == "CMD:YELLOW") _activateYellow();
        else if (command == "CMD:GREEN")  _activateGreen();
        else if (command == "CMD:OFF")  turnAllOff();
        else Serial.println("[LED] Unknown command: " + command);
    }

private:
    static constexpr unsigned long STATUS_BLINK_INTERVAL_MS = 800;

    uint8_t       _redPin;
    uint8_t       _yellowPin;
    uint8_t       _greenPin;
    
    void _activateRed() {
        digitalWrite(_redPin,    HIGH);
        digitalWrite(_yellowPin, LOW);
        digitalWrite(_greenPin,  LOW);
        Serial.println("[LED] Red ON");
    }

    void _activateYellow() {
        digitalWrite(_redPin,    LOW);
        digitalWrite(_yellowPin, HIGH);
        digitalWrite(_greenPin,  LOW);
        Serial.println("[LED] Yellow ON");
    }

    void _activateGreen() {
        digitalWrite(_redPin,    LOW);
        digitalWrite(_yellowPin, LOW);
        digitalWrite(_greenPin,  HIGH);
        Serial.println("[LED] Green ON");
    }
};
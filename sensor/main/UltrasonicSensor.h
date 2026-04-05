#pragma once
#include <Arduino.h>

class UltrasonicSensor {
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin)
        : _trigPin(trigPin), _echoPin(echoPin) {}

    void begin() {
        pinMode(_trigPin, OUTPUT);
        pinMode(_echoPin, INPUT);
    }

    // Returns distance in cm, or -1.0 if no echo received
    float readDistance() {
        _sendTriggerPulse();
        long duration = pulseIn(_echoPin, HIGH, ECHO_TIMEOUT_US);
        if (duration == 0) return INVALID_READING;
        return duration * SOUND_SPEED_CM_US / 2.0f;
    }

private:
    static constexpr float SOUND_SPEED_CM_US = 0.0343f;
    static constexpr float INVALID_READING   = -1.0f;
    static constexpr unsigned long ECHO_TIMEOUT_US = 30000UL;

    uint8_t _trigPin;
    uint8_t _echoPin;

    void _sendTriggerPulse() {
        digitalWrite(_trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(_trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(_trigPin, LOW);
    }
};
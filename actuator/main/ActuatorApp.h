#pragma once
#include "LedController.h"
#include "TcpActuatorClient.h"
#include "NetworkConfig.h"

class ActuatorApp {
public:
    ActuatorApp(uint8_t redPin, uint8_t yellowPin, uint8_t greenPin)
        : _leds(redPin, yellowPin, greenPin) {}

    void begin() {
        Serial.begin(115200);
        _leds.begin();
        _client.connectToWifi();
        _client.connectAndRegister();
    }

    void update() {
        if (!_client.isWifiConnected()) {
            _client.connectToWifi();
        }

        if (!_client.isServerConnected()) {
            Serial.println("[TCP] Reconnecting to server...");
            _leds.turnAllOff();
            delay(RECONNECT_DELAY_MS);
            _client.connectAndRegister();
            return;
        }

        String command = _client.readCommand();
        if (command.length() > 0) {
            _leds.executeCommand(command);
        }
    }

private:
    LedController     _leds;
    TcpActuatorClient _client;
};
#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "NetworkConfig.h"

class TcpActuatorClient {
public:
    TcpActuatorClient() : _receiveBuffer("") {}

    void connectToWifi() {
        Serial.print("[WiFi] Connecting");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\n[WiFi] Connected · IP: " + WiFi.localIP().toString());
    }

    bool connectAndRegister() {
        if (!_client.connect(SERVER_IP, SERVER_PORT)) {
            Serial.println("[TCP] Failed to connect to server");
            return false;
        }
        _registerAsActuator();
        return true;
    }

    bool isWifiConnected()  { return WiFi.status() == WL_CONNECTED; }
    bool isServerConnected(){ return _client.connected(); }

    // Returns the next complete command, or "" if none available
    String readCommand() {
        while (_client.available()) {
            char c = _client.read();
            _receiveBuffer += c;

            if (c == '\n') {
                _receiveBuffer.trim();
                String command   = _receiveBuffer;
                _receiveBuffer   = "";
                if (command.length() > 0) return command;
            }
        }
        return "";
    }

private:
    static constexpr int REGISTRATION_TIMEOUT_MS = 3000;

    WiFiClient _client;
    String     _receiveBuffer;

    void _registerAsActuator() {
        _client.print("TYPE:ACTUATOR\n");
        Serial.println("[TCP] Registration message sent");
        _waitForRegistrationAck();
    }

    void _waitForRegistrationAck() {
        unsigned long startTime = millis();

        while (millis() - startTime < REGISTRATION_TIMEOUT_MS) {
            while (_client.available()) {
                char c = _client.read();
                _receiveBuffer += c;

                if (c == '\n') {
                    _receiveBuffer.trim();
                    Serial.println("[TCP] Server response: " + _receiveBuffer);
                    _receiveBuffer = "";
                    return;
                }
            }
            delay(10);
        }
        Serial.println("[TCP] No registration acknowledgment received");
    }
};
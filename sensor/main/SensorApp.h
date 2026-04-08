#pragma once
#include <Arduino.h>
#include "UltrasonicSensor.h"
#include "TcpSensorClient.h"
#include "NetworkConfig.h"

class SensorApp {
public:
    SensorApp(uint8_t trigPin, uint8_t echoPin)
        : _sensor(trigPin, echoPin) {}

    void begin() {
        Serial.begin(115200);
        _sensor.begin();
        _client.connectToWifi();

        if (!_client.connectToServer()) {
            Serial.println("[TCP] Failed to connect to server");
        }
    }

    void update() {
        if (!_ensureConnections()) {
            delay(RECONNECT_DELAY_MS);
            return;
        }

        float distance = _sensor.readDistance();

        if (distance < 0) {
            Serial.println("[Sensor] No echo received");
            delay(SEND_INTERVAL_MS);
            return;
        }

        if (!_client.sendDistance(distance)) {
            Serial.println("[TCP] Message lost - no ACK received");
            _client.disconnect();
        }

        delay(SEND_INTERVAL_MS);
    }

private:
    UltrasonicSensor _sensor;
    TcpSensorClient  _client;

    bool _ensureConnections() {
        if (!_client.isWifiConnected()) {
            Serial.println("[WiFi] Connection lost, reconnecting...");
            _client.disconnect();
            _client.connectToWifi();
        }

        if (!_client.isServerConnected()) {
            Serial.println("[TCP] Reconnecting to server...");
            _client.disconnect();
            delay(RECONNECT_DELAY_MS);

            if (!_client.connectToServer()) {
                Serial.println("[TCP] Reconnection failed, retrying...");
                return false;
            }

            Serial.println("[TCP] Reconnected successfully");
        }

        return true;
    }
};
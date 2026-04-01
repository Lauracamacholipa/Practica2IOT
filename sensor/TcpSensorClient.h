#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "NetworkConfig.h"

class TcpSensorClient {
public:
    TcpSensorClient() : _sequenceNumber(0), _receiveBuffer("") {}

    void connectToWifi() {
        Serial.print("[WiFi] Connecting");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\n[WiFi] Connected · IP: " + WiFi.localIP().toString());
    }

    bool connectToServer() {
        Serial.printf("[TCP] Connecting to %s:%d\n", SERVER_IP, SERVER_PORT);
        return _client.connect(SERVER_IP, SERVER_PORT);
    }

    bool isWifiConnected()  { return WiFi.status() == WL_CONNECTED; }
    bool isServerConnected(){ return _client.connected(); }

    bool sendDistance(float distanceCm) {
        _sequenceNumber++;
        String message = _buildMessage(distanceCm, _sequenceNumber);
        Serial.printf("[TCP] Sending SEQ=%u DIST=%.1fcm\n", _sequenceNumber, distanceCm);
        return _sendWithAck(message, _sequenceNumber);
    }

private:
    WiFiClient _client;
    uint32_t   _sequenceNumber;
    String     _receiveBuffer;

    String _buildMessage(float distanceCm, uint32_t seq) {
        return "TYPE:SENSOR|SEQ:" + String(seq) +
               "|DIST:" + String(distanceCm, 1) + "\n";
    }

    bool _sendWithAck(const String& message, uint32_t seq) {
        String expectedAck = "ACK:" + String(seq);

        for (int attempt = 1; attempt <= MAX_SEND_ATTEMPTS; attempt++) {
            if (!_client.connected()) return false;

            _client.print(message);

            if (_waitForAck(expectedAck)) return true;

            Serial.printf("[TCP] ACK timeout — attempt %d/%d\n",
                          attempt, MAX_SEND_ATTEMPTS);
        }
        return false;
    }

    bool _waitForAck(const String& expectedAck) {
        unsigned long startTime = millis();

        while (millis() - startTime < ACK_TIMEOUT_MS) {
            while (_client.available()) {
                char c = _client.read();
                _receiveBuffer += c;

                if (c == '\n') {
                    _receiveBuffer.trim();
                    bool matched = (_receiveBuffer == expectedAck);
                    if (!matched) {
                        Serial.println("[TCP] Unexpected response: " + _receiveBuffer);
                    }
                    _receiveBuffer = "";
                    return matched;
                }
            }
            delay(10);
        }
        return false;
    }
};
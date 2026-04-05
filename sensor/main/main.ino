#include "NetworkConfig.h"
#include "UltrasonicSensor.h"
#include "TcpSensorClient.h"

static const uint8_t TRIG_PIN = 26;
static const uint8_t ECHO_PIN = 25;

UltrasonicSensor sensor(TRIG_PIN, ECHO_PIN);
TcpSensorClient  client;

void setup() {
    Serial.begin(115200);
    sensor.begin();
    client.connectToWifi();

    if (!client.connectToServer()) {
        Serial.println("[TCP] Failed to connect to server");
    }
}

void loop() {
    if (!client.isWifiConnected()) {
        client.connectToWifi();
    }

    if (!client.isServerConnected()) {
        Serial.println("[TCP] Reconnecting to server...");
        client.disconnect();    
        delay(RECONNECT_DELAY_MS);
        if (!client.connectToServer()) {
            Serial.println("[TCP] Reconnection failed, retrying...");
        }
        return;
    }

    float distance = sensor.readDistance();

    if (distance < 0) {
        Serial.println("[Sensor] No echo received");
        delay(SEND_INTERVAL_MS);
        return;
    }

    if (!client.sendDistance(distance)) {
        Serial.println("[TCP] Message lost — no ACK received");
    }

    delay(SEND_INTERVAL_MS);
}
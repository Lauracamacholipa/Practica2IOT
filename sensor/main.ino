#include "NetworkConfig.h"
#include "UltrasonicSensor.h"
#include "TcpSensorClient.h"

static const uint8_t TRIG_PIN = 5;
static const uint8_t ECHO_PIN = 18;

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
        delay(RECONNECT_DELAY_MS);
        client.connectToServer();
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
```

---

## Actuator — File structure
```
actuator/
├── main.ino
├── NetworkConfig.h
├── LedController.h
└── TcpActuatorClient.h
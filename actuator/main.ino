#include "NetworkConfig.h"
#include "LedController.h"
#include "TcpActuatorClient.h"

static const uint8_t RED_LED_PIN    = 25;
static const uint8_t GREEN_LED_PIN  = 26;
static const uint8_t STATUS_LED_PIN = 27;

LedController    leds(RED_LED_PIN, GREEN_LED_PIN, STATUS_LED_PIN);
TcpActuatorClient client;

void setup() {
    Serial.begin(115200);
    leds.begin();
    client.connectToWifi();
    client.connectAndRegister();
}

void loop() {
    leds.updateStatusBlink();

    if (!client.isWifiConnected()) {
        client.connectToWifi();
    }

    if (!client.isServerConnected()) {
        Serial.println("[TCP] Reconnecting to server...");
        leds.turnAllOff();
        delay(RECONNECT_DELAY_MS);
        client.connectAndRegister();
        return;
    }

    String command = client.readCommand();
    if (command.length() > 0) {
        leds.executeCommand(command);
    }

    delay(10);
}
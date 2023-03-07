#ifndef _HOMELAB_H_
#define _HOMELAB_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// MQTT data packet definition
struct packet {
float v1;     // measured battery 1 voltage
float v2;     // measured battery 2 voltage
bool battery_relay_state;   // battery select relay status
bool mains_relay_state;   // mains charger relay status
};

static char mqtt_payload[255];

// Function Prototypes //

void payload_update(packet p);
void setup_wifi(void);
void callback(String topic, byte* message, unsigned int length);
void reconnect(void);

#endif
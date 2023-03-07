#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/* GLOBAL CONSTANT variables */

// Put your Router credentials here
// const char* ssid     = "my_SSID"; 
// const char* password = "my_password";

const int battery_relay = D5; // changed from D6
const int mains_relay   = D7;

// Note: each espClient needs to be given a unique espClient name if there are multiple devices running in the home automation system
const char* client_name = "Client-133";  // Name of client.  Needs to be unique for each client
const char* sub_topic   = "olive2/control";
const char* pub_topic   = "olive2/status";
const char* unit_id     = "Olive2";

// IP address of the MQTT broker
// const char* mqtt_server = "192.168.3.95"; // old raspberry pi mqtt_server
const char* mqtt_server = "192.168.3.253";

/* Global Variable Declarations */

// WiFiClient espClient;
// PubSubClient client(espClient);
// DynamicJsonDocument  doc(255);
// static char mqtt_payload[255];

// MQTT data packet definition
packet pkt;

#endif
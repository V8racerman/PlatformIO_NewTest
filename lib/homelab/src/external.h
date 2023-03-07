#ifndef _EXTERNAL_H_
#define _EXTERNAL_H_

#include <homelab.h>

/* GLOBAL CONSTANT variables */

extern const char* ssid; 
extern const char* password;

extern const int battery_relay;
extern const int mains_relay;

// Note: each espClient needs to be given a unique espClient name if there are multiple devices running in the home automation system
extern const char* client_name;  // Name of client.  Needs to be unique for each client
extern const char* sub_topic;
extern const char* pub_topic;
extern const char* unit_id;

// IP address of the MQTT broker
// const char* mqtt_server = "192.168.3.95"; // old raspberry pi mqtt_server
extern const char* mqtt_server;

/* Global Variable Declarations */

extern packet pkt;
#endif
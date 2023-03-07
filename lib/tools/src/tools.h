#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <Arduino.h>

// change the VREF and DC offset values 
#define VREF 46.31 // D6, R1 = 470k, R2 = 39k
// #define VREF 22.0 // D5, R1=470k, R2 =100k
// #define VREF 4.25  // 8266 with on-board battery charger
// #define VREF 5.0  // stand-alone 8266
// #define VREF 4.25 // temporary value for ESP32U
#define RESOLUTION 1024
#define DC_OFFSET  -0.2
#define CORRECTION_FACTOR 1.016           // with WIFI disabled
// #define CORRECTION_FACTOR 1.05         // with wi-fi enabled

float read_voltage(void);
float get_voltage(int pre_delay);

#endif
#include <Arduino.h>
#include <tools.h>
#include <homelab.h>
#include <external.h>
#include <private.h>
#include <global.h>

#define SHORT_DELAY 100
#define LONG_DELAY  30000

const long measure_interval     = 60000;       // 60 seconds
const long sleep_interval       = 1200000000;  // 20 minutes
const long max_charge_period    = 2400000;     // 40 minutes
const float no_voltage          = 11.4;
const float min_voltage         = 12.4;
const float max_voltage         = 14.2;
float v = 0;

// Timers auxiliary variables
long now;
long start_charge;
long lastMeasure;

void go_to_sleep_now(String sleep_message)
{

  Serial.print(sleep_message);
  Serial.println(" ... going to sleep");
  Serial.println();
  digitalWrite(mains_relay, LOW);   // turn the power relay off
  digitalWrite(battery_relay, LOW); // turn the battery relay off (i.e. select battery 1)
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED pin OFF - signifies device is going to sleep
  ESP.deepSleep(sleep_interval);
  delay(1000);
}

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(battery_relay, OUTPUT);
  pinMode(mains_relay, OUTPUT);

  digitalWrite(battery_relay, LOW); // initially, turn the battery relay control pin OFF (i.e. select Battery 1)
  digitalWrite(mains_relay, LOW);   // initially, turn the mains relay control pin OFF
  digitalWrite(LED_BUILTIN, LOW);   // initially, turn the LED pin ON - signifies device is awake

  Serial.begin(57600);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  Serial.println(" ");
  Serial.println("Waking from sleep ...");
  pkt.v1 = get_voltage(LONG_DELAY);
  Serial.print("Battery 1 Voltage = ");
  Serial.println(pkt.v1);
  digitalWrite(battery_relay, HIGH);                    // switch to battery 2
  pkt.v2 = get_voltage(LONG_DELAY);
  Serial.print("Battery 2 Voltage = ");
  Serial.println(pkt.v2);
  payload_update(pkt);
  if ((pkt.v1 < no_voltage) || (pkt.v1 > min_voltage)) {  // Battery 1 either not connected or sufficiently charged
    if ((pkt.v2 < no_voltage) || (pkt.v2 > min_voltage)) { // Battery 2 either not connected or sufficiently charged
      go_to_sleep_now("Both batteries are either below 10.0 volts or above 12.3 volts.");
    }
  } else { digitalWrite(battery_relay, LOW ); }
  digitalWrite(mains_relay, HIGH);  // switch on mains relay
  Serial.println(" Commencing Charging sequence ...");
  lastMeasure = millis();
  start_charge = lastMeasure;
  payload_update(pkt);
}

void loop()
{
  now = millis();
  if (now - lastMeasure > measure_interval)
  {
    lastMeasure = lastMeasure + measure_interval;
    v = get_voltage(SHORT_DELAY);
    if (digitalRead(battery_relay)) { pkt.battery_relay_state = true; } else { pkt.battery_relay_state = false; }
    if (digitalRead(mains_relay))   { pkt.mains_relay_state   = true; } else { pkt.mains_relay_state   = false; }
    if (pkt.mains_relay_state) { Serial.print("Power Relay is ON, "); }
    if (!pkt.battery_relay_state) { 
      Serial.print("Battery 1");  
      pkt.v1 = v;
    } else {
      Serial.print("Battery 2");
      pkt.v2 = v;
    }
    Serial.println(" is charging.");
    Serial.print("Voltage = ");
    Serial.println(v);
    payload_update(pkt);
    if (v <  no_voltage) { go_to_sleep_now("Battery disconnected "); }
    if (v > max_voltage) { go_to_sleep_now("Maximum charge voltage reached "); }
  }
  if (now - start_charge > max_charge_period) { go_to_sleep_now("Charge period limit reached "); }
}
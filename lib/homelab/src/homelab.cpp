#include <homelab.h>
#include <external.h>

WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonDocument doc(255);

void payload_update(packet p) {

  if (digitalRead(battery_relay)) { p.battery_relay_state = true; } else { p.battery_relay_state = false; }
  if (digitalRead(mains_relay))   { p.mains_relay_state   = true; } else { p.mains_relay_state   = false; }

  doc["IP"] = WiFi.localIP();
  doc["MAC"] = WiFi.macAddress();
  doc["ID"] = unit_id;
  doc["v1"] = p.v1;
  doc["v2"] = p.v2;
  doc["r1"] = p.battery_relay_state;
  doc["r2"] = p.mains_relay_state;
  serializeJson(doc, mqtt_payload);
  Serial.println(mqtt_payload);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (!client.connected()) { reconnect(); }
  if (!client.loop()) { client.connect(client_name); }
  client.publish(pub_topic, mqtt_payload);
  delay(10000);
  WiFi.mode(WIFI_OFF);
  delay(1000);
}

// This functions connects the ESP32 to the router
void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA); // added by me
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP32 IP address: ");
  Serial.println(WiFi.localIP());
}

// This function is executed when a message arrives on a topic that the ESP8266 is subscribed to
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Add more "if" statements to control more GPIOs with MQTT
    if(topic==sub_topic) {
        if (messageTemp == "OFF") {
        Serial.println("Turning power relay off");
        digitalWrite(mains_relay, LOW);
        digitalWrite(battery_relay, LOW); 
        }
      if (messageTemp == "ONE") {
        Serial.println("Turning on Charger for Battery 1");
        digitalWrite(battery_relay, LOW); 
        digitalWrite(mains_relay, HIGH);
        }
      if (messageTemp == "TWO") {
        Serial.println("Turning on Charger for Battery 2");
        digitalWrite(battery_relay, HIGH); 
        digitalWrite(mains_relay, HIGH);
        }
        payload_update(pkt);
    }
  Serial.println();
}

// This functions reconnects the ESP8266/ESP32 to MQTT broker
// Change the function below to subscribe to more topics
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_name)) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics to control more things ...
      client.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
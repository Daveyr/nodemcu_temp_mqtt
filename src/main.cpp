#include <Arduino.h>
#include <PubSubClient.h>
#include "WiFi.h"
#include <arduino_secrets.h>

// params
#define led_pin 2
char ssid[] = secret_ssid;
char ssid_password[] = secret_ssid_password;
char mqtt_server[] = secret_mqtt_server;
char mqtt_user[] = secret_mqtt_user;
char mqtt_password[] = secret_mqtt_password;
const char* topic = "room_temperature";

// Example
// Update these with values suitable for your network.
byte mac[]    = {  0xB4, 0xE6, 0x2D, 0xFB, 0x06, 0x59 };
IPAddress ip(192, 168, 1, 32);
IPAddress server(192, 16, 1, 40);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

WiFiClient wifiClient;
PubSubClient client;

//PubSubClient client(wifiClient);
//PubSubClient client("192.168.1.40", 1883, callback, wifiClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic,"hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600); 
  WiFi.begin(ssid, ssid_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  } 
      Serial.println("Connected to the WiFi network");
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Pubsub example
    client.setClient(wifiClient);
    client.setServer("192.168.1.40",1883);
    //client.setServer(server, 1883);
    client.setCallback(callback);    
    // Allow the hardware to sort itself out
    delay(1500);
  }

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led_pin, HIGH);
delay(500);
digitalWrite(led_pin, LOW);
delay(1500);
if (!client.connected()) {
    reconnect();
  }
client.loop();
}
#include <Arduino.h>
#include <PubSubClient.h>
#include "ESP8266WiFi.h"
#include <arduino_secrets_shhm.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// params
// #define led_pin 4
#define oneWireBus 2
int sample_interval = 5000;
float temp = 0.0;
char ssid[] = secret_ssid;
char ssid_password[] = secret_ssid_password;
char mqtt_server[] = secret_mqtt_server;
char mqtt_user[] = secret_mqtt_user;
char mqtt_password[] = secret_mqtt_password;
const char* topic = "room_temperature";
// double randNumber;
char msg_out[20];

// One Wire device initialisation
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Wifi initialisation
WiFiClient wifiClient;
PubSubClient client;

// Functions

void callback(char *topic, byte *payload, int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient", mqtt_user, mqtt_password))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic, "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  // put your setup code here, to run once:

  // Random number generation for testing only
  randomSeed(analogRead(0));

  // pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
  Serial.print("Starting...");
  WiFi.begin(ssid, ssid_password);
  while (WiFi.status() != WL_CONNECTED)
  {
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
  client.setServer(secret_mqtt_server, 1883);
  client.setCallback(callback);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  // put your main code here, to run repeatedly:
  // digitalWrite(led_pin, HIGH);

  // Temperature readings
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  Serial.print("Temperature is: ");
  Serial.println(temp);
  delay(500);
  // digitalWrite(led_pin, LOW);
  if (!client.connected())
  {
   reconnect();
  }
  client.loop();
  // randNumber = random(10, 21);
  dtostrf(temp, 2, 2, msg_out);
  client.publish(topic, msg_out);
  delay(sample_interval);
}
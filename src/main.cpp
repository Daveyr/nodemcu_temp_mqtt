#include <Arduino.h>
#include <PubSubClient.h>
#include "WiFi.h"
#include <arduino_secrets.h>
// #include <arduino_secrets_adafruit.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Prototype Functions
void reconnect(PubSubClient, char *, char *);

// params
#define led_pin 2
#define oneWireBus 23
int sample_interval = 5000;
float temp = 0.0;
char ssid[] = secret_ssid;
char ssid_password[] = secret_ssid_password;
char mqtt_server[] = secret_mqtt_server;
char mqtt_user[] = secret_mqtt_user;
char mqtt_password[] = secret_mqtt_password;
const char* topic = "unearthed/feeds/room-temperature";

// char io_server[] = secret_io_server;
// char io_user[] = secret_io_user;
// char io_password[] = secret_io_password;

// double randNumber;
char msg_out[20];

// One Wire device initialisation
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Wifi initialisation
WiFiClient wifiClient;
PubSubClient pubsub_client;
// PubSubClient pubsub_io_client;

// Functions

void callback(char *topic, byte *payload, unsigned int length)
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

void reconnect(PubSubClient mqtt_client, char *username, char *password)
{
  // Loop until we're reconnected
  while (!mqtt_client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect("arduinoClient", username, password))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // pubsub_client.publish(topic, "hello world");
      // ... and resubscribe
      // pubsub_client.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
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
  // randomSeed(analogRead(0));

  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
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

  // Pubsub main client
  pubsub_client.setClient(wifiClient);
  pubsub_client.setServer(secret_mqtt_server, 1883);
  pubsub_client.setCallback(callback);

  // Adafruit IO pubsub client
  // pubsub_io_client.setClient(wifiClient);
  // pubsub_io_client.setServer(io_server, 1883);
  // pubsub_io_client.setCallback(callback);

  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(led_pin, HIGH);

  // Temperature readings
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  Serial.print("Temperature is: ");
  Serial.println(temp);
  delay(500);
  digitalWrite(led_pin, LOW);
  if (!pubsub_client.connected())
  {
   reconnect(pubsub_client, mqtt_user, mqtt_password);
  }
  pubsub_client.loop();
  
  // if (!pubsub_io_client.connected())
  // {
  //  reconnect(pubsub_io_client, io_user, io_password);
  // }
  // pubsub_io_client.loop();

  // randNumber = random(10, 21);
  dtostrf(temp, 2, 2, msg_out);
  pubsub_client.publish(topic, msg_out);
  // pubsub_io_client.publish(topic, msg_out);
  delay(sample_interval);
}
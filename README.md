# nodemcu_temp_mqtt
Monitoring temperature with a nodemcu (esp32s) and publishing to an mqtt broker.

The aim of this repo is to equip a cheap microcontroller to publish temperatures to an MQTT server and track within [Home Assistant](https://www.home-assistant.io/)

## Features
This work in progress repo has the following complete features:
* Connect to WiFi network
* Connect to MQTT server with authentication
* Random number generation
* Publish random number to an MQTT topic every 1.5 seconds.

For future features, see the _issues_ already published.

## Secrets
For security, the account details of WiFi network and MQTT server are held in a file called `arduino_secrets.h` in the `/src` folder and is untracked in the repo. An example file called `arduino_secrets_example.h` is provided for guidance. Enter your specific details into this file and rename to *arduino_secrets.h* in order to use.


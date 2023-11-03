# basics-nano-esp32-wifi
Getting Wi-fi up and running for serial data passthrough with an Arduino Nano ESP32

This example opens a socket over WiFi (port 80) and then echoes anything received on that WiFi socket to the serial interface.

## Usage
This is a PlatformIO project (contained in the "software" directory). Easiest way to use this is to open that folder in VS Code and then build and upload it to your Arduino Nano ESP32. No external electronics are required at all.

NOTE: To use this example, rename "exampleWifiCredentials.h" to "wifiCredentials.h" and update the SSID and password to those required for your wifi network.

## Remaining unanswered questions
- TODO: Work out why only complete lines are written to the serial interface. This might just be just a function of the serial monitor I'm using (Putty), or it could be something to do with the setup of the serial library.


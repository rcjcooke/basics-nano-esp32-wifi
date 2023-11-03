/*
This example opens a socket over WiFi (port 80) and then echoes anything
received on that WiFi socket to the serial interface.

NOTE: To use this example, rename "exampleWifiCredentials.h" to
"wifiCredentials.h" and update the SSID and password to those required for your
wifi network.
*/
#include <Arduino.h>
#include <WiFi.h>

// Wifi credentials recorded in this file
#include "wifiCredentials.h"

/************************
 * Constants
 ************************/
// The characters used for the "waiting" spinner on the display
static const char WAITING_CHARS[] = "-\\|/";

/************************
 * Variables
 ************************/
// Listen on socket 1000 for incoming connections
WiFiServer wifiServer(1000);

/************************
 * Utility functions
 ************************/
// Format Wi-Fi status to something human readable
String formatWiFiStatus(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD:
      return "WL_NO_SHIELD";
    case WL_IDLE_STATUS:
      return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:
      return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:
      return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:
      return "WL_CONNECTED";
    case WL_CONNECT_FAILED:
      return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:
      return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:
      return "WL_DISCONNECTED";
    default:
      return "UNKNOWN STATUS: " + status;
  }
}

void connectWiFi() {
  // Connect and let us know how that goes
  wl_status_t status;
  unsigned long prevMillis = millis();
  while (status != WL_CONNECTED) {
    // If the status has changed, let us know
    if (WiFi.status() != status) {
      status = WiFi.status();
      Serial.println();
      Serial.println("WiFi status change to: " + formatWiFiStatus(status));
      Serial.print("Connecting to wiFi");
    }
    // If it's been 500 ms since last time we checked, let us know we're still alive
    if (millis() - prevMillis >= 500) {
      Serial.print(".");
      prevMillis = millis();
    }
  }
  Serial.println();
  Serial.println("Connected. IP: " + WiFi.localIP().toString());
}

/************************
 * Entry point methods
 ************************/
void setup() {
  // Set up serial interface comms
  Serial.begin(115200);
  while(!Serial); // Wait for initialisation of the serial interface

  // Set up the Wifi connection - credentials pulled from included wifiCredentials.h (git ignored)
  Serial.println("Starting up WiFi interface");
  WiFi.begin(ssid, password);

  // Establish WiFi connection
  connectWiFi();

  // Start our server socket
  Serial.println("Opening port 1000 for new connections");
  wifiServer.begin();

}

void loop() {
  // If the WiFi connection status has changed, let us know and wait for reconnect
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  Serial.println("Waiting for client connection");
  // Note: This (intentionally) only supports a single client connection - this
  // is not an async implementation
  WiFiClient client = wifiServer.available();
  // If it's been 2 second since the last time we checked, let us know we're still alive
  while (!client) {
    static unsigned long prevMillis = millis();
    if (millis() - prevMillis >= 2000) {
      Serial.print(".");
      prevMillis = millis();
    }
    client = wifiServer.available();
  }
  Serial.println();
  Serial.println("Client connected: " + client.remoteIP().toString());
  if (client) {
    while (client.connected()) {
      // Read any data available and echo it out to the serial interface
      while (client.available() > 0) {
        char c = client.read();
        Serial.write(c);
      }
    }
 
    // Free up client resources
    client.stop();
    Serial.println("Client disconnected");
  }
}


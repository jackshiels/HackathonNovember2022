#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "arduino_secrets.h"

// Define the wifi details
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Define the server
ESP8266WebServer server(4050);

// Define the GET value
String serialSignal = "Empty";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Attempt to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Connected as: ");
  Serial.println(WiFi.localIP());

  // Set up web server
  startWebserver();
}

void loop() {
  // Checks for a serial signal and builds the webpage accordingly
  delay(50);
  server.handleClient();
}

void startWebserver() {
  // When connected and IP address obtained start HTTP server
  // Sets delegate functions for server actions of GET
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void handle_OnConnect() {
  Serial.print("Request received");
  server.send(200, "text/html", serialSignal);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
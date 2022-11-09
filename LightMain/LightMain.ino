#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "arduino_secrets.h"

// Define the wifi details
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Define the pins
int led = 2;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  WiFi.begin(ssid, password);

  // Attempt to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Connected as: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Checks every 15 seconds for presence from a web server
  delay(15000);
  http.begin("http://192.168.1.20:4050/");
  Serial.println(http.GET());
}

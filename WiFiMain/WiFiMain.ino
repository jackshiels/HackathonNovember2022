#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "arduino_secrets.h"

// Define the wifi details
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// Define the server
ESP8266WebServer server(4050);

// Define the GET value
String webMessage = "Empty";

// Define the sensor GPIO
int sensor = D1;
int val = 0;

// Define the random titles
String openTitles[5] { "PING PONG NOW LEGAL", "OPEN FOR BUSINESS", "QUICK, NOBODY'S LOOKING", "FREE AS A BIRD", "PING!" };
String closedTitles[5] { "TOO SLOW", "TRY AGAIN NEXT TIME", "DON'T BOTHER", "BALLS :(", "FUN CANCELLED" };
long randSelect = 0;

// Define the buzzer pin
int buzzer = D3;

// Dev options
bool graphMode = false;

void setup() {
  Serial.begin(115200);
  pinMode(sensor, INPUT);
  pinMode(buzzer, OUTPUT);
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
  delay(25);
  // Perform the proximity sensor operation. Turn LED on if motion detected
  val = digitalRead(sensor);
  if (val == HIGH) {
    Serial.println("On");
    webMessage = "1";
    if (graphMode) { Serial.println("Presence: 10"); }
  }
  if (val == LOW) {
    Serial.println("Off");
    webMessage = "0";
    if (graphMode) { Serial.println("Presence: 5"); }
  }
  // HTTP stuff
  server.handleClient();
}

void startWebserver() {
  // When connected and IP address obtained start HTTP server
  // Sets delegate functions for server actions of GET
  server.on("/", handle_OnConnect);
  server.on("/checkme", handle_OnConnect_Web);
  server.on("/annoy", handle_Annoyance_Web);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void handle_OnConnect() {
  Serial.print("Request received");
  server.send(200, "text/html", webMessage);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void handle_OnConnect_Web() {
  Serial.print("Request received");
  server.send(200, "text/html", buildPage());
}

void handle_Annoyance_Web() {
  Serial.print("Request received");
  ToneWarning();
  server.send(200, "text/html", annoyanceWebpage());
}

String buildPage(){
  String result = "<!DOCTYPE html><html><body><div style=\"text-align: center\">";
  result = result + randomTitle();
  result = result + "<br>";
  result = result + availabilityMessage();
  result = result + "<br>";
  result = result + gifSelector();
  result = result + annoyButton();
  result = result + "</div></body></html>";
  Serial.println(result);
  return result;
}

String randomTitle(){
  randSelect = random(0, 5);
  if (val == HIGH){
    return "<h2>" + closedTitles[randSelect] + "</h2>";
  }
  else{
    return "<h2>" + openTitles[randSelect] + "</h2>";
  }
}

String gifSelector(){
  if (val == HIGH){
    return "<img src=\"https://media0.giphy.com/media/Ol9ktCsGryGnrExZEW/giphy.gif?cid=790b761147c954582210824289e34575955f060e026cba1d&rid=giphy.gif&ct=g\" style=\"width:480px;height:480px;\">";
  }
  else{
    return "<img src=\"https://media1.giphy.com/media/ECwTCTrHPVqKI/giphy.gif?cid=ecf05e47jw1r8vfsekwn98j3nknjecq01zber3ib1uv0drco&rid=giphy.gif&ct=g\" style=\"width:480px;height:360px;\">";
  }
}

String availabilityMessage(){
  randSelect = random(0, 5);
  if (val == HIGH){
    return "<p>Room is occupied :(</p>";
  }
  else{
    return "<p>Room is free! :DDD</p>";
  }
}

String annoyButton(){
  if (val == HIGH){
    return "<br><br><br><a href=\"/annoy\"><button style=\"width: 300px; height: 75px;\"><h1>Annoy Occupants</h1></button></a>";
  }
  else{
    return "";
  }
}

void ToneWarning(){
  // Annoy people
  for (int i = 0; i < 20; i++){
    tone(buzzer, 1000);
    delay(100);
    noTone(buzzer);
    tone(buzzer, 500);
    delay(100);
    noTone(buzzer);
  }
}

String annoyanceWebpage(){
  String page = "<!DOCTYPE html><html><body><div style=\"text-align: center\">";
  page += "<h2>ANNOYANCE: INITIATED</h2><img src=\"https://media0.giphy.com/media/9zfUAVeQxzPMs/giphy.gif?cid=ecf05e475zvnep75v3hni2kwfoyrjxpgs391eqp422f7lr2x&rid=giphy.gif&ct=g\"";
  page += "style=\"width:480px;height:360px;\"></div></body></html>";
  return  page;
}
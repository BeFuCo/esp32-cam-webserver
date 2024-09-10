// code snippets taken from 
// https://raydiy.de/esp32-cam-platformio-ueberwachungskamera-mit-live-video-stream-per-webserver/
// René Aye: Thank's for your great tutorial

#include <Arduino.h>
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "gateway03";
const char* password = "65505213651803934831";

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi ...");

 WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  // Print ESP Local IP Address
  Serial.println(" connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {}
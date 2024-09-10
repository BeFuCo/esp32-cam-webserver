// code snippets taken from 
// https://raydiy.de/esp32-cam-platformio-ueberwachungskamera-mit-live-video-stream-per-webserver/
// René Aye: Thank's for your great tutorial

#include <Arduino.h>
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "gateway03";
const char* password = "65505213651803934831";

// change the hostname 
// code snippet taken from https://deepbluembedded.com/esp32-hostname-change-arduino/
// To change the ESP32 hostname, you need to call the WiFi.setHostname() function before calling 
// WiFi.mode() and then WiFi.begin() in this exact same order.
// Functions for setting the new WiFi host name shall be performed in this order
// Thanks for this great tutorial
const char* hostname = "esp32cam01";

void setup() {
  Serial.begin(9600);

  // WiFi config hostname
  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_STA);
//  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  
    
  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi ...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  // Print ESP Local IP Address
  Serial.println(" connected!");
  Serial.print("local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP32-Cam Webserver Hostname: ");
  Serial.println(WiFi.getHostname());
  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println("dBm");
}

void loop() {}
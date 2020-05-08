#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include "config.h"

void onOTAStart() {
  
}

void setup() {
  String hostname(HOSTNAME);
  WiFi.hostname(hostname);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.enableAP(false);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  
  ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.begin();
  ArduinoOTA.onStart(onOTAStart);
}

void loop() {
  ArduinoOTA.handle();
}

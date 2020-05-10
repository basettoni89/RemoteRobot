#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <RemoteDebug.h>
#include <Transmission.h>
#include "config.h"


#define DLOG(msg, ...) if(Debug.isActive(Debug.DEBUG)){Debug.printf(msg, ##__VA_ARGS__);}
#define VLOG(msg, ...) if(Debug.isActive(Debug.VERBOSE)){Debug.printf(msg, ##__VA_ARGS__);}

RemoteDebug Debug;
Transmission transmission(15, 13, 12, 14, 1024, &Debug);

void onOTAStart() {
  
}

void debugCallback() {
    String cmdString = Debug.getLastCommand();
    uint8_t len = cmdString.length();

    if(len > 0) {
      char* cmd = strdup(cmdString.c_str());
      char* token = strtok(cmd, " ");
      if(token == 0) {
        DLOG("Message invalid.\n");
        return;
      }
      int dir = atoi(token);
      token = strtok(NULL, " ");
      if(token == 0) {
        DLOG("Message invalid.\n");
        return;
      }
      int turn = atoi(token);
      DLOG("Move: %d %d\n", dir, turn);
      transmission.Move(dir, turn);
    }
}

void setup() {
  transmission.Init();

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

  Debug.begin((const char *)hostname.c_str());
  Debug.setResetCmdEnabled(true);
  Debug.setCallBackProjectCmds(debugCallback);
  Debug.setSerialEnabled(false);
}

void loop() {
  ArduinoOTA.handle();
  yield();
  Debug.handle();
}

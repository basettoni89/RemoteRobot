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
Transmission transmission(15, 13, 12, 14);

void onOTAStart() {
  
}

void debugCallback() {
    String cmdString = Debug.getLastCommand();
    uint8_t len = cmdString.length();

    if(len > 0) {
      const char* cmd = cmdString.c_str();
      DLOG("%s\n", cmd);
      
      Transmission::Direction d = Transmission::Stop;
      if(cmd[0] == 'f') {
        d = Transmission::Forward;
      }
      else if(cmd[0] == 'b'){
        d = Transmission::Back;
      } else {
        DLOG("Message invalid.\n");
        return;
      }

      Transmission::Turn t = Transmission::None;
      uint8_t i = 2;
      if(cmd[2] == 'r') {
        t = Transmission::Right;
        i = 4;
      } else if(cmd[2] == 'l') {
        t = Transmission::Left;
        i = 4;
      }

      uint8_t p = 0;
      for(;i < len;i++) {
        uint8_t c = cmd[i] - '0';
        if(c >= 0 && c < 10) {
          p += c * pow(10, (len - i - 1));
        } else {
          DLOG("Message invalid.\n");
        }
      }
      
      DLOG("Moving: %d %d %d\n", d, t, p);
      transmission.Move(d, t, p);
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

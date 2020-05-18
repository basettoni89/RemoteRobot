#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <RemoteDebug.h>
#include <WebSocketsServer.h>
#include <Transmission.h>
#include "config.h"

#if USE_REMOTE_DEBUG
#define DLOG(msg, ...) if(Debug.isActive(Debug.DEBUG)){Debug.printf(msg, ##__VA_ARGS__);}
#define VLOG(msg, ...) if(Debug.isActive(Debug.VERBOSE)){Debug.printf(msg, ##__VA_ARGS__);}
#else
#define DLOG(msg, ...) Serial.printf(msg, ##__VA_ARGS__);
#endif

RemoteDebug Debug;
WebSocketsServer webSocket(5123);
Transmission transmission(15, 13, 12, 14, 1024);//, &Debug);

void onOTAStart() {
  
}

void handleCommand(const char* command)
{
    char* cmd = strdup(command);
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

void debugCallback() {
    String cmdString = Debug.getLastCommand();
    handleCommand(cmdString.c_str());
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
          DLOG("[%u] Disconnected!\n", num);
          break;
        case WStype_CONNECTED:
          {
            IPAddress ip = webSocket.remoteIP(num);
            DLOG("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            // send message to client
            webSocket.sendTXT(num, "Connected");
          }
          break;
        case WStype_TEXT:
          DLOG("[%u] get Text: %s\n", num, payload);
          handleCommand((char*) payload);
          // send message to client
          // webSocket.sendTXT(num, "message here");

          // send data to all connected clients
          // webSocket.broadcastTXT("message here");
          break;
        case WStype_BIN:
          DLOG("[%u] get binary length: %u\n", num, length);
          hexdump(payload, length);

          // send message to client
          // webSocket.sendBIN(num, payload, length);
          break;
        default:
          break;
    }
}

void setup() {
  transmission.Init();

  Serial.begin(115200);

  String hostname(HOSTNAME);
  WiFi.hostname(hostname);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.enableAP(false);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
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

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

  DLOG("Done startup.\n");
}

void loop() {
  ArduinoOTA.handle();
  yield();
  Debug.handle();
  webSocket.loop();
}

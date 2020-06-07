#ifndef GARAGE_WEBSERVER
#define GARAGE_WEBSERVER

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "Constants.h"

ESP8266WebServer server(80);

bool otaEnabled = false;
const char* otaIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

void handleRoot() {
  if (otaEnabled) {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", otaIndex);
  } else {
    server.send(200, "text/plain", "hello from esp8266!");
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void setupWebServer () {
  server.on("/", handleRoot);

  server.on("/trigger", []() {
    // momentary activation
    digitalWrite(RELAY, RELAY_ON);
    digitalWrite(WIFI_LED, LED_ON);
    delay(100);
    // deactivate
    digitalWrite(RELAY, RELAY_OFF);
    digitalWrite(WIFI_LED, LED_OFF);

    server.send(200, "application/json", "{\"message\": \"ok\", \"success\": true}");
  });

  server.onNotFound(handleNotFound);

  server.begin();
}

void setupOta () {
  otaEnabled = true;

  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.setDebugOutput(true);
      Serial.printf("Update: %s\n", upload.filename.c_str());
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    }
    yield();
  });
}

bool loopWebServer () {
  server.handleClient();
  return true;
}

#endif
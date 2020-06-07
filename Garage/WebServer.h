#ifndef GARAGE_WEBSERVER
#define GARAGE_WEBSERVER

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "Constants.h"

bool otaEnabled = false;
const char* otaIndex = "<form method='POST' action='/ota-post' enctype='multipart/form-data'>Send .bin file: <input type='file' name='update'><input type='submit' value='Update'></form>";

void handleOtaForm() {
  if (otaEnabled) {
    wm.server->sendHeader("Connection", "close");
    wm.server->send(200, "text/html", otaIndex);
  } else {
    wm.server->send(400, "text/plain", "OTA is disabled");
  }
}

void setupWebServer () {
  wm.server->on("/ota", handleOtaForm);

  wm.server->on("/trigger", []() { // hit this route to cause a momentary activation of the relay
    // momentary activation
    digitalWrite(RELAY, RELAY_ON);
    digitalWrite(TRIGGER_LED, LED_ON);
    delay(100);
    // deactivate
    digitalWrite(RELAY, RELAY_OFF);
    digitalWrite(TRIGGER_LED, LED_OFF);

    wm.server->send(200, "application/json", "{\"message\": \"ok\", \"success\": true}");
  });
}

void setupOta () {
  otaEnabled = true;

  wm.server->on("/ota-post", HTTP_POST, []() {
    wm.server->sendHeader("Connection", "close");
    wm.server->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = wm.server->upload();
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

#endif
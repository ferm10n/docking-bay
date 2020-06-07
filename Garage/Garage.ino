#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "WifiStuff.h"
#include "Constants.h"
#include "WebServer.h"
#include "LedStuff.h"

const char* ssid = STASSID;
const char* password = STAPSK;
bool disconnectTrigger = false;
wl_status_t previousWifiStatus = WL_CONNECTED; // initially set to be connected to the first real disconnect status triggers a blink


void setup(void) {
  // setup outputs and initial state
  pinMode(RELAY, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);
  pinMode(TRIGGER_LED, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(RELAY, RELAY_OFF);
  digitalWrite(5, RELAY_OFF);
  digitalWrite(WIFI_LED, LED_OFF);
  digitalWrite(TRIGGER_LED, LED_OFF);

  // allow serial connections
  Serial.begin(74880);
  blinkTriggerLed(3000); // pause to allow serial connection

  // the wifi manager (wm) creates a new server instance every time it needs to perform a new connection.
  // instead of trying to worry about when it does this and making sure we attach our routes correctly,
  // we can simply add a callback to when a new instance is created!
  wm.setWebServerCallback([]() {
    setupWebServer();
    setupOta();
  });

  setupWifi();
}


void loop(void) {
  wl_status_t wifiStatus = WiFi.status();
  if (wifiStatus != WL_CONNECTED) {
    if (previousWifiStatus === WL_CONNECTED) { // a disconnect just happened
      blinkWifiLed(1000); // long pause to show portal is being activated
      blinkingWifiStart(); // enable intermittent blinks to show portal is active
      wm.autoConnect(APSSID, APPSK); // try to let a connection be configured
    }
  } else if (previousWifiStatus !== WL_CONNECTED) { // did a connection just happen?
    // connected to AP!
    blinkingWifiStop(); // stop the intermittent blinking

    // successful blink pattern
    blinkWifiLed(50);
    delay(200);
    blinkWifiLed(50);
    delay(200);
    blinkWifiLed(50);
    delay(200);

    // allow configuration to happen even after connection is made
    wm.startWebPortal();
  }
  previousWifiStatus = wifiStatus;
  wm.process(); // wm must always be running since its server is used for everything
}

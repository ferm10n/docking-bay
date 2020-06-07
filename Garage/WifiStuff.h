#ifndef GARAGE_WIFI_STUFF
#define GARAGE_WIFI_STUFF

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "Constants.h"
#include "LedStuff.h"

WiFiManager wm;
bool portalRunning = false;

void activatePortal () {
  blinkWifiLed(1000);
  blinkingWifiStart();
  portalRunning = true;
  wm.startConfigPortal(APSSID, APPSK);
}

bool portalDeactivated = false;
void deactivatePortal () {
  blinkingWifiStop();
  portalDeactivated = true;
  portalRunning = false;

  // cannot stop it if its not running.. which it isn't after a successful connection
  // wm.stopConfigPortal();
}

void setupWifi () {
  WiFi.mode(WIFI_STA);
  
  // wm.resetSettings();

  wm.setEnableConfigPortal(true); // ?
  wm.setConfigPortalBlocking(false); // ?

  wm.setSaveConfigCallback(deactivatePortal);
  
  Serial.println("");
}

bool loopWifiManager () {
  if (WiFi.status() != WL_CONNECTED) {
    if (portalRunning) {
      wm.process();
    } else {
      activatePortal();
    }
  }

  // watchdog will bite me if I do this in deactivatePortal()
  if (portalDeactivated) {
    portalDeactivated = false;
    blinkWifiLed(50);
    delay(200);
    blinkWifiLed(50);
    delay(200);
    blinkWifiLed(50);
    delay(200);
  }
  return portalRunning;
}

#endif
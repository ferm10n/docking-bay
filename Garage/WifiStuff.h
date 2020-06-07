#ifndef GARAGE_WIFI_STUFF
#define GARAGE_WIFI_STUFF

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "Constants.h"
#include "LedStuff.h"

WiFiManager wm;
bool autoConnecting = false;

void activatePortal () {
  
  wm.startConfigPortal(APSSID, APPSK);
}

bool portalDeactivated = false;
// void deactivatePortal () {
//   blinkingWifiStop();
//   portalDeactivated = true;
//   portalRunning = false;

//   // cannot stop it if its not running.. which it isn't after a successful connection
//   // wm.stopConfigPortal();
// }

void setupWifi () {
  // by default, wm will try to start as an AP. This will make it try to connect to a saved wifi network (if one exists)
  WiFi.mode(WIFI_STA);

  // normally wifi config is loaded from flash. resetSettings would wipe it, for dev purposes
  // wm.resetSettings();

  // normally wm will run once during setup and then stop once connected to wifi.
  // we want to use the wm to also serve our custom pages since it already binds to port 80.
  // to do this, we need to keep wm running, and non-blocking.
  // wm process will need to be called in loop() to handle requests.
  wm.setConfigPortalBlocking(false);

  wm.startWebPortal(); // this creates the server instance
}

// bool loopWifiManager () {
  

//   // watchdog will bite me if I do this in deactivatePortal()
//   if (portalDeactivated) {
//     portalDeactivated = false;
//     blinkWifiLed(50);
//     delay(200);
//     blinkWifiLed(50);
//     delay(200);
//     blinkWifiLed(50);
//     delay(200);
//   }
//   return portalRunning;
// }

#endif
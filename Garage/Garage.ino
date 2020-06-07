#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "Constants.h"
#include "WebServer.h"
#include "WifiStuff.h"
#include "LedStuff.h"

const char* ssid = STASSID;
const char* password = STAPSK;


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
  blinkTriggerLed(3000); // pause to allow connection

  setupWifi();
  // // setup wifi 
  // Serial.printf("Trying to connect to %s (%s)", ssid, password);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // Serial.println("");

  // // Wait for connection
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(450);
  //   Serial.print(WIFI_LED);
  //   Serial.print(".");

  //   digitalWrite(WIFI_LED, LED_ON);
  //   delay(50);
  //   digitalWrite(WIFI_LED, LED_OFF);
  // }
  // Serial.println("");
  // digitalWrite(1, LED_ON);
  // Serial.print("Connected!");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  // delay(1000);
  // digitalWrite(WIFI_LED, LED_OFF);

  // setupWebServer();
  // setupOta();
  // Serial.println("HTTP server started");
}


void loop(void) {
  loopWifiManager() || loopWebServer();
  yield();
}

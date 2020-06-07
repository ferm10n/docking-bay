#ifndef GARAGE_LED_STUFF
#define GARAGE_LED_STUFF

#include <Ticker.h>

#include "Constants.h"

void blinkWifiLed (unsigned long timeOn) {
    digitalWrite(WIFI_LED, LED_ON);
    delay(timeOn);
    digitalWrite(WIFI_LED, LED_OFF);
}

void blinkTriggerLed (unsigned long timeOn) {
    digitalWrite(TRIGGER_LED, LED_ON);
    delay(timeOn);
    digitalWrite(TRIGGER_LED, LED_OFF);
}

Ticker wifiBlinker;
void wifiBlinkerFn () {
    if (millis() % 3000 < 50) {
        digitalWrite(WIFI_LED, LED_ON);
    } else {
        digitalWrite(WIFI_LED, LED_OFF);
    }
}
void blinkingWifiStart () {
    wifiBlinker.attach(.05, wifiBlinkerFn); // run fn every 50ms
}
void blinkingWifiStop () {
    wifiBlinker.detach();
}

#endif
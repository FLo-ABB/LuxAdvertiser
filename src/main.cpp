#include <Arduino.h>
#include <ArduinoBLE.h>
#include <Wire.h>
#include "ble_manager.h"
#include "led_manager.h"
#include "debug.h"
#include "startup_sequence.h"

const bool DEBUG_ENABLED = false;

static const unsigned long RETRY_INTERVAL_MS = 10000;
static bool sensorReady = false;
static bool bleReady = false;
static unsigned long lastRetryMs = 0;

void setup() {
  debugInit();
  DBG_PRINTLN("[setup] boot");

  DBG_PRINTLN("[setup] runDebugBootPulse()");
  runDebugBootPulse();

  DBG_PRINTLN("[setup] initLedManager()");
  initLedManager();
  delay(100);
  
  DBG_PRINTLN("[setup] Wire.begin()");
  Wire.begin();
  delay(100);
  
  DBG_PRINTLN("[setup] tryBringup()");
  tryBringup(bleReady, sensorReady);
  lastRetryMs = millis();
  DBG_PRINTLN("[setup] complete");
}

void loop() {
  if (bleReady && sensorReady) {
    advertiseBTHome();
  }

  if ((!sensorReady || !bleReady) && millis() - lastRetryMs >= RETRY_INTERVAL_MS) {
    DBG_PRINTLN("[loop] retry bringup");
    tryBringup(bleReady, sensorReady);
    lastRetryMs = millis();
  }
}

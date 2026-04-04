#include "startup_sequence.h"

#include "ble_manager.h"
#include "lux_sensor.h"
#include "led_manager.h"
#include "debug.h"

void runDebugBootPulse() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(80);
  digitalWrite(LED_BUILTIN, LOW);
  delay(80);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(80);
  digitalWrite(LED_BUILTIN, LOW);
  delay(80);
}

void tryBringup(bool& bleReady, bool& sensorReady) {
  DBG_PRINTLN("[bringup] begin");

  if (!bleReady) {
    DBG_PRINTLN("[bringup] setupBLE() start");
    bleReady = setupBLE();
    if (bleReady) {
      DBG_PRINTLN("[bringup] setupBLE() OK");
      setLedStatus(LED_STATUS_BLE_OK);
    } else {
      DBG_PRINTLN("[bringup] setupBLE() FAILED");
      setLedStatus(LED_STATUS_CRITICAL);
      return;
    }
  }

  if (!sensorReady) {
    DBG_PRINTLN("[bringup] initLuxSensor() start");
    sensorReady = initLuxSensor();
    if (sensorReady) {
      DBG_PRINTLN("[bringup] initLuxSensor() OK");
      setLedStatus(LED_STATUS_SENSOR_OK);
    } else {
      DBG_PRINTLN("[bringup] initLuxSensor() FAILED");
      setLedStatus(LED_STATUS_CRITICAL);
    }
  }

  if (sensorReady && bleReady) {
    DBG_PRINTLN("[bringup] all systems ready");
    setLedStatus(LED_STATUS_OFF);
  }

  DBG_PRINT("[bringup] state bleReady=");
  DBG_PRINT(bleReady ? "1" : "0");
  DBG_PRINT(" sensorReady=");
  DBG_PRINTLN(sensorReady ? "1" : "0");
}

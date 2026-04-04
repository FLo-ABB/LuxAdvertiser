#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>

enum LedStatus {
  LED_STATUS_OFF,
  LED_STATUS_SENSOR_OK,
  LED_STATUS_BLE_OK,
  LED_STATUS_CRITICAL
};

void initLedManager();
void setLedStatus(LedStatus status);

#endif // LED_MANAGER_H

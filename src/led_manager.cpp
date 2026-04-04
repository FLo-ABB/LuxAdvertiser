#include "led_manager.h"

// Prefer explicit RGB board macros when available.
#if !defined(LED_RED) && defined(LEDR)
#define LED_RED LEDR
#endif

#if !defined(LED_GREEN) && defined(LEDG)
#define LED_GREEN LEDG
#endif

#if !defined(LED_BLUE) && defined(LEDB)
#define LED_BLUE LEDB
#endif

// Fallback to built-in single LED.
#ifndef LED_RED
#define LED_RED LED_BUILTIN
#endif

#ifndef LED_GREEN
#define LED_GREEN LED_BUILTIN
#endif

#ifndef LED_BLUE
#define LED_BLUE LED_BUILTIN
#endif

// XIAO RGB LED is active-low: LOW = ON, HIGH = OFF.
static const uint8_t LED_ON_LEVEL = LOW;
static const uint8_t LED_OFF_LEVEL = HIGH;

static void allLedsOff() {
  digitalWrite(LED_RED, LED_OFF_LEVEL);
  digitalWrite(LED_GREEN, LED_OFF_LEVEL);
  digitalWrite(LED_BLUE, LED_OFF_LEVEL);
}

void initLedManager() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  allLedsOff();
}

void setLedStatus(LedStatus status) {
  switch (status) {
    case LED_STATUS_SENSOR_OK:
      allLedsOff();
      digitalWrite(LED_GREEN, LED_ON_LEVEL);
      delay(100);
      digitalWrite(LED_GREEN, LED_OFF_LEVEL);
      break;
    case LED_STATUS_BLE_OK:
      allLedsOff();
      digitalWrite(LED_BLUE, LED_ON_LEVEL);
      delay(100);
      digitalWrite(LED_BLUE, LED_OFF_LEVEL);
      break;
    case LED_STATUS_CRITICAL:
      allLedsOff();
      digitalWrite(LED_RED, LED_ON_LEVEL);
      delay(100);
      digitalWrite(LED_RED, LED_OFF_LEVEL);
      break;
    case LED_STATUS_OFF:
    default:
      allLedsOff();
      break;
  }
}

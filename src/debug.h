#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

extern const bool DEBUG_ENABLED;

inline void debugInit(unsigned long baud = 115200, unsigned long startupWaitMs = 2000) {
  if (DEBUG_ENABLED) {
    Serial.begin(baud);
    delay(startupWaitMs);
  }
}

#define DBG_PRINT(x) do { if (DEBUG_ENABLED) { Serial.print(x); } } while (0)
#define DBG_PRINTLN(x) do { if (DEBUG_ENABLED) { Serial.println(x); } } while (0)

#endif // DEBUG_H

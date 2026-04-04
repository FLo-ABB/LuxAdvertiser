#include "battery.h"
#include "debug.h"

// Battery ADC pin (P0.14, pin 14 on Seeed Xiao nRF52840)
#define BATTERY_PIN 14

// Function to get the current battery level
uint8_t getBatteryLevel() {
  DBG_PRINTLN("[battery] analogRead() start");
  // Read ADC value
  int raw = analogRead(BATTERY_PIN);
  DBG_PRINT("[battery] analogRead() raw=");
  if (DEBUG_ENABLED) {
    Serial.println(raw);
  }
  
  // Convert to voltage (assuming voltage divider 1:1, so Vadc = Vbat / 2)
  float vbat = (raw * 3.3 / 1024.0) * 2.0;
  
  // Calculate percentage (LiPo: 3.0V = 0%, 4.2V = 100%)
  float percent = (vbat - 3.0) / (4.2 - 3.0) * 100.0;
  
  // Constrain to 0-100%
  uint8_t result = constrain(percent, 0, 100);
  DBG_PRINT("[battery] level=");
  if (DEBUG_ENABLED) {
    Serial.println(result);
  }
  return result;
}
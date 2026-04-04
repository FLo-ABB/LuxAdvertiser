#include "battery.h"
#include "debug.h"

#if defined(PIN_VBAT)
#define BATTERY_READ_PIN PIN_VBAT
#else
#define BATTERY_READ_PIN A0
#endif

// XIAO nRF52840: VBUS is available as pin 32 (high when USB/charging cable connected)
// We use this to detect active charging and avoid ADC hangs (wiki FAQ Q3)
#define VBUS_DETECT_PIN 32

static uint8_t lastBatteryLevel = 100;

// Function to get the current battery level
uint8_t getBatteryLevel() {
  // VBUS detection: If USB/charging cable is connected, VBUS will be HIGH
  // This prevents ADC hangs during charging (wiki FAQ Q3)
  static bool vbusPinInitialized = false;
  if (!vbusPinInitialized) {
    pinMode(VBUS_DETECT_PIN, INPUT);
    vbusPinInitialized = true;
  }
  
  bool usbPowered = digitalRead(VBUS_DETECT_PIN) == HIGH;
  DBG_PRINT("[battery] VBUS=");
  if (DEBUG_ENABLED) {
    Serial.println(usbPowered);
  }
  
  // Wiki FAQ Q3: Avoid reading battery ADC when USB powered/charging
  // to prevent P0.31 voltage issues and ADC hangs
  if (usbPowered) {
    DBG_PRINTLN("[battery] skipping read - USB powered");
    return lastBatteryLevel;
  }

  DBG_PRINTLN("[battery] analogRead() start");
  // Read ADC value from P0.14 (ADC_BAT)
  unsigned long startMs = millis();
  int raw = analogRead(BATTERY_READ_PIN);
  unsigned long elapsed = millis() - startMs;
  
  DBG_PRINT("[battery] analogRead() raw=");
  if (DEBUG_ENABLED) {
    Serial.print(raw);
    Serial.print(" elapsed=");
    Serial.println(elapsed);
  }
  
  // Convert to voltage (assuming voltage divider 1:1, so Vadc = Vbat / 2)
  float vbat = (raw * 3.3 / 1024.0) * 2.0;
  
  // Calculate percentage (LiPo: 3.0V = 0%, 4.2V = 100%)
  float percent = (vbat - 3.0) / (4.2 - 3.0) * 100.0;
  
  // Constrain to 0-100%
  uint8_t result = constrain(percent, 0, 100);
  lastBatteryLevel = result;
  DBG_PRINT("[battery] level=");
  if (DEBUG_ENABLED) {
    Serial.println(result);
  }
  return result;
}
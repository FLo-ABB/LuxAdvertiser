#include "ble_manager.h"
#include <ArduinoBLE.h>
#include "lux_sensor.h"
#include "battery.h"
#include "debug.h"

// BTHome service UUID
const char* BTHOME_UUID = "FCD2";

// Advertising data buffer
uint8_t adv_data[20];
size_t adv_data_len = 0;

unsigned long lastAdvertise = 0;
bool bleInitialized = false;

static void debugPrintHex(const char* label, const uint8_t* data, size_t len) {
  if (!DEBUG_ENABLED) return;
  
  DBG_PRINT(label);
  for (size_t i = 0; i < len; i++) {
    Serial.print(" ");
    if (data[i] < 0x10) Serial.print("0");
    Serial.print(data[i], HEX);
  }
  Serial.println();
}

static void buildPlaceholderBTHomeData() {
  // Safe placeholder used only during BLE setup to avoid ADC/I2C side effects.
  adv_data[0] = 0xD2;
  adv_data[1] = 0xFC;
  adv_data[2] = 0x40;
  adv_data[3] = 0x01;
  adv_data[4] = 0;       // battery = 0%
  adv_data[5] = 0x16;
  adv_data[6] = 0;       // charging = 0 (not charging)
  adv_data[7] = 0x05;
  adv_data[8] = 0;       // lux high
  adv_data[9] = 0;       // lux mid
  adv_data[10] = 0;      // lux low
  adv_data_len = 11;
}

void buildBTHomeData() {
  DBG_PRINTLN("[ble] buildBTHomeData: getLuxValue()");
  uint16_t lux = getLuxValue();

  DBG_PRINTLN("[ble] buildBTHomeData: getBatteryLevel()");
  uint8_t battery = getBatteryLevel();

  DBG_PRINTLN("[ble] buildBTHomeData: pack payload");

  uint32_t luxScaled = (uint32_t)lux * 100U;

  // Check if device is USB powered (charging)
  static bool vbusPinInitialized = false;
  if (!vbusPinInitialized) {
    pinMode(32, INPUT);  // VBUS pin
    vbusPinInitialized = true;
  }
  uint8_t isCharging = digitalRead(32) == HIGH ? 1 : 0;

  // BTHome v2 payload format:
  // [UUID: 0xFCD2] [Info: 0x40] [0x01: battery %] [0x16: charging] [0x05: lux]
  adv_data[0] = 0xD2;          // UUID low byte
  adv_data[1] = 0xFC;          // UUID high byte
  adv_data[2] = 0x40;          // Device info (unencrypted, regular, v2)
  adv_data[3] = 0x01;          // Object ID: Battery level
  adv_data[4] = battery;        // Battery percentage
  adv_data[5] = 0x16;          // Object ID: Battery charging status
  adv_data[6] = isCharging;    // 0=not charging, 1=charging
  adv_data[7] = 0x05;          // Object ID: Illuminance
  adv_data[8] = (uint8_t)(luxScaled & 0xFF);
  adv_data[9] = (uint8_t)((luxScaled >> 8) & 0xFF);
  adv_data[10] = (uint8_t)((luxScaled >> 16) & 0xFF);

  adv_data_len = 11;
  
  if (DEBUG_ENABLED) {
    DBG_PRINT("[ble] lux=");
    Serial.print(lux);
    Serial.print(" luxScaled=");
    Serial.print(luxScaled);
    Serial.print(" battery=");
    Serial.print(battery);
    Serial.print(" charging=");
    Serial.println(isCharging);
    debugPrintHex("[ble] payload:", adv_data, adv_data_len);
  }
  
  DBG_PRINTLN("[ble] buildBTHomeData: done");
}

bool setupBLE() {
  DBG_PRINTLN("[ble] BLE.begin()...");
  if (!BLE.begin()) {
    DBG_PRINTLN("[ble] BLE.begin() failed");
    return false;
  }
  DBG_PRINTLN("[ble] BLE.begin() OK");

  DBG_PRINTLN("[ble] BLE.setLocalName()");
  BLE.setLocalName("LuxAdvertiser");

  DBG_PRINTLN("[ble] BLE.setAdvertisedService()");
  BLEService bthomeService(BTHOME_UUID);
  BLE.setAdvertisedService(bthomeService);

  DBG_PRINTLN("[ble] buildPlaceholderBTHomeData()");
  buildPlaceholderBTHomeData();

  DBG_PRINTLN("[ble] BLE.setAdvertisedServiceData()");
  BLE.setAdvertisedServiceData(0xFCD2, adv_data + 2, adv_data_len - 2);

  bleInitialized = true;
  DBG_PRINTLN("[ble] setup complete");
  return true;
}

void advertiseBTHome() {
  if (!bleInitialized) {
    return;
  }

  if (lastAdvertise == 0 || millis() - lastAdvertise > 30000) {
    buildBTHomeData();
    BLE.setAdvertisedServiceData(0xFCD2, adv_data + 2, adv_data_len - 2);

    DBG_PRINTLN("[ble] BLE.advertise() sending...");
    BLE.advertise();
    delay(500);
    BLE.stopAdvertise();
    
    if (DEBUG_ENABLED) {
      unsigned long now = millis();
      DBG_PRINT("[ble] advertise complete at ");
      Serial.println(now);
    }

    lastAdvertise = millis();
  }
}
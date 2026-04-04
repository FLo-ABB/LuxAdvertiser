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

static void buildPlaceholderBTHomeData() {
  // Safe placeholder used only during BLE setup to avoid ADC/I2C side effects.
  adv_data[0] = 0xD2;
  adv_data[1] = 0xFC;
  adv_data[2] = 0x40;
  adv_data[3] = 0x01;
  adv_data[4] = 0;
  adv_data[5] = 0x05;
  adv_data[6] = 0;
  adv_data[7] = 0;
  adv_data[8] = 0;
  adv_data_len = 9;
}

void buildBTHomeData() {
  DBG_PRINTLN("[ble] buildBTHomeData: getLuxValue()");
  uint16_t lux = getLuxValue();

  DBG_PRINTLN("[ble] buildBTHomeData: getBatteryLevel()");
  uint8_t battery = getBatteryLevel();

  DBG_PRINTLN("[ble] buildBTHomeData: pack payload");

  uint32_t luxScaled = (uint32_t)lux * 100U;

  adv_data[0] = 0xD2;
  adv_data[1] = 0xFC;
  adv_data[2] = 0x40;
  adv_data[3] = 0x01;
  adv_data[4] = battery;
  adv_data[5] = 0x05;
  adv_data[6] = (uint8_t)(luxScaled & 0xFF);
  adv_data[7] = (uint8_t)((luxScaled >> 8) & 0xFF);
  adv_data[8] = (uint8_t)((luxScaled >> 16) & 0xFF);

  adv_data_len = 9;
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

    BLE.advertise();
    delay(500);
    BLE.stopAdvertise();

    lastAdvertise = millis();
  }
}
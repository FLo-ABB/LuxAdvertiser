#include "lux_sensor.h"
#include <Adafruit_VEML7700.h>
#include <Wire.h>
#include "debug.h"

Adafruit_VEML7700 veml;
static bool sensorInitialized = false;

static bool i2cDevicePresent(uint8_t addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission() == 0;
}

static void logI2cScan() {
  DBG_PRINTLN("[sensor] I2C scan start");
  bool foundAny = false;
  for (uint8_t addr = 1; addr < 127; ++addr) {
    if (i2cDevicePresent(addr)) {
      foundAny = true;
      if (DEBUG_ENABLED) {
        Serial.print("[sensor] I2C device found at 0x");
        if (addr < 16) {
          Serial.print('0');
        }
        Serial.println(addr, HEX);
      }
    }
  }
  if (!foundAny) {
    DBG_PRINTLN("[sensor] I2C scan found no devices");
  }
  DBG_PRINTLN("[sensor] I2C scan end");
}

// Function to initialize the lux sensor
bool initLuxSensor() {
  DBG_PRINTLN("[sensor] probe address 0x10");
  if (!i2cDevicePresent(0x10)) {
    DBG_PRINTLN("[sensor] no ACK at 0x10");
    logI2cScan();
    sensorInitialized = false;
    return false;
  }

  DBG_PRINTLN("[sensor] ACK at 0x10");
  DBG_PRINTLN("[sensor] veml.begin()...");
  if (!veml.begin()) {
    DBG_PRINTLN("[sensor] veml.begin() failed");
    logI2cScan();
    sensorInitialized = false;
    return false;
  }

  DBG_PRINTLN("[sensor] configure gain/integration");
  // Configure sensor if needed
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_100MS);
  sensorInitialized = true;
  DBG_PRINTLN("[sensor] init complete");
  return true;
}

// Function to get the current lux value
uint16_t getLuxValue() {
  if (!sensorInitialized) {
    return 0;
  }
  return veml.readLux();
}
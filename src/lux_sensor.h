#ifndef LUX_SENSOR_H
#define LUX_SENSOR_H

#include <Arduino.h>

// Function to initialize the lux sensor
bool initLuxSensor();

// Function to get the current lux value
uint16_t getLuxValue();

#endif // LUX_SENSOR_H
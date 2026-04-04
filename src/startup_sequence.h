#ifndef STARTUP_SEQUENCE_H
#define STARTUP_SEQUENCE_H

#include <Arduino.h>

void runDebugBootPulse();
void tryBringup(bool& bleReady, bool& sensorReady);

#endif // STARTUP_SEQUENCE_H

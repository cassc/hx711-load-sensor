#ifndef __E32_LOAD_SCALE__
#define __E32_LOAD_SCALE__
#include <Arduino.h>
#include <HX711.h>


#include <EEPROM.h>

#define LOADCELL_DOUT_PIN 23
#define LOADCELL_SCK_PIN 19

void setupScale();

float getWeight();

void tare();

float readCalibrationFactor();
void writeCalibrationFactor(float val, bool persist = false);

#endif

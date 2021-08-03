#include "scale.h"

HX711 scale;
float calibration_factor = -96650;

float readCalibrationFactor()
{
    if (EEPROM.readBool(0))
    {
        return EEPROM.readFloat(3);
    }
    Serial.println("Scale factor not configured, using defeault!");
    return -96650;
}

void writeCalibrationFactor(float val, bool persist)
{
    calibration_factor = val;
    Serial.printf("Changing calibration factor to %.2f\r\n", val);
    scale.set_scale(calibration_factor);
    if (!persist)
    {
        return;
    }

    if (!EEPROM.readBool(0))
    {
        EEPROM.writeBool(0, true);
    }

    EEPROM.writeFloat(3, val);
}

void setupScale()
{
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    calibration_factor = readCalibrationFactor();
    scale.set_scale(calibration_factor);
}

float getWeight()
{
    return scale.get_units();
}

void tare(){
    scale.tare();
}
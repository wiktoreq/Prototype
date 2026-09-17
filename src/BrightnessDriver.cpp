#include "BrightnessDriver.h"

static uint8_t lastLightBrightness = 0xFF;

void BrightnessDriver::configure()
{
    lastLightBrightness = 0xFF;
}

void BrightnessDriver::update(uint8_t lightBrightness)
{
    if (lightBrightness == lastLightBrightness) {
        return;
    }

    lastLightBrightness = lightBrightness;
}

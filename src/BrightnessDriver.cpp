#include "BrightnessDriver.h"

static uint8_t lastScreenBrightness = 0xFF;
static uint8_t lastLightBrightness = 0xFF;

void BrightnessDriver::configure()
{
    lastScreenBrightness = 0xFF;
    lastLightBrightness = 0xFF;
}

void BrightnessDriver::update(uint8_t screenBrightness, uint8_t lightBrightness)
{
    if (screenBrightness == lastScreenBrightness &&
        lightBrightness == lastLightBrightness) {
        return;
    }

    lastScreenBrightness = screenBrightness;
    lastLightBrightness = lightBrightness;

    // TODO: map 0-100 onto backlight PWM and the external-light output
    // once those pins are assigned in Pins.h.
}

#pragma once
#include <stdint.h>

namespace BrightnessDriver
{
    // Resets last-applied tracking. Call once from setup.
    void configure();

    // Applies screen (backlight) and external-light brightness (0-100).
    // Pins are not assigned yet; this is a no-op until they are.
    void update(uint8_t screenBrightness, uint8_t lightBrightness);
}

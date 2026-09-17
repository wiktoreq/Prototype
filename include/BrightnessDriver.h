#pragma once
#include <stdint.h>

namespace BrightnessDriver
{
    // Resets last-applied tracking. Call once from setup.
    void configure();

    // Tracks external-light brightness (0-100).
    // Hardware control remains with the POJ 3 inputs.
    void update(uint8_t lightBrightness);
}

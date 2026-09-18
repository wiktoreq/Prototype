#pragma once

#include <stdint.h>

// Slider values used to initialize hardware after the display is configured.
struct UiControlState
{
    uint8_t lightBrightness;
    uint8_t heightSpeed;
    uint8_t positionSpeed;
};

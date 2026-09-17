#pragma once
#include <stdint.h>

// Snapshot of on-screen knobs and hold-buttons. DisplayApp fills this;
// HardwareController consumes it. No pin or PWM types belong here.
struct UiControlState {
    enum Screen : uint8_t {
        Main = 0,
        Height = 1,
        Position = 2
    };

    Screen screen;

    // Main-menu external-light knob, 0-100.
    uint8_t lightBrightness;

    // Height-tab speed knob (0-100) and CONTRACT/RETRACT hold-buttons.
    uint8_t heightSpeed;
    bool heightContracting;
    bool heightRetracting;

    // Position-tab speed knob (0-100) and CONTRACT/RETRACT hold-buttons.
    uint8_t positionSpeed;
    bool positionContracting;
    bool positionRetracting;
};

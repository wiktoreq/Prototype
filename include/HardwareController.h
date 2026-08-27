#pragma once
#include <stdint.h>
#include "UiControlState.h"

namespace HardwareController
{
    // Applies initial brightness from the UI snapshot. Call after DisplayApp::setup().
    void configure(const UiControlState& ui);

    // Maps current knob/button state onto PwmDriver and BrightnessDriver.
    // Call every loop after DisplayApp::loop(), including untouched frames.
    void apply(const UiControlState& ui);

    // BLE requested duty for an actuator PWM pin. Applied immediately unless
    // the local UI currently owns that pin (a hold-button is pressed).
    void setRemoteDuty(uint8_t pin, uint8_t duty);
}

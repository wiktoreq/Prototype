#pragma once
#include <stdint.h>
#include "UiControlState.h"

namespace HardwareController
{
    // Applies initial external-light state. Call after DisplayApp::setup().
    void configure(const UiControlState& ui);

    // Maps current knob/button state onto PWM and external-light drivers.
    // Call every loop after DisplayApp::loop(), including untouched frames.
    void apply(const UiControlState& ui);

    // BLE requested duty for an actuator PWM pin. Applied immediately unless
    // the local UI currently owns that pin (a hold-button is pressed).
    void setRemoteDuty(uint8_t pin, uint8_t duty);
}

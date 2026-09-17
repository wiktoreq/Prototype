#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "PwmDriver.h"

namespace GpioInputs
{
    // Configures all digital inputs and initializes controlled outputs safely.
    void configure();

    // Applies the POJ actuator/LED controls and the mute falling-edge toggle.
    void loop();
}

#pragma once
#include <stdint.h>
#include "GpioInputs.h"
#include "Pins.h"
#include "PwmDriver.h"
#include "UiControlState.h"

namespace HardwareController
{
    // Applies initial slider values and stopped directions after UI setup.
    void configure(const UiControlState& ui);

    // Touchscreen event handlers.
    void setLightBrightness(uint8_t brightness);
    void toggleMute();
    void toggleSleep();
    void setHeightSpeed(uint8_t speed);
    void setHeightDirection(bool contracting, bool retracting);
    void setPositionSpeed(uint8_t speed);
    void setPositionDirection(bool contracting, bool retracting);

    // Applies a BLE-requested PWM duty immediately.
    void setRemoteDuty(uint8_t pin, uint8_t duty);
}

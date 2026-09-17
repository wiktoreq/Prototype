#pragma once
#include <Arduino.h>
#include "Pins.h"
#include "PwmDriver.h"

namespace GpioInputs
{
    using DisplayAction = void (*)();

    // Configures all digital inputs and initializes controlled outputs safely.
    void configure();

    // Registers the display power actions used by both sleep controls.
    void setDisplaySleepHandlers(DisplayAction sleepOn, DisplayAction sleepOff);

    // Applies the physical POJ, mute, and sleep controls.
    void loop();

    // Actions shared by physical controls and touchscreen event handlers.
    void toggleMute();
    void toggleSleep();
    void setLedDuty(uint8_t duty);
    void setActuator24VDirection(bool requestA, bool requestB);
    void setActuator12VDirection(bool requestA, bool requestB);
}

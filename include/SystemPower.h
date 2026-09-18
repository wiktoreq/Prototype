#pragma once

#include <stdint.h>

namespace SystemPower
{
    using DisplayPowerAction = void (*)();

    // Configures power-related outputs and resets all state.
    void configure();

    // Registers the display commands used when sleep mode changes.
    void setDisplayPowerActions(DisplayPowerAction turnDisplayOff,
                                DisplayPowerAction turnDisplayOn);

    // Reports whether the main program loop must remain frozen.
    bool isSleeping();

    // Shared actions for physical and touchscreen controls.
    void toggleSleep();
    void toggleMute();
    void toggleLed();
    void setLedDuty(uint8_t duty);
    void setLedBrightness(uint8_t percent);

    // Returns the duty currently applied to the 12 V LED.
    uint8_t getLedDuty();
}

#pragma once

#include <stdint.h>

namespace ActuatorController
{
    // Configures actuator direction pins and starts both actuators stopped.
    void configure();

    // Combines the physical direction inputs with active touchscreen requests.
    void updatePhysicalInputs();

    // Sets touchscreen direction requests. Conflicting requests stop the actuator.
    void set24VDirection(bool requestA, bool requestB);
    void set12VDirection(bool requestA, bool requestB);

    // Maps a percentage to the corresponding actuator PWM channel.
    void set24VSpeed(uint8_t percent);
    void set12VSpeed(uint8_t percent);

    // Clears touchscreen requests and drives every direction output LOW.
    void stopAll();
}

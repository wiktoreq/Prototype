#pragma once
#include <stdint.h>

namespace PwmDriver
{
    // Configures the LEDC timer and the two actuator PWM channels.
    void configure();

    // Updates duty on the channel mapped to ledPin (13 or 21).
    void updateDuty(uint8_t ledPin, uint8_t duty);
}

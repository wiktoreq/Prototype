#pragma once

#include <stdint.h>

namespace PwmDriver
{
    // Configures the LEDC timer, two actuator channels, and 12 V LED channel.
    void configure();

    // Converts a percentage in the range 0-100 to an 8-bit PWM duty.
    uint8_t percentToDuty(uint8_t percent);

    // Updates duty on the channel mapped to the supplied PWM output pin.
    void updateDuty(uint8_t pin, uint8_t duty);
}

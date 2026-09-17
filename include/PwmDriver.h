#pragma once
#include <stdint.h>
#include "Pins.h"
#include "driver/ledc.h"

namespace PwmDriver
{
    // Configures the LEDC timer, two actuator channels, and 12 V LED channel.
    void configure();

    // Updates duty on the channel mapped to the supplied PWM output pin.
    void updateDuty(uint8_t pin, uint8_t duty);
}

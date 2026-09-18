#pragma once

namespace GpioInputs
{
    // Configures every physical input and resets button tracking.
    void configure();

    // Handles all physical inputs while the system is awake.
    void update();

    // Checks only the physical sleep button while the system is frozen.
    void checkSleepButton();
}

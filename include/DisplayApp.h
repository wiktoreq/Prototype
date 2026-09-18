#pragma once

#include "UiControlState.h"

namespace DisplayApp
{
    // Initializes the display, touch input, menus, and UI event handlers.
    void setup();

    // Polls and routes touchscreen input.
    void loop();

    // Returns the slider values needed to initialize hardware outputs.
    UiControlState getControlState();

    // Sends the display controller into sleep mode.
    void sleepOn();

    // Wakes the display controller.
    void sleepOff();
}

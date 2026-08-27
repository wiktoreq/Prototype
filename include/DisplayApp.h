#pragma once

namespace DisplayApp
{
    // Initializes the TFT, touch calibration, tab bar, and content screens.
    void setup();

    // Polls the touchscreen and routes taps to the tab bar or active screen.
    void loop();
}

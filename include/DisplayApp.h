#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "FS.h"
#include "SPIFFS.h"
#include "ActuatorMenu.h"
#include "Config.h"
#include "GpioInputs.h"
#include "HardwareController.h"
#include "MainMenu.h"
#include "TopMenu.h"
#include "UiControlState.h"
#include "UiWidgets.h"

namespace DisplayApp
{
    // Initializes the TFT, touch calibration, tab bar, and content screens.
    void setup();

    // Polls the touchscreen and routes taps to the tab bar or active screen.
    void loop();

    void sleepOn();

    void sleepOff();

    // Returns the current knob values and hold-button flags for hardware.
    UiControlState getControlState();
}

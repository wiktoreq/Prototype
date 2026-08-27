#include <Arduino.h>

#include "GpioInputs.h"
#include "PwmDriver.h"
#include "Bluetooth.h"
#include "DisplayApp.h"
#include "HardwareController.h"

// Combined firmware entry point. The original front-end and back-end
// projects each had their own setup()/loop(); those live in DisplayApp
// and the Bluetooth/PWM modules, and are called from here.
// HardwareController maps UI knobs/buttons onto those hardware modules.

void setup()
{
    GpioInputs::configure();
    PwmDriver::configure();

    Serial.begin(115200);
    delay(10000);

    Bluetooth::configure();
    DisplayApp::setup();
    HardwareController::configure(DisplayApp::getControlState());
}

void loop()
{
    Bluetooth::loop();
    DisplayApp::loop();
    HardwareController::apply(DisplayApp::getControlState());
}

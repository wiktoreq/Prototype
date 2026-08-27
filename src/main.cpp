#include <Arduino.h>

#include "GpioInputs.h"
#include "PwmDriver.h"
#include "Bluetooth.h"
#include "DisplayApp.h"

// Combined firmware entry point. The original front-end and back-end
// projects each had their own setup()/loop(); those live in DisplayApp
// and the Bluetooth/PWM modules, and are called from here.

void setup()
{
    GpioInputs::configure();
    PwmDriver::configure();

    Serial.begin(115200);
    delay(10000);

    Bluetooth::configure();
    DisplayApp::setup();
}

void loop()
{
    Bluetooth::loop();
    DisplayApp::loop();
}

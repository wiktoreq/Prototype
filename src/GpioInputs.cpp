#include "GpioInputs.h"
#include "Pins.h"
#include <Arduino.h>

void GpioInputs::configure()
{
    pinMode(PIN_INPUT_A, INPUT);
    pinMode(PIN_INPUT_B, INPUT);
}

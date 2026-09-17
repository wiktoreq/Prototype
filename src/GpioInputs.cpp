#include "GpioInputs.h"
#include "Pins.h"
#include <Arduino.h>

void GpioInputs::configure()
{
    pinMode(POJ_1_A, INPUT);
    pinMode(POJ_1_B, INPUT);

    pinMode(POJ_2_A, INPUT);
    pinMode(POJ_2_B, INPUT);

    pinMode(POJ_3_A, INPUT);
    pinMode(POJ_3_B, INPUT);

    pinMode(MUTE_COM, INPUT_PULLUP);
    pinMode(MUTE_LED, OUTPUT);

    pinMode(SLEEP_COM, INPUT_PULLUP);
    pinMode(SLEEP_LED, OUTPUT);

    pinMode(ACTUATOR_A_24V, OUTPUT);
    pinMode(ACTUATOR_B_24V, OUTPUT);

    pinMode(ACTUATOR_A_12V, OUTPUT);
    pinMode(ACTUATOR_B_12V, OUTPUT);

    pinMode(LED12V_PWM, OUTPUT);

    pinMode(MIC_PHANTOM, OUTPUT);
}

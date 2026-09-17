#include "GpioInputs.h"

namespace
{
constexpr unsigned long LED_TOGGLE_HOLD_MS = 1000;

uint8_t led12VDuty = 255;
uint8_t rememberedLed12VDuty = 255;
unsigned long ledButtonsHeldSince = 0;
bool ledButtonsHeld = false;
bool ledToggleHandled = false;
bool monitoringMuteFlag = false;

void writeExclusivePair(uint8_t inputA, uint8_t inputB, uint8_t outputA, uint8_t outputB)
{
    const bool requestA = digitalRead(inputA) == HIGH;
    const bool requestB = digitalRead(inputB) == HIGH;

    if (requestA == requestB) {
        digitalWrite(outputA, LOW);
        digitalWrite(outputB, LOW);
    } else if (requestA) {
        digitalWrite(outputB, LOW);
        digitalWrite(outputA, HIGH);
    } else {
        digitalWrite(outputA, LOW);
        digitalWrite(outputB, HIGH);
    }
}

void updateLedDuty()
{
    const bool incrementPressed = digitalRead(POJ_3_A) == HIGH;
    const bool decrementPressed = digitalRead(POJ_3_B) == HIGH;

    if (incrementPressed && decrementPressed) {
        if (!ledButtonsHeld) {
            ledButtonsHeld = true;
            ledButtonsHeldSince = millis();
            ledToggleHandled = false;
        }

        if (!ledToggleHandled &&
            millis() - ledButtonsHeldSince >= LED_TOGGLE_HOLD_MS) {
            if (led12VDuty == 0) {
                led12VDuty = rememberedLed12VDuty;
            } else {
                rememberedLed12VDuty = led12VDuty;
                led12VDuty = 0;
            }
            PwmDriver::updateDuty(LED12V_PWM, led12VDuty);
            ledToggleHandled = true;
        }
        return;
    }

    ledButtonsHeld = false;
    ledToggleHandled = false;

    uint8_t newDuty = led12VDuty;
    if (incrementPressed && led12VDuty < 255) {
        newDuty++;
    } else if (decrementPressed && led12VDuty > 0) {
        newDuty--;
    }

    if (newDuty != led12VDuty) {
        led12VDuty = newDuty;
        PwmDriver::updateDuty(LED12V_PWM, led12VDuty);
    }
}

void updateMute()
{
    if (digitalRead(MUTE_COM) == HIGH) {
        monitoringMuteFlag = true;
        return;
    }

    if (monitoringMuteFlag) {
        monitoringMuteFlag = false;
        digitalWrite(MIC_PHANTOM, digitalRead(MIC_PHANTOM) == HIGH ? LOW : HIGH);
        digitalWrite(MUTE_LED, digitalRead(MUTE_LED) == HIGH ? LOW : HIGH);
    }
}
}

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

    digitalWrite(ACTUATOR_A_24V, LOW);
    digitalWrite(ACTUATOR_B_24V, LOW);
    digitalWrite(ACTUATOR_A_12V, LOW);
    digitalWrite(ACTUATOR_B_12V, LOW);
    digitalWrite(MUTE_LED, LOW);
    digitalWrite(MIC_PHANTOM, LOW);

    led12VDuty = 255;
    rememberedLed12VDuty = 255;
    ledButtonsHeld = false;
    ledToggleHandled = false;
    monitoringMuteFlag = false;
}

void GpioInputs::loop()
{
    writeExclusivePair(POJ_1_A, POJ_1_B, ACTUATOR_A_24V, ACTUATOR_B_24V);
    writeExclusivePair(POJ_2_A, POJ_2_B, ACTUATOR_A_12V, ACTUATOR_B_12V);
    updateLedDuty();
    updateMute();
}

#include "GpioInputs.h"

#include <Arduino.h>

#include "ActuatorController.h"
#include "Pins.h"
#include "SystemPower.h"

namespace
{
constexpr unsigned long LED_TOGGLE_HOLD_MILLISECONDS = 100;

bool muteButtonWasPressed = false;
bool sleepButtonWasPressed = false;
bool bothLedButtonsAreHeld = false;
bool ledToggleWasHandled = false;
unsigned long ledButtonsPressedAt = 0;

bool pinIsHigh(uint8_t pin)
{
    return digitalRead(pin) == HIGH;
}

void resetButtonTracking()
{
    muteButtonWasPressed = false;
    sleepButtonWasPressed = false;
    bothLedButtonsAreHeld = false;
    ledToggleWasHandled = false;
    ledButtonsPressedAt = 0;
}

void updateMuteButton()
{
    if (pinIsHigh(MUTE_COM)) {
        muteButtonWasPressed = true;
        return;
    }

    if (muteButtonWasPressed) {
        muteButtonWasPressed = false;
        SystemPower::toggleMute();
    }
}

void updateSleepButton()
{
    if (pinIsHigh(SLEEP_COM)) {
        sleepButtonWasPressed = true;
        return;
    }

    if (sleepButtonWasPressed) {
        sleepButtonWasPressed = false;
        SystemPower::toggleSleep();
    }
}

void beginLedButtonHold()
{
    bothLedButtonsAreHeld = true;
    ledToggleWasHandled = false;
    ledButtonsPressedAt = millis();
}

bool ledButtonHoldReachedToggleTime()
{
    unsigned long heldFor = millis() - ledButtonsPressedAt;
    return heldFor >= LED_TOGGLE_HOLD_MILLISECONDS;
}

void updateLedButtonHold()
{
    if (!bothLedButtonsAreHeld) {
        beginLedButtonHold();
    }

    if (ledToggleWasHandled) {
        return;
    }

    if (ledButtonHoldReachedToggleTime()) {
        SystemPower::toggleLed();
        ledToggleWasHandled = true;
    }
}

void resetLedButtonHold()
{
    bothLedButtonsAreHeld = false;
    ledToggleWasHandled = false;
}

void incrementLedDuty()
{
    uint8_t duty = SystemPower::getLedDuty();
    if (duty < 255) {
        SystemPower::setLedDuty(duty + 1);
    }
}

void decrementLedDuty()
{
    uint8_t duty = SystemPower::getLedDuty();
    if (duty > 0) {
        SystemPower::setLedDuty(duty - 1);
    }
}

void updateLedButtons()
{
    bool decrementIsPressed = pinIsHigh(POJ_3_A);
    bool incrementIsPressed = pinIsHigh(POJ_3_B);

    if (decrementIsPressed && incrementIsPressed) {
        updateLedButtonHold();
        return;
    }

    resetLedButtonHold();

    if (incrementIsPressed) {
        incrementLedDuty();
        return;
    }

    if (decrementIsPressed) {
        decrementLedDuty();
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
    pinMode(SLEEP_COM, INPUT_PULLUP);

    resetButtonTracking();
}

void GpioInputs::update()
{
    updateSleepButton();
    if (SystemPower::isSleeping()) {
        return;
    }

    ActuatorController::updatePhysicalInputs();
    updateLedButtons();
    updateMuteButton();
}

void GpioInputs::checkSleepButton()
{
    updateSleepButton();
}

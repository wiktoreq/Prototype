#include "SystemPower.h"

#include <Arduino.h>

#include "ActuatorController.h"
#include "Pins.h"
#include "PwmDriver.h"

namespace
{
bool sleepModeEnabled = false;
uint8_t currentLedDuty = 255;
uint8_t rememberedLedDuty = 255;
SystemPower::DisplayPowerAction turnDisplayOff = nullptr;
SystemPower::DisplayPowerAction turnDisplayOn = nullptr;

void enterSleepMode()
{
    sleepModeEnabled = true;
    ActuatorController::stopAll();

    digitalWrite(MIC_PHANTOM, LOW);
    digitalWrite(MUTE_LED, LOW);
    digitalWrite(SLEEP_LED, HIGH);
    digitalWrite(SCREEN_LED, HIGH);

    if (turnDisplayOff != nullptr) {
        turnDisplayOff();
    }
}

void leaveSleepMode()
{
    sleepModeEnabled = false;
    digitalWrite(SLEEP_LED, LOW);
    digitalWrite(SCREEN_LED, LOW);

    if (turnDisplayOn != nullptr) {
        turnDisplayOn();
    }
}
}

void SystemPower::configure()
{
    pinMode(MUTE_LED, OUTPUT);
    pinMode(SLEEP_LED, OUTPUT);
    pinMode(SCREEN_LED, OUTPUT);
    pinMode(MIC_PHANTOM, OUTPUT);

    digitalWrite(MUTE_LED, LOW);
    digitalWrite(SLEEP_LED, LOW);
    digitalWrite(SCREEN_LED, LOW);
    digitalWrite(MIC_PHANTOM, LOW);

    sleepModeEnabled = false;
    currentLedDuty = 255;
    rememberedLedDuty = 255;
}

void SystemPower::setDisplayPowerActions(DisplayPowerAction displayOffAction,
                                         DisplayPowerAction displayOnAction)
{
    turnDisplayOff = displayOffAction;
    turnDisplayOn = displayOnAction;
}

bool SystemPower::isSleeping()
{
    return sleepModeEnabled;
}

void SystemPower::toggleSleep()
{
    if (sleepModeEnabled) {
        leaveSleepMode();
        return;
    }

    enterSleepMode();
}

void SystemPower::toggleMute()
{
    bool phantomPowerIsOn = digitalRead(MIC_PHANTOM) == HIGH;
    bool muteLedIsOn = digitalRead(MUTE_LED) == HIGH;

    if (phantomPowerIsOn) {
        digitalWrite(MIC_PHANTOM, LOW);
    } else {
        digitalWrite(MIC_PHANTOM, HIGH);
    }

    if (muteLedIsOn) {
        digitalWrite(MUTE_LED, LOW);
    } else {
        digitalWrite(MUTE_LED, HIGH);
    }
}

void SystemPower::toggleLed()
{
    if (currentLedDuty == 0) {
        setLedDuty(rememberedLedDuty);
        return;
    }

    rememberedLedDuty = currentLedDuty;
    setLedDuty(0);
}

void SystemPower::setLedDuty(uint8_t duty)
{
    currentLedDuty = duty;
    PwmDriver::updateDuty(LED12V_PWM, currentLedDuty);
}

void SystemPower::setLedBrightness(uint8_t percent)
{
    uint8_t duty = PwmDriver::percentToDuty(percent);
    setLedDuty(duty);
}

uint8_t SystemPower::getLedDuty()
{
    return currentLedDuty;
}

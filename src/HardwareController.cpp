#include "HardwareController.h"

#include "ActuatorController.h"
#include "PwmDriver.h"
#include "SystemPower.h"

void HardwareController::configure(const UiControlState& ui)
{
    setLightBrightness(ui.lightBrightness);
    setHeightSpeed(ui.heightSpeed);
    setPositionSpeed(ui.positionSpeed);
    setHeightDirection(false, false);
    setPositionDirection(false, false);
}

void HardwareController::setLightBrightness(uint8_t brightness)
{
    SystemPower::setLedBrightness(brightness);
}

void HardwareController::toggleMute()
{
    SystemPower::toggleMute();
}

void HardwareController::toggleLed()
{
    SystemPower::toggleLed();
}

void HardwareController::setHeightSpeed(uint8_t speed)
{
    ActuatorController::set24VSpeed(speed);
}

void HardwareController::setHeightDirection(bool contracting, bool retracting)
{
    ActuatorController::set24VDirection(contracting, retracting);
}

void HardwareController::setPositionSpeed(uint8_t speed)
{
    ActuatorController::set12VSpeed(speed);
}

void HardwareController::setPositionDirection(bool contracting, bool retracting)
{
    ActuatorController::set12VDirection(contracting, retracting);
}

void HardwareController::setRemoteDuty(uint8_t pin, uint8_t duty)
{
    PwmDriver::updateDuty(pin, duty);
}

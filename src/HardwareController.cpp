#include "HardwareController.h"

namespace
{
uint8_t percentToDuty(uint8_t percent)
{
    if (percent >= 100) {
        return 255;
    }
    return static_cast<uint8_t>((percent * 255UL) / 100UL);
}
}

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
    GpioInputs::setLedDuty(percentToDuty(brightness));
}

void HardwareController::toggleMute()
{
    GpioInputs::toggleMute();
}

void HardwareController::toggleSleep()
{
    GpioInputs::toggleSleep();
}

void HardwareController::setHeightSpeed(uint8_t speed)
{
    PwmDriver::updateDuty(ACTUATOR_PWM_24V, percentToDuty(speed));
}

void HardwareController::setHeightDirection(bool contracting, bool retracting)
{
    GpioInputs::setActuator24VDirection(contracting, retracting);
}

void HardwareController::setPositionSpeed(uint8_t speed)
{
    PwmDriver::updateDuty(ACTUATOR_PWM_12V, percentToDuty(speed));
}

void HardwareController::setPositionDirection(bool contracting, bool retracting)
{
    GpioInputs::setActuator12VDirection(contracting, retracting);
}

void HardwareController::setRemoteDuty(uint8_t pin, uint8_t duty)
{
    PwmDriver::updateDuty(pin, duty);
}

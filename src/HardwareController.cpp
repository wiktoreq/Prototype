#include "HardwareController.h"

// Local hold-buttons win while pressed. On release the UI writes duty 0 for
// that actuator. When the UI does not own a pin, the last BLE duty stands.

static bool uiOwnsActuatorA = false;
static bool uiOwnsActuatorB = false;
static uint8_t lastUiDutyA = 0;
static uint8_t lastUiDutyB = 0;

// Maps a 0-100 slider onto the 8-bit LEDC duty used by PwmDriver.
static uint8_t speedToDuty(uint8_t speed)
{
    if (speed >= 100) {
        return 255;
    }
    return (uint8_t)((speed * 255UL) / 100UL);
}

// Drives one actuator speed pin from a hold-button pair. CONTRACT and RETRACT
// currently share the same speed PWM because no direction GPIOs are assigned;
// either button enables motion, XOR-both or neither stops it.
static void applyActuator(uint8_t pin,
                          uint8_t speed,
                          bool contracting,
                          bool retracting,
                          bool& uiOwned,
                          uint8_t& lastUiDuty)
{
    const bool uiDriving = contracting != retracting;
    if (uiDriving) {
        const uint8_t duty = speedToDuty(speed);
        if (!uiOwned || duty != lastUiDuty) {
            PwmDriver::updateDuty(pin, duty);
            lastUiDuty = duty;
        }
        uiOwned = true;
        return;
    }

    if (uiOwned) {
        PwmDriver::updateDuty(pin, 0);
        lastUiDuty = 0;
        uiOwned = false;
    }
}

void HardwareController::configure(const UiControlState& ui)
{
    uiOwnsActuatorA = false;
    uiOwnsActuatorB = false;
    lastUiDutyA = 0;
    lastUiDutyB = 0;
    BrightnessDriver::configure();
    BrightnessDriver::update(ui.lightBrightness);
}

void HardwareController::apply(const UiControlState& ui)
{
    BrightnessDriver::update(ui.lightBrightness);

    applyActuator(ACTUATOR_PWM_24V,
                  ui.heightSpeed,
                  ui.heightContracting,
                  ui.heightRetracting,
                  uiOwnsActuatorA,
                  lastUiDutyA);

    applyActuator(ACTUATOR_PWM_12V,
                  ui.positionSpeed,
                  ui.positionContracting,
                  ui.positionRetracting,
                  uiOwnsActuatorB,
                  lastUiDutyB);
}

void HardwareController::setRemoteDuty(uint8_t pin, uint8_t duty)
{
    const bool uiOwned =
        (pin == ACTUATOR_PWM_24V) ? uiOwnsActuatorA :
        (pin == ACTUATOR_PWM_12V) ? uiOwnsActuatorB :
        false;

    if (uiOwned) {
        return;
    }

    PwmDriver::updateDuty(pin, duty);
}

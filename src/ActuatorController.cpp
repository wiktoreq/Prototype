#include "ActuatorController.h"

#include <Arduino.h>

#include "Pins.h"
#include "PwmDriver.h"

namespace
{
struct DirectionRequest
{
    bool requestA;
    bool requestB;
};

DirectionRequest touchscreen24VRequest = {false, false};
DirectionRequest touchscreen12VRequest = {false, false};

void driveExclusivePair(bool requestA, bool requestB, uint8_t outputA, uint8_t outputB)
{
    if (requestA == requestB) {
        digitalWrite(outputA, LOW);
        digitalWrite(outputB, LOW);
        return;
    }

    if (requestA) {
        digitalWrite(outputB, LOW);
        digitalWrite(outputA, HIGH);
        return;
    }

    digitalWrite(outputA, LOW);
    digitalWrite(outputB, HIGH);
}

void updateDirectionPair(uint8_t inputA,
                         uint8_t inputB,
                         uint8_t outputA,
                         uint8_t outputB,
                         const DirectionRequest& touchscreenRequest)
{
    bool requestA = digitalRead(inputA) == HIGH;
    bool requestB = digitalRead(inputB) == HIGH;

    requestA = requestA || touchscreenRequest.requestA;
    requestB = requestB || touchscreenRequest.requestB;

    driveExclusivePair(requestA, requestB, outputA, outputB);
}

void update24VDirection()
{
    updateDirectionPair(POJ_1_A,
                        POJ_1_B,
                        ACTUATOR_A_24V,
                        ACTUATOR_B_24V,
                        touchscreen24VRequest);
}

void update12VDirection()
{
    updateDirectionPair(POJ_2_A,
                        POJ_2_B,
                        ACTUATOR_A_12V,
                        ACTUATOR_B_12V,
                        touchscreen12VRequest);
}
}

void ActuatorController::configure()
{
    pinMode(ACTUATOR_A_24V, OUTPUT);
    pinMode(ACTUATOR_B_24V, OUTPUT);
    pinMode(ACTUATOR_A_12V, OUTPUT);
    pinMode(ACTUATOR_B_12V, OUTPUT);

    stopAll();
}

void ActuatorController::updatePhysicalInputs()
{
    update24VDirection();
    update12VDirection();
}

void ActuatorController::set24VDirection(bool requestA, bool requestB)
{
    touchscreen24VRequest.requestA = requestA;
    touchscreen24VRequest.requestB = requestB;
    update24VDirection();
}

void ActuatorController::set12VDirection(bool requestA, bool requestB)
{
    touchscreen12VRequest.requestA = requestA;
    touchscreen12VRequest.requestB = requestB;
    update12VDirection();
}

void ActuatorController::set24VSpeed(uint8_t percent)
{
    uint8_t duty = PwmDriver::percentToDuty(percent);
    PwmDriver::updateDuty(ACTUATOR_PWM_24V, duty);
}

void ActuatorController::set12VSpeed(uint8_t percent)
{
    uint8_t duty = PwmDriver::percentToDuty(percent);
    PwmDriver::updateDuty(ACTUATOR_PWM_12V, duty);
}

void ActuatorController::stopAll()
{
    touchscreen24VRequest.requestA = false;
    touchscreen24VRequest.requestB = false;
    touchscreen12VRequest.requestA = false;
    touchscreen12VRequest.requestB = false;

    digitalWrite(ACTUATOR_A_24V, LOW);
    digitalWrite(ACTUATOR_B_24V, LOW);
    digitalWrite(ACTUATOR_A_12V, LOW);
    digitalWrite(ACTUATOR_B_12V, LOW);
}

#pragma once
#include <stdint.h>

// GPIO used as digital inputs in the original backend firmware.
constexpr uint8_t PIN_INPUT_A = 9;
constexpr uint8_t PIN_INPUT_B = 10;

// LEDC PWM outputs for the two actuator speed signals.
constexpr uint8_t PIN_ACTUATOR_A = 13;
constexpr uint8_t PIN_ACTUATOR_B = 21;

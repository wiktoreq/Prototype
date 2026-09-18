#pragma once

#include <stdint.h>

// 24 V actuator direction inputs
constexpr uint8_t POJ_1_A = 11;
constexpr uint8_t POJ_1_B = 12;

// 12 V actuator direction inputs
constexpr uint8_t POJ_2_A = 39;
constexpr uint8_t POJ_2_B = 40;

// 12 V LED brightness inputs
constexpr uint8_t POJ_3_A = 41;
constexpr uint8_t POJ_3_B = 42;

// Mute controls
constexpr uint8_t MUTE_LED = 21;
constexpr uint8_t MUTE_COM = 47;

// Sleep controls
constexpr uint8_t SLEEP_LED = 9;
constexpr uint8_t SLEEP_COM = 10;
// Board control line used together with the TFT sleep commands.
constexpr uint8_t SCREEN_LED = 8;

// 24 V actuator outputs
constexpr uint8_t ACTUATOR_A_24V = 37;
constexpr uint8_t ACTUATOR_B_24V = 38;
constexpr uint8_t ACTUATOR_PWM_24V = 1;

// 12 V actuator outputs
constexpr uint8_t ACTUATOR_A_12V = 35;
constexpr uint8_t ACTUATOR_B_12V = 36;
constexpr uint8_t ACTUATOR_PWM_12V = 2;

// 12 V LED PWM output
constexpr uint8_t LED12V_PWM = 14;

// Microphone phantom-power output
constexpr uint8_t MIC_PHANTOM = 48;

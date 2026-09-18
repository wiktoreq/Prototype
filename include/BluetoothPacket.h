#pragma once

#include <stdint.h>

// Packed BLE payload: target PWM pin and 8-bit duty cycle.
struct __attribute__((packed)) BluetoothPacket
{
    uint8_t targetPin;
    uint8_t duty;
};

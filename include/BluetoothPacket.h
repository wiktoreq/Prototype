#pragma once
#include <stdint.h>

// Packed BLE payload: target PWM pin and 8-bit duty cycle.
// The original type name is kept so the on-wire layout stays obvious.
struct __attribute__((packed)) BuetoothPacket
{
  uint8_t pin_num;
  uint8_t duty;
};

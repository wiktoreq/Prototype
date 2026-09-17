#pragma once
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <cstring>
#include "BluetoothPacket.h"
#include "HardwareController.h"

namespace Bluetooth
{
    // Creates the NimBLE UART-style server and starts advertising.
    void configure();

    // Handles received PWM packets (via HardwareController), serial-to-BLE notify,
    // and reconnect advertising.
    void loop();
}

#pragma once

namespace Bluetooth
{
    // Creates the NimBLE UART-style server and starts advertising.
    void configure();

    // Handles received PWM packets, serial-to-BLE notify, and reconnect advertising.
    void loop();
}

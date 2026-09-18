#include "Bluetooth.h"

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <cstring>

#include "BluetoothPacket.h"
#include "HardwareController.h"

namespace
{
constexpr char DEVICE_NAME[] = "ESP32-S3";
constexpr char SERVICE_UUID[] = "249efd7a-56f5-4013-ac77-80f50fa85304";
constexpr char RECEIVE_CHARACTERISTIC_UUID[] = "1b0484e2-08d1-4376-9173-32af58a86a36";
constexpr char TRANSMIT_CHARACTERISTIC_UUID[] = "badc375a-b3ef-4c63-ae97-bb73da0d7d34";

BluetoothPacket receivedPacket = {};
volatile bool packetIsReady = false;
NimBLEServer* server = nullptr;
NimBLECharacteristic* transmitCharacteristic = nullptr;
bool deviceIsConnected = false;
bool deviceWasConnected = false;

class ServerConnectionCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer*, NimBLEConnInfo&) override
    {
        deviceIsConnected = true;
    }

    void onDisconnect(NimBLEServer*, NimBLEConnInfo&, int) override
    {
        deviceIsConnected = false;
    }
};

class ReceiveCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic* characteristic, NimBLEConnInfo&) override
    {
        std::string value = characteristic->getValue();
        size_t receivedLength = value.length();
        size_t expectedLength = sizeof(BluetoothPacket);

        if (receivedLength != expectedLength) {
            Serial.printf("Warning: expected %u bytes but received %u bytes\n",
                          static_cast<unsigned int>(expectedLength),
                          static_cast<unsigned int>(receivedLength));
            return;
        }

        std::memcpy(&receivedPacket, value.data(), expectedLength);
        packetIsReady = true;
    }
};

void processReceivedPacket()
{
    if (!packetIsReady) {
        return;
    }

    packetIsReady = false;
    Serial.println("--- New Data Packet Received ---");
    Serial.printf("Target Pin: %u\n",
                  static_cast<unsigned int>(receivedPacket.targetPin));
    Serial.printf("Duty: %u\n",
                  static_cast<unsigned int>(receivedPacket.duty));
    Serial.println("--------------------------------");

    HardwareController::setRemoteDuty(receivedPacket.targetPin, receivedPacket.duty);
}

void transmitSerialInput()
{
    if (!deviceIsConnected || Serial.available() <= 0) {
        return;
    }

    String serialMessage = Serial.readString();
    transmitCharacteristic->setValue(serialMessage.c_str());
    transmitCharacteristic->notify();

    // Give the Bluetooth stack time to send before another notification.
    delay(10);
}

void updateAdvertisingState()
{
    if (!deviceIsConnected && deviceWasConnected) {
        delay(500);
        server->startAdvertising();
        Serial.println("Bluetooth advertising restarted");
    }

    deviceWasConnected = deviceIsConnected;
}
}

void Bluetooth::configure()
{
    NimBLEDevice::init(DEVICE_NAME);

    server = NimBLEDevice::createServer();
    server->setCallbacks(new ServerConnectionCallbacks());

    NimBLEService* service = server->createService(SERVICE_UUID);
    transmitCharacteristic = service->createCharacteristic(
        TRANSMIT_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::NOTIFY);

    NimBLECharacteristic* receiveCharacteristic = service->createCharacteristic(
        RECEIVE_CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::WRITE);
    receiveCharacteristic->setCallbacks(new ReceiveCallbacks());

    server->start();
    server->getAdvertising()->start();
    Serial.println("Bluetooth is waiting for a client");
}

void Bluetooth::loop()
{
    processReceivedPacket();
    transmitSerialInput();
    updateAdvertisingState();
}

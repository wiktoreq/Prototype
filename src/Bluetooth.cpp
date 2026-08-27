#include "Bluetooth.h"
#include "BluetoothPacket.h"
#include "HardwareController.h"
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <cstring>

#define SERVICE_UUID "249efd7a-56f5-4013-ac77-80f50fa85304" // UART service UUID
#define CHARACTERISTIC_UUID_RX "1b0484e2-08d1-4376-9173-32af58a86a36" //recieve
#define CHARACTERISTIC_UUID_TX "badc375a-b3ef-4c63-ae97-bb73da0d7d34" //transmit

static BuetoothPacket rxData;
static volatile bool newDataAvailable = false;

static NimBLEServer *pServer = NULL;
static NimBLECharacteristic *pTxCharacteristic;
static bool deviceConnected = false;
static bool oldDeviceConnected = false;

class MyServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) { deviceConnected = true; };
    void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) { deviceConnected = false; }
};

class MyCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo)
    {
        std::string value = pCharacteristic->getValue();
        size_t length = value.length();

        if (length == sizeof(BuetoothPacket))
        {
            memcpy(&rxData, value.data(), sizeof(BuetoothPacket));
            newDataAvailable = true;
        }
        else
        {
            Serial.printf("Warning: Expected %d bytes, but recieved %d\n", sizeof(BuetoothPacket), length);
        }
    }
};

void Bluetooth::configure()
{
    // Create the BLE Device
    NimBLEDevice::init("ESP32-S3");
    // Create the BLE Server
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    // Create the BLE Service
    NimBLEService *pService = pServer->createService(SERVICE_UUID);
    // Create a BLE Characteristic
    // NimBLE auto-creates the 0x2902 CCCD when NOTIFY is set (no BLE2902 needed)
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::NOTIFY);
    NimBLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());
    // Start the server so the service is started automatically.
    pServer->start();
    // Start advertising
    pServer->getAdvertising()->start();
    Serial.println("Waiting a client connection to notify..");
}

void Bluetooth::loop()
{
    if (newDataAvailable)
    {
        newDataAvailable = false;

        Serial.println("--- New Data Packet Recieved ---");
        Serial.printf("Target Pin: %d\n", rxData.pin_num);
        Serial.printf("State Set: %d\n", rxData.duty);
        Serial.println("-----------------------------------");
        HardwareController::setRemoteDuty(rxData.pin_num, rxData.duty);
    }
    if (deviceConnected)
    {
        if (Serial.available() > 0)
        {
            Serial.println("Device connected 1");
            String serialString = Serial.readString();
            Serial.println("Sending Value: " + serialString);
            pTxCharacteristic->setValue(serialString.c_str());
            pTxCharacteristic->notify();
            delay(10); // bluetooth stack will go into congestion, if too many packets are sent
        }
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

/*
 * LedService.cpp
 *
 * Created: 2019-04-20 15:22:32
 *  Author: kong
 */

#include "LedService.h"

LedService::LedService() : m_ledPin(LED_BUILTIN), m_buttonPin(4) // pin 4
{
    pinMode(m_ledPin, OUTPUT);
    pinMode(m_buttonPin, INPUT);
}

LedService::~LedService()
{
    end();
}

void LedService::setup()
{
    m_ledService = new BLEService("19B10010-E8F2-537E-4F6C-D104768A1214");
    m_ledControlCharacteristic  = new BLEByteCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
    m_buttonStateCharacteristic = new BLEByteCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
    //BLE.setLocalName("LedController");
    BLE.setAdvertisedService(*m_ledService);
    m_ledService->addCharacteristic(*m_ledControlCharacteristic);
    m_ledService->addCharacteristic(*m_buttonStateCharacteristic);
    BLE.addService(*m_ledService);
    //
    BLE.setEventHandler(BLEConnected, handle_LedConnectEvent);
    BLE.setEventHandler(BLEDisconnected, handle_LedDisconnectEvent);
    m_ledControlCharacteristic->setEventHandler(BLEWritten, handle_LedCharacteristicWrittenEvent);
    //
    m_ledControlCharacteristic->writeValue(0);
    m_buttonStateCharacteristic->writeValue(0);
    BLE.advertise();
}

void LedService::runService()
{
    char buttonValue = digitalRead(m_buttonPin);
    boolean buttonChanged = (m_buttonStateCharacteristic->value() != buttonValue);
    if (buttonChanged) {
        m_ledControlCharacteristic->writeValue(buttonValue);
        m_buttonStateCharacteristic->writeValue(buttonValue);
    }
    if (m_ledControlCharacteristic->written() || buttonChanged) {
        if (m_ledControlCharacteristic->value()) {
            Serial.println("LED on");  //----
            digitalWrite(m_ledPin, HIGH);
        } else {
            Serial.println("LED off"); //----
            digitalWrite(m_ledPin, LOW);
        }
    }
}

void LedService::end()
{
    delete m_ledService;
    delete m_ledControlCharacteristic;
    delete m_buttonStateCharacteristic;
}

void LedService::controlLed(int signal)
{
    if (signal == HIGH) {
        digitalWrite(m_ledPin, HIGH);
    } else {
        digitalWrite(m_ledPin, LOW);
    }
}

unsigned char LedService::getValue()
{
    return m_ledControlCharacteristic->value();
}

LedService x_ledService;

//
// EVENT HANDLER
//
void handle_LedConnectEvent(BLEDevice central)
{
    Serial.print("central connected event: ");
    Serial.println(central.address());
}

void handle_LedDisconnectEvent(BLEDevice central)
{
    Serial.print("central disconnected event: ");
    Serial.println(central.address());
}

void handle_LedCharacteristicWrittenEvent(BLEDevice central, BLECharacteristic characteristic)
{
    Serial.print("characteristic written event: ");
    if (x_ledService.getValue()) {
        Serial.println("LED on");
        x_ledService.controlLed(HIGH);
    } else {
        Serial.println("LED off");
        x_ledService.controlLed(LOW);
    }
}

/* EOF */
/*
 * BatteryService.cpp
 *
 * Created: 2019-04-20 13:57:21
 *  Author: kong
 */

#include "BatteryService.h"

BatteryService::BatteryService() : m_lastLevel(0), m_lastCheckTime(0)
{
}

BatteryService::~BatteryService()
{
    end();
}

void BatteryService::setup()
{
    m_batteryService = new BLEService("180F");
    m_batteryLevelCharacteristic = new BLEUnsignedCharCharacteristic("2A19", BLERead | BLENotify);
    //BLE.setLocalName("BatteryMonitor");
    BLE.setAdvertisedService(*m_batteryService);
    m_batteryService->addCharacteristic(*m_batteryLevelCharacteristic);
    BLE.addService(*m_batteryService);
    m_batteryLevelCharacteristic->writeValue(m_lastLevel);
    BLE.advertise();
}

void BatteryService::runService()
{
    unsigned long currentTime = millis();
    if ((currentTime - m_lastCheckTime) >= 1000) { // 1000ms
        m_lastCheckTime = currentTime;
        int level = analogRead(A0);
        int batteryLevel = map(level, 0, 1023, 0, 100);
        if (batteryLevel != m_lastLevel) {
            m_batteryLevelCharacteristic->writeValue((uint8_t)batteryLevel); // uint8
            m_lastLevel = batteryLevel;
            Serial.print("battery level % is now: "); //----
            Serial.println(batteryLevel); //----
        }
    }
}

void BatteryService::end()
{
    delete m_batteryService;
    delete m_batteryLevelCharacteristic;
}

BatteryService x_batteryService;

/* EOF */
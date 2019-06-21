/*
 * DIService.cpp
 *
 * Created: 2019-04-20 17:07:54
 *  Author: kong
 */

#include "DIService.h"

DIService::DIService()
{
}

DIService::~DIService()
{
    end();
}

void DIService::setup()
{
    m_diService = new BLEService("180A");
    m_manufacturerNameCharateristic = new BLEStringCharacteristic("2A29", BLERead, 14);
    m_modelNumberCharacteristic = new BLEStringCharacteristic("2A24", BLERead, 5);
    m_SerialNumberCharacteristic = new BLEStringCharacteristic("2A25", BLERead, 9);
    BLE.setAdvertisedService(*m_diService);
    m_diService->addCharacteristic(*m_manufacturerNameCharateristic);
    m_diService->addCharacteristic(*m_modelNumberCharacteristic);
    m_diService->addCharacteristic(*m_SerialNumberCharacteristic);
    BLE.addService(*m_diService);
    m_manufacturerNameCharateristic->writeValue("ReadDesignTech");
    m_modelNumberCharacteristic->writeValue("RDT-1");
    m_SerialNumberCharacteristic->writeValue("SN-000001");
    BLE.advertise();
}

void DIService::runService()
{
    BLE.poll();
}

void DIService::end()
{
    delete m_diService;
    delete m_manufacturerNameCharateristic;
    delete m_modelNumberCharacteristic;
    delete m_SerialNumberCharacteristic;
}

DIService x_diService;

/* EOF */
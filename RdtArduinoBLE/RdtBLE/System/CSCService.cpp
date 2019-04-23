/*
 * CSCService.cpp
 *
 * Created: 2019-04-20 18:00:26
 *  Author: kong
 */

#include "Tc5Timer.h"
#include "CSCService.h"

CSCService::CSCService() : 
m_flags(0), m_cumulativeWheelRevolution(0), m_lastWheelEventTime(0),
m_cumulativeCrankRevolution(0), m_lastCrankEventTime(0)
{
    m_notify = false;
    x_tc5_tick.begin(1000); // 1 sec tick
}

CSCService::~CSCService()
{
    end();
}

void CSCService::setup()
{
    m_cscService = new BLEService("1816");
    m_cscMeasurement = new BLECharacteristic("2A5B", BLENotify, 7, true); // 11 -> 7
    m_cscFeature = new BLEWordCharacteristic("2A5C", BLERead);
    m_sensorLocation = new BLEByteCharacteristic("2A5D", BLERead);
    //BLE.setLocalName("K-Roller");
    BLE.setAdvertisedService(*m_cscService);
    m_cscService->addCharacteristic(*m_cscMeasurement);
    m_cscService->addCharacteristic(*m_cscFeature);
    m_cscService->addCharacteristic(*m_sensorLocation);
    BLE.addService(*m_cscService);
    setMeasurementData();
    m_cscMeasurement->writeValue(m_data, 1); // 
    m_cscFeature->writeValueBE(0x0001); // wheel revolution supported
    m_sensorLocation->writeValue(12); // rear wheel
    BLE.advertise();
}

void CSCService::runService()
{
    if (m_flags == 0) {
        m_flags =  0x01; // wheel revolution data present
        x_tc5_tick.addCallback(&handle_CSCNotification);
        x_tc5_tick.start();
    }
    BLE.poll();
    
   if (m_flags == 0x01 && m_notify) {
       m_notify = false;
       //Serial.print("N");
       notifyMeasurementData();
   }
}

void CSCService::setMeasurementData()
{
    m_data[0] = m_flags;
    m_data[1] = (m_cumulativeWheelRevolution >> 24) & 0xff;
    m_data[2] = (m_cumulativeWheelRevolution >> 16) & 0xff;
    m_data[3] = (m_cumulativeWheelRevolution >> 8)  & 0xff;
    m_data[4] = (m_cumulativeWheelRevolution) & 0xff;
    m_data[5] = (m_lastWheelEventTime >> 8) & 0xff;
    m_data[6] = (m_lastWheelEventTime) & 0xff;
    m_data[7] = 0;
    //m_data[7]  = (m_cumulativeCrankRevolution >> 8) & 0xff;
    //m_data[8]  = (m_cumulativeCrankRevolution) & 0xff;
    //m_data[9]  = (m_lastCrankEventTime >> 8) & 0xff;
    //m_data[10] = (m_lastCrankEventTime) & 0xff;
    //m_data[11] = 0;
}

void CSCService::notifyMeasurementData()
{
    //
    x_distance += x_speed;
    m_cumulativeWheelRevolution = x_distance / x_wheel;
    uint32_t time = m_lastWheelEventTime + 1024;
    m_lastWheelEventTime = time % 65536; // rolls over every 64 seconds
    //
    setMeasurementData();
    m_cscMeasurement->writeValue(m_data, 7);
}

void CSCService::end()
{
    delete m_cscService;
    delete m_cscMeasurement;
    delete m_cscFeature;
    delete m_sensorLocation;
}

CSCService x_cscService;

//
// TIMER HANDLER
//
void handle_CSCNotification()
{
    //Serial.print("T");
    x_cscService.m_notify = true;
    //x_cscService.notifyMeasurementData();
}

/* EOF */
﻿/*
 * CSCService.h
 *
 * Created: 2019-04-20 17:59:50
 *  Author: kong
 */

#ifndef CSCSERVICE_H_
#define CSCSERVICE_H_

#include "ArduinoBLE.h"

extern uint32_t x_speed;
extern uint32_t x_distance;
extern uint32_t x_wheel;

class CSCService
{
public:
    CSCService();
    ~CSCService();
    void setup();
    void runService();
    void setMeasurementData();
    void notifyMeasurementData();
    void end();
    bool m_notify;
private:
    uint8_t  m_flags;
    uint32_t m_cumulativeWheelRevolution;
    uint16_t m_lastWheelEventTime; // 1/1024 sec resolution
    uint16_t m_cumulativeCrankRevolution;
    uint16_t m_lastCrankEventTime; // 1/1024 sec resolution
    uint8_t  m_data[12];
    BLEService* m_cscService; // 0x1816
    BLECharacteristic* m_cscMeasurement; // 0x2A5B
    BLEWordCharacteristic*   m_cscFeature;     // 0x2A5C
    BLEByteCharacteristic*   m_sensorLocation; // 0x2A5D
};

extern CSCService x_cscService;

//
// TIMER HANDLER
//
void handle_CSCNotification();

#endif /* CSCSERVICE_H_ */
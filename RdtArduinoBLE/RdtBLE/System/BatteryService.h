/*
 * BatteryService.h
 *
 * Created: 2019-04-20 13:56:46
 *  Author: kong
 */ 

#ifndef BATTERYSERVICE_H_
#define BATTERYSERVICE_H_

#include "ArduinoBLE.h"

class BatteryService
{
public:
    BatteryService();
    ~BatteryService();
    void setup();
    void runService();
    void end();
private:
    int m_lastLevel;
    unsigned long m_lastCheckTime; // millisec
    BLEService* m_batteryService;
    BLEUnsignedCharCharacteristic* m_batteryLevelCharacteristic;
};

extern BatteryService x_batteryService;

#endif /* BATTERYSERVICE_H_ */
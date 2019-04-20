/*
 * LedService.h
 *
 * Created: 2019-04-20 15:21:47
 *  Author: kong
 */ 

#ifndef LEDSERVICE_H_
#define LEDSERVICE_H_

#include "ArduinoBLE.h"

class LedService
{
public:
    LedService();
    ~LedService();
    void setup();
    void runService();
    void end();
    void controlLed(int signal);
    unsigned char getValue();
private:
    int m_ledPin;
    int m_buttonPin;
    BLEService* m_ledService;
    BLEByteCharacteristic* m_ledControlCharacteristic;
    BLEByteCharacteristic* m_buttonStateCharacteristic;
};

extern LedService x_ledService;

//
// EVENT HANDLER
//
void handleConnectEvent(BLEDevice central);
void handleDisconnectEvent(BLEDevice central);
void handleCharacteristicWrittenEvent(BLEDevice central, BLECharacteristic characteristic);

#endif /* LEDSERVICE_H_ */
/*
 * DIService.h
 *
 * Created: 2019-04-20 17:07:15
 *  Author: kong
 */

#ifndef DISERVICE_H_
#define DISERVICE_H_

#include "ArduinoBLE.h"

class DIService
{
public:
    DIService();
    ~DIService();
    void setup();
    void runService();
    void end();
private:
    BLEService* m_diService;
    BLEStringCharacteristic* m_manufacturerNameCharateristic;
    BLEStringCharacteristic* m_modelNumberCharacteristic;
    BLEStringCharacteristic* m_SerialNumberCharacteristic;
};

extern DIService x_diService;

#endif /* DISERVICE_H_ */
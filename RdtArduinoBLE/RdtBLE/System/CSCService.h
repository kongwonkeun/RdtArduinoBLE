/*
 * CSCService.h
 *
 * Created: 2019-04-20 17:59:50
 *  Author: kong
 */

#ifndef CSCSERVICE_H_
#define CSCSERVICE_H_

#include "ArduinoBLE.h"

class CSCService
{
public:
    CSCService();
    ~CSCService();
    void setup();
    void runService();
    void end();
private:
    BLEService* m_cscService;
    BLE
};



#endif /* CSCSERVICE_H_ */
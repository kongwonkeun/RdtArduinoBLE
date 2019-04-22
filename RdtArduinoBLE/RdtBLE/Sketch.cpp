/*
 * Sketch.cpp
 *
 * Created: 2019-04-17 15:39:06
 * Author : kong
 */

#include <Arduino.h>
#include "ArduinoBLE.h"
#include "LedService.h"
#include "DIService.h"
#include "CSCService.h"

void setup()
{
    Serial.begin(9600);
    while (!Serial);

    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }
    
    //x_ledService.setup();
    x_diService.setup();
    x_cscService.setup();
    
    Serial.println("waiting for BLE connection");
}

void loop()
{
    BLEDevice central = BLE.central();
    if (central) {
        Serial.print("connected to central: ");
        Serial.println(central.address());
        
        while (central.connected())
        {
            //x_ledService.runService();
            x_diService.runService();
            x_cscService.runService();
        }
        
        Serial.print("disconnected from central: ");
        Serial.println(central.address());
    }
}

/* EOF */
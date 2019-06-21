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
    /* bluetooth serial module interface
       Serial1.begin(115200);
    */
    BLE.setLocalName("K-Roller");
    x_cscService.setup();
    x_diService.setup();
    // x_batteryService.setup();
    // x_ledService.setup();

    Serial.println("waiting for BLE connection");
}

uint32_t x_speed = 0;
uint32_t x_distance = 0;
uint32_t x_wheel = 2;

void loop()
{
    /* two way communication between console and bluetooth serial terminal
    if (Serial.available()) {
        char c = Serial.read();
        Serial.write(c);
        Serial1.write(c);
    }
    if (Serial1.available()) {
        char x = Serial1.read();
        Serial.write(x);
    }
    */
    BLEDevice central = BLE.central();
    if (central) {

        Serial.print("connected to central: ");
        Serial.println(central.address());

        while (central.connected()) {
            x_cscService.runService();
            x_diService.runService();
            // x_batteryService.runService();
            // x_ledService.runService();
            /* control logic for csc profile (cycling speed and cadence) */
            if (Serial.available()) {
                char c = Serial.read();
                switch (c) {
                    case 'x':
                        x_speed++;
                        if (x_speed > 11) x_speed = 11;
                        break;
                    case 'z':
                        x_speed--;
                        if (x_speed <  0) x_speed = 0;
                        break;
                    case 'c':
                        x_speed = 0;
                        break;
                }
                Serial.write(c);
            }
        }
        Serial.print("disconnected from central: ");
        Serial.println(central.address());
    }

}

/* EOF */
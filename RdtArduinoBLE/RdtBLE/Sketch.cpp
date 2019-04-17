/*
 * Sketch.cpp
 *
 * Created: 2019-04-17 15:39:06
 * Author : kong
 */

#include <Arduino.h>
#include "ArduinoBLE.h"

void setup_battery_service();
void setup_led_control_callback_service();
void setup_led_control_service();
void setup_led_service();

void run_battery_service();
void run_led_control_callback_service();
void run_led_control_service();
void run_led_service();

void setup()
{
    Serial.begin(9600);
    while (!Serial);

    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }
    
    setup_battery_service();
    //setup_led_control_callback_service();
    //setup_led_control_service();
    //setup_led_service();
    
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
            run_battery_service();
            //run_led_control_callback_service();
            //run_led_control_service();
            //run_led_service();
        }
        
        Serial.print("disconnected from central: ");
        Serial.println(central.address());
    }
}

//
// BATTERY SERVICE
//
BLEService x_batteryService("180F");
BLEUnsignedCharCharacteristic x_batteryLevelCharacteristic("2A19", BLERead | BLENotify);
int  x_lastBatteryLevel = 0;
long x_lastMillisec = 0;

void setup_battery_service()
{
    BLE.setLocalName("BatteryMonitor");
    BLE.setAdvertisedService(x_batteryService); // add the service UUID
    x_batteryService.addCharacteristic(x_batteryLevelCharacteristic); // add the battery level characteristic
    BLE.addService(x_batteryService); // add the battery service
    x_batteryLevelCharacteristic.writeValue(x_lastBatteryLevel); // set initial value for this characteristic
    BLE.advertise();
}

void run_battery_service()
{
    long currentMillisec = millis();
    if ((currentMillisec - x_lastMillisec) >= 200) {
        x_lastMillisec = currentMillisec;
        int battery = analogRead(A0);
        int batteryLevel = map(battery, 0, 1023, 0, 100);
        if (batteryLevel != x_lastBatteryLevel) {
            Serial.print("battery level % is now: ");
            Serial.println(batteryLevel);
            x_batteryLevelCharacteristic.writeValue(batteryLevel);
            x_lastBatteryLevel = batteryLevel;
        }
    }
}

//
// LED CONTROL CALLBACK SERVICE
//
BLEService x_ledCallbackService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic x_ledCallbackCaracteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
const int x_ledCallbackPin = LED_BUILTIN;
void handleConnectEvent(BLEDevice central);
void handleDisconnectEvent(BLEDevice central);
void handleCharacteristicWrittenEvent(BLEDevice central, BLECharacteristic characteristic);

void setup_led_control_callback_service()
{
    BLE.setLocalName("LEDCallback");
    BLE.setAdvertisedService(x_ledCallbackService);
    x_ledCallbackService.addCharacteristic(x_ledCallbackCaracteristic);
    BLE.addService(x_ledCallbackService);
    BLE.setEventHandler(BLEConnected, handleConnectEvent);
    BLE.setEventHandler(BLEDisconnected, handleDisconnectEvent);
    x_ledCallbackCaracteristic.setEventHandler(BLEWritten, handleCharacteristicWrittenEvent);
    x_ledCallbackCaracteristic.setValue(0);
    BLE.advertise();
}

void run_led_control_callback_service()
{
    BLE.poll();
}

void handleConnectEvent(BLEDevice central)
{
    Serial.print("central connected event: ");
    Serial.println(central.address());
}

void handleDisconnectEvent(BLEDevice central)
{
    Serial.print("central disconnected event: ");
    Serial.println(central.address());
}

void handleCharacteristicWrittenEvent(BLEDevice central, BLECharacteristic characteristic)
{
    Serial.print("characteristic written event: ");
    if (x_ledCallbackCaracteristic.value()) {
        Serial.println("LED on");
        digitalWrite(x_ledCallbackPin, HIGH);
    } else {
        Serial.println("LED off");
        digitalWrite(x_ledCallbackPin, LOW);
    }
}

//
// LED CONTROL SERVICE
//
BLEService x_ledCntlService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic x_ledSwitchCaracteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
const int x_ledCntlPin = LED_BUILTIN;

void setup_led_control_service()
{
    pinMode(x_ledCntlPin, OUTPUT);
    BLE.setLocalName("LED");
    BLE.setAdvertisedService(x_ledCntlService);
    x_ledCntlService.addCharacteristic(x_ledSwitchCaracteristic);
    BLE.addService(x_ledCntlService);
    x_ledSwitchCaracteristic.writeValue(0);
    BLE.advertise();
}

void run_led_control_service()
{
    if (x_ledSwitchCaracteristic.written()) {
        if (x_ledSwitchCaracteristic.value()) {
            Serial.println("LED on");
            digitalWrite(x_ledCntlPin, HIGH);
        } else {
            Serial.println(F("LED off"));
            digitalWrite(x_ledCntlPin, LOW);
        }
    }
}

//
// LED ON OFF NOTIFICATION SERVICE
//
BLEService x_ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service
BLEByteCharacteristic x_ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic x_buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
const int x_ledPin = LED_BUILTIN; // set LED Pin to on-board LED
const int x_buttonPin = 4; // set Button Pin to digital pin 4

void setup_led_service()
{
    pinMode(x_ledPin, OUTPUT); // use the LED as an output
    pinMode(x_buttonPin, INPUT); // use button pin as an input
    BLE.setLocalName("ButtonLED");
    BLE.setAdvertisedService(x_ledService);
    x_ledService.addCharacteristic(x_ledCharacteristic);
    x_ledService.addCharacteristic(x_buttonCharacteristic);
    BLE.addService(x_ledService);
    x_ledCharacteristic.writeValue(0);
    x_buttonCharacteristic.writeValue(0);
    BLE.advertise();
}

void run_led_service()
{
    char buttonValue = digitalRead(x_buttonPin);
    boolean buttonChanged = (x_buttonCharacteristic.value() != buttonValue);
    if (buttonChanged) {
        x_ledCharacteristic.writeValue(buttonValue);
        x_buttonCharacteristic.writeValue(buttonValue);
    }
    if (x_ledCharacteristic.written() || buttonChanged) {
        if (x_ledCharacteristic.value()) {
            Serial.println("LED on");
            digitalWrite(x_ledPin, HIGH);
        } else {
            Serial.println("LED off");
            digitalWrite(x_ledPin, LOW);
        }
    }
}

/* EOF */
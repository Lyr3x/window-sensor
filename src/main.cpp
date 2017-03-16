/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * DESCRIPTION
 *
 * window/door sensor with sleep function
 * Future: Batterylevel sending!
 */


// Enable debug prints to serial monitor
#define MY_DEBUG
#define SLEEP_TIME 0
// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

#include <SPI.h>
#include <MySensors.h>
#include <Bounce2.h>
#include <Arduino.h>
#include <BatteryMeter.h>

#define CHILD_ID 3
#define BUTTON_PIN  3  // Arduino Digital I/O pin for button/reed switch

Bounce debouncer = Bounce();
int oldValue=-1;
int value = -1;
int getDebouncedValue();

// Change to V_LIGHT if you use S_LIGHT in presentation below
MyMessage msg(CHILD_ID,V_TRIPPED);




BatteryMeter battery(3); //BatteryMeter instance
#define CHILD_ID_BATTERY 2  //MySensors Battery child ID
#define BATTERY_FULL 4.2 // a 18650 lithium ion battery usually give 4.2V when full
#define BATTERY_ZERO 2.4 // 2.4V limit for 328p at 16MHz. 1.9V, limit for nrf24l01 without
MyMessage voltage_msg(CHILD_ID_BATTERY, V_VOLTAGE); //MySensors battery voltage message instance
void sendBatteryStatus();

void setup()
{
  Serial.begin(115200);
  // Setup the button
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  // Activate internal pull-up
  digitalWrite(BUTTON_PIN,HIGH);

  // After setting up the button, setup debouncer
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(5);

}

void presentation() {
  // Register binary input sensor to gw (they will be created as child devices)
  // You can use S_DOOR, S_MOTION or S_LIGHT here depending on your usage.
  // If S_LIGHT is used, remember to update variable type you send in. See "msg" above.
  present(CHILD_ID, S_DOOR);
  present(CHILD_ID_BATTERY, S_CUSTOM);  //battery level
}
//  Check if digital input has changed and send in new value
void loop()
{
  sendBatteryStatus();
  sleep(digitalPinToInterrupt(BUTTON_PIN), CHANGE, SLEEP_TIME);
  delay(500);
  getDebouncedValue();

#ifdef MY_DEBUG
    Serial.print("New value: ");
    Serial.println(value);
    Serial.print("Old value: ");
    Serial.println(oldValue);
#endif

  delay(100);
  if (value != oldValue) {
     // Send in the new value
     send(msg.set(value==HIGH ? 1 : 0));

    #ifdef MY_DEBUG
    if(value==HIGH){
        Serial.println("window is open");
    }else{
      Serial.println("window is closed");
    }
    #endif
    // Serial.print("New value: ");
    // Serial.println(value);
     oldValue = value;
  }
}

int getDebouncedValue(){
  // Get the update value
  for (size_t i = 0; i < 5; i++) {
    debouncer.update();
    value = debouncer.read();
    delay(10);
    /* code */
  }
  return value;
}

void sendBatteryStatus(){
  float voltage = battery.checkBatteryLevel();
  send(voltage_msg.set(voltage, 3)); // redVcc returns millivolts. Set wants volts and how many decimals (3 in our case)
  sendBatteryLevel(round((voltage - BATTERY_ZERO) * 100.0 / (BATTERY_FULL - BATTERY_ZERO)));
}

#include "BatteryMeter.h"

int oldBatteryPcnt = 0;
long oldvoltage = 0;
int sensorValue = 0;
int batteryPcnt = 0;
float voltage = 0;

BatteryMeter::BatteryMeter(int pin)
{
	_pin = pin;
}

float BatteryMeter::checkBatteryLevel()
{
  sensorValue = analogRead(_pin);
	batteryPcnt = sensorValue / 10;
	voltage = sensorValue * (3.30/1023.00);

	if(oldvoltage != voltage){
		oldvoltage = voltage;
		return voltage;
	}else{
		return oldvoltage;
	}
}

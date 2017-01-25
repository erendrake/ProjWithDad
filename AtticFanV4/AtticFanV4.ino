/**
   The MySensors Arduino library handles the wireless radio link and protocol
   between your home built sensors/actuators and HA controller of choice.
   The sensors forms a self healing radio network with optional repeaters. Each
   repeater and gateway builds a routing tables in EEPROM which keeps track of the
   network topology allowing messages to be routed to nodes.

   Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
   Copyright (C) 2013-2015 Sensnology AB
   Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors

   Documentation: http://www.mysensors.org
   Support Forum: http://forum.mysensors.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   version 2 as published by the Free Software Foundation.

 *******************************

   DESCRIPTION

   Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
   http://www.mysensors.org/build/temp
*/


// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24

#define MY_NODE_ID 3
#include "config.h"
// Enabled repeater feature for this node
// #define MY_REPEATER_FEATURE

#include <SPI.h>
#include <MySensors.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#define attic_Louver_Open LOW
#define attic_Louver_Closed HIGH
#define attic_Fan_On LOW
#define attic_Fan_Off HIGH

//Temperature Thresholds
const int systemDiffOn = SYSTEM_DIFF_ON;
const int systemDiffOff = SYSTEM_DIFF_OFF;

// Timer
const int minimumFanOnMs = 60000;
const int fanDelayMs = 10000;
const int loopSleepTimerMs = 10000;

#define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No

#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 16

unsigned long SLEEP_TIME = 3000; // Sleep time between reads (in milliseconds) No sleep if repeter
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature.

float lastTemperature[MAX_ATTACHED_DS18B20];
int numSensors = 0;
bool receivedConfig = false;
bool metric = true;
// Initialize temperature message


// Define Child ID's for MySensors GW
const int Number_Child_IDs = 4; // Number of Child ID's
const int Number_Temp_Sensors = 2; // Number of Temp Sensors
const int Shop_Temp_ID = 0; // Shop Temp Child ID
const int Attic_Temp_ID = 1; // Attic Temp Child ID
const int Attic_Fan_ID = 10; // Attic Fan on/off
const int Attic_Louver_ID = 11; // Attic Louver open/closed

// Set Default Readings
float ShopTemp = -127.0;
float AtticTemp = -127.0;
float currentShopTemp = -127.0;
float currentAtticTemp = -127.0;

// system Status
bool attac_Fan = false;   // ??????
bool attic_Louver = false;// ??????
bool atticFanStatus = false;
bool atticLouverStatus = false;


// Initialize temperature message
MyMessage msg_attic_temp(Attic_Temp_ID, V_TEMP);
MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
MyMessage msg_attic_fan(Attic_Fan_ID, V_STATUS);
MyMessage msg_attic_louver(Attic_Louver_ID, V_STATUS);
MyMessage msg(0, V_TEMP);

// Hex Addresses of Dallas Temp Sensors
DeviceAddress Shop_Temp_Addr = { 0x28, 0xB8, 0x3D, 0x29, 0x07, 0x00, 0x00, 0x49 };
DeviceAddress Attic_Temp_Addr = { 0x28, 0x30, 0x12, 0x29, 0x07, 0x00, 0x00, 0x95 };



void before()
{
	//  TRANSPORT_DEBUG(PSTR("VERBOSE:B\n"));
	  // Startup up the OneWire library
	const int tempResolution = 9;
	sensors.setResolution(Shop_Temp_Addr, tempResolution);
	sensors.setResolution(Attic_Temp_Addr, tempResolution);
	// Startup up the OneWire library
	sensors.begin();
}

void setup()
{
	// requestTemperatures() will not block current thread
	sensors.setWaitForConversion(false);
	//  setupPins(); 
	pinMode(attic_Fan_Pin, OUTPUT);
	digitalWrite(attic_Fan_Pin, HIGH);
	pinMode(attic_Louver_Pin, OUTPUT);
	digitalWrite(attic_Louver_Pin, HIGH);
	//  pinMode(runtimePin, OUTPUT);  
	//  digitalWrite(runtimePin, HIGH); 
}

void presentation() {
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("Temperature Sensor", "1.1");

	//Send the sensor node sketch version information to the gateway
	// Send the sketch version information to the gateway and Controller
	sendSketchInfo("AF and LC:", "V3");

	// Fetch the number of attached temperature sensors  
	numSensors = sensors.getDeviceCount();
	Serial.print("# of Sensors= ");
	Serial.println(numSensors);

	// Present all sensors to controller
	// Array for storing temps
	present(Shop_Temp_ID, S_TEMP, "ShopTemp");
	present(Attic_Temp_ID, S_TEMP, "AtticTemp");
	present(Attic_Fan_ID, S_BINARY, "AtticFan");
	present(Attic_Louver_ID, S_BINARY, "AtticLouver");

	// Present all sensors to controller
	for (int i = 0; i < numSensors && i < MAX_ATTACHED_DS18B20; i++) {
		present(i, S_TEMP);
	}
}

void loop()
{
	// Request temperatures from Dallas sensors
	sensors.requestTemperatures();
	// Wait for sensors to resolve
	wait(1000);
	// Print results
	Serial.println();
	Serial.print("Number of Devices found on bus = ");
	Serial.println(sensors.getDeviceCount());
	Serial.print("Getting temperatures... ");
	Serial.println();
	// 
	// ************ Get Attic Temp by Address *****************
	Serial.print("Attic Temperature is:   ");
	AtticTemp = printTemperature(sensors, Attic_Temp_Addr);
	Serial.println();
	currentAtticTemp = AtticTemp;
	// Send GW the new Attic_Temp
	Serial.println();
	send(msg_attic_temp.setSensor(Attic_Temp_ID).set(AtticTemp, 1));
	// ********** End if get Attic Temp by Address ************ 

	// ************ Get Shop Temp by Address *******************
	Serial.print("Shop Temperature is:   ");
	ShopTemp = printTemperature(sensors, Shop_Temp_Addr);
	Serial.println();
	currentShopTemp = ShopTemp;
	// Send GW the new Shop_Temp
	Serial.println();
	send(msg_shop_temp.setSensor(Shop_Temp_ID).set(ShopTemp, 1));
	// ************ End of Get Shop Temp by Address ************


	// **** Open Louver and Turn On Fan If Attic is Hotter Than The Shop ****
	//  float systemDifference = AtticTemp - ShopTemp;
	Serial.print("Just about to call processAtticFan systemDiff= ");
	//    Serial.println(int(systemDifference));
	processAtticFan();
}
// ******* End of viod loop ********

//******** Start of printTemperature ***********
float printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
	float tempC = sensors.getTempC(deviceAddress);
	if (tempC == -127.00)
	{
		Serial.print("Error getting temperature  ");
	}
	else
	{
		Serial.print("F: ");
		Serial.print(tempF);
		//   Serial.print(" F: ");
		//   Serial.print(DallasTemperature::toFahrenheit(tempC));
		return(tempF);
	}
}// ***** End printTemperature ********




//*********** Stat of processAtticFan *************
// All Relays and LED's are Active LOW

void processAtticFan() {
	Serial.println("We made it to the processAtticFan");
	float systemDifference = AtticTemp - ShopTemp;
	if (systemDifference > systemDiffOn && atticFanStatus == false && atticLouverStatus == false) {
		digitalWrite(attic_Louver_Pin, attic_Louver_Open);
		atticLouverStatus = true;
		//    TRANSPORT_DEBUG(PSTR("Attic Louver: Opening!\n"));
			//wait for Louver to open
		delay(fanDelayMs);
		//    TRANSPORT_DEBUG(PSTR("Attic Louver: Open!\n"));
		//  Send GW New Status
		send(msg_attic_louver.setSensor(Attic_Louver_ID).set(1));
		digitalWrite(attic_Fan_Pin, attic_Fan_On);
		atticFanStatus = true;
		//    TRANSPORT_DEBUG(PSTR("Attic Fan: On!\n"));
		//  Send GW New Status
		send(msg_attic_fan.setSensor(Attic_Fan_ID).set(1));
		return;
	}
	else if (systemDifference < systemDiffOff && atticFanStatus == true && atticLouverStatus == true) {
		digitalWrite(attic_Louver_Pin, attic_Louver_Closed);
		atticLouverStatus = false;
		//    TRANSPORT_DEBUG(PSTR("Attic Louver: Closing!\n"));
		//  Send GW New Status
		send(msg_attic_louver.setSensor(Attic_Louver_ID).set(0));
		//wait for fan to stop 
		delay(fanDelayMs);

		digitalWrite(attic_Fan_Pin, attic_Fan_Off);
		atticFanStatus = false;
		//    TRANSPORT_DEBUG(PSTR("Attic Fan: Off!\n"));
		//  Send GW New Status
		send(msg_attic_fan.setSensor(Attic_Fan_ID).set(0));
		return;
	}
}// ********** End processAtticFan ************

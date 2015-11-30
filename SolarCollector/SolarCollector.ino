#include "config.h"
#include <MySensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Communications
const int serialBPS = SERIAL_BPS;

// Timer
const int minimumPumpOnSeconds = MINIMUM_PUMP_ON_SECONDS;
const int tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;

// LED Pins
const int panelPumpLedPin = PANEL_PUMP_LED_PIN;
const int runtimePin = RUNTIME_LED_PIN;
const int generalAlarmPin = GENERAL_ALARM_LED_PIN;

// Relay Pins
const int panelPumpPin = PANEL_PUMP_PIN;

// Sensor Pins
const int tempBusPin = TEMP_BUS_PIN;

// Sensor Settings
const int tempResolution = TEMP_9_BIT;
const DeviceAddress tempPanelSensorAddress = TEMP_PANEL_SENSOR_ADDRESS;
const DeviceAddress tempTankSensorAddress = TEMP_TANK_SENSOR_ADDRESS;


OneWire  ds(tempBusPin);
DallasTemperature sensors(&ds);


 
void setup() {
  setupLedPins();
  
  Serial.begin(serialBPS);

  setupTempSensors();
}

void loop() {
  digitalWrite(runtimePin, LOW);
  requestTempSensors();
  logTempSensors();  

  processPanelPump();
  
  digitalWrite(runtimePin, HIGH);  
  delay(tempPollingDelay * 1000);
}


  



void setupTempSensors(){  
  Serial.println("Solaranator 4000");
  
  sensors.begin();  
  sensors.setResolution(tempResolution);
  int deviceCount = sensors.getDeviceCount();
  Serial.print("Found ");
  Serial.print(deviceCount);
  Serial.println(" temp probes");   

  for (int i = 0; i < deviceCount; i++) {
    DeviceAddress address;
    if(sensors.getAddress(address, i))
    {
      Serial.print("Address for the device ");
      Serial.print(i +1);
      Serial.print(" (index ");    
      Serial.print(i);
      Serial.print(" ) is: ");
      printAddress(address);        
      Serial.println("");
    } 
  }
}

void processPanelPump(){
  
  
}

void setupLedPins(){
  pinMode(panelPumpLedPin, OUTPUT);   
  digitalWrite(panelPumpLedPin, HIGH);  
  pinMode(panelPumpPin, OUTPUT);   
  digitalWrite(panelPumpPin, HIGH);  
  pinMode(panelPumpLedPin, OUTPUT);  
  digitalWrite(panelPumpLedPin, HIGH);  
  pinMode(runtimePin, OUTPUT);  
  digitalWrite(runtimePin, HIGH);  
}

void requestTempSensors(){  
  Serial.print("Requesting temperatures from pin: ");
  Serial.println(tempBusPin);
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  
}

void logTempSensors(){
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  for (int i = 0; i < sensors.getDeviceCount(); i++) { 
    Serial.print("Temperature for the device ");
    Serial.print(i + 1);
    Serial.print(" ( index: ");    
    Serial.print(i);
    Serial.print(" ) is: ");
    Serial.println(sensors.getTempFByIndex(i));  
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

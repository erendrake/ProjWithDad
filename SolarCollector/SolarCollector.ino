#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Communications
const int serialBPS = SERIAL_BPS;

//Temperature Thresholds
const int systemDiffOn = SYSTEM_DIFF_ON;
const int systemDiffOff = SYSTEM_DIFF_OFF;
const int systemOverheat = SYSTEM_OVERHEAT;

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
const char* tempPanelSensorName = TEMP_PANEL_SENSOR_NAME;
const DeviceAddress tempTankSensorAddress = TEMP_TANK_SENSOR_ADDRESS;
const char* tempTankSensorName = TEMP_TANK_SENSOR_NAME;

OneWire  ds(tempBusPin);
DallasTemperature sensors(&ds);

// Current Readings
float currentTankTemp = -127.0;
float currentPanelTemp = -127.0;

// Pump Status
bool panelPumpStatus = false;

 
void setup() {
  setupLedPins(); 
  pinMode(panelPumpPin, OUTPUT); 
  Serial.begin(serialBPS);
  setupTempSensors();
}

void loop() {
  digitalWrite(runtimePin, LOW);
  requestTempSensors();
  currentPanelTemp = logTempSensor(tempPanelSensorAddress, tempPanelSensorName);  
  currentTankTemp = logTempSensor(tempTankSensorAddress, tempTankSensorName);  

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
  ds.reset_search();

  bool foundPanelSensor = false;
  bool foundTankSensor = false;

  byte addr[8];
  while(ds.search(addr)){
     if(memcmp(tempPanelSensorAddress, addr, 8) == 0)
     {
       Serial.println("Found Panel!");
       foundPanelSensor = true;
     }
     else if(memcmp(tempTankSensorAddress, addr, 8) == 0)
     {
       Serial.println("Found Tank!");
       foundTankSensor = true;
     }
     else {
       Serial.println("Found Other!");
     }
  }

  if(foundPanelSensor && foundTankSensor){
    Serial.println("Woot!");
  }
  else {
    Serial.println("Bummer!");
  }
}


void processPanelPump(){
  float systemDifference = currentPanelTemp - currentTankTemp;

  if (currentTankTemp > systemOverheat){  
    digitalWrite(panelPumpPin, LOW);  
    return;
  }

  if (systemDifference > systemDiffOn && panelPumpStatus == false){
    digitalWrite(panelPumpPin, HIGH);  
    Serial.println("PanelPump: On!");
    panelPumpStatus = true;
  }
  else if(systemDifference < systemDiffOff && panelPumpStatus == true){
    digitalWrite(panelPumpPin, LOW);  
    Serial.println("PanelPump: Off!");
    panelPumpStatus = false;
  }
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

float logTempSensor(const DeviceAddress address, const char* name){    
    Serial.print("Temperature for the device ");
    Serial.print(name);
    Serial.print(" is: ");
    float result = sensors.getTempF(address);
    Serial.println(result);  
    return result;
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

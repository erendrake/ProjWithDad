/**
"Solar Heater Controller", "1.1 RW/CW 11/11/16" 
 * Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
 * http://www.mysensors.org/build/temp
 */


// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24
#include "config.h"
#include <SPI.h>
#define MY_NODE_ID 25
#include <MySensors.h>  
#include <DallasTemperature.h>
#include <OneWire.h>

#define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 16

// Solar addons
//Temperature Thresholds
const int systemDiffOn = SYSTEM_DIFF_ON;
const int systemDiffOff = SYSTEM_DIFF_OFF;
const int systemOverheat = SYSTEM_OVERHEAT;

// Timer
const int minimumPumpOnSeconds = MINIMUM_PUMP_ON_SECONDS;
const int tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;

// LED Pins
const int runtimePin = RUNTIME_LED_PIN;
const int generalAlarmPin = GENERAL_ALARM_LED_PIN;
const int panelPumpLED = 13;
// Relay Pins
const int panelPumpPin = PANEL_PUMP_PIN;

// Sensor Pins
const int tempBusPin = TEMP_BUS_PIN;

// Sensor Settings
const int tempResolution = TEMP_9_BIT;
//const DeviceAddress tempPanelSensorAddress = TEMP_PANEL_SENSOR_ADDRESS;
//const DeviceAddress tempTankSensorAddress = TEMP_TANK_SENSOR_ADDRESS;
const char blarg = "hi buddy!";

// OneWire  ds(tempBusPin);
// DallasTemperature sensors(&ds);

// Current Readings
float currentTankTemp = -127.0;
float currentPanelTemp = -127.0;

// Pump Status
bool panelPumpStatus = false;

// 
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 

//
float lastTemperature[MAX_ATTACHED_DS18B20];
int numSensors=0;
bool receivedConfig = false;
bool metric = true;
// Initialize temperature message
MyMessage msg(0,V_TEMP);

void before()
{
  TRANSPORT_DEBUG(PSTR("BLARG:B\n"));
  // Startup up the OneWire library
  sensors.begin();
}

void setup()  
{ 
  TRANSPORT_DEBUG(PSTR("BLARG:S\n"));
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  setupPins(); 
}



void presentation() {
  TRANSPORT_DEBUG(PSTR("BLARG:P\n"));
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Solar Heater Controller", "1.1 RW/CW 12/1/16");
  // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();
  // Present all sensors to controller
  // Array for storing temps
  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {   
     present(i, S_TEMP);
  }
}

float getTempByIndex(int index){
  
  float temperature = static_cast<float>(static_cast<int>(sensors.getTempFByIndex(index)) * 10.) / 10.;
  //TRANSPORT_DEBUG(PSTR("Temp @ index: " + index + " " + temperature + "f\n"));
  return temperature;
}

void loop()     
{ 
  TRANSPORT_DEBUG(PSTR("BLARG:L\n"));
  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

  // query conversion time and sleep until conversion completed
  int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  // sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
  wait(conversionTime);

  // Read temperatures and send them to controller 
  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {
 
    // Fetch and round temperature to one decimal
    float temperature = getTempByIndex(i);
    // Only send data if temperature has changed and no error
    #if COMPARE_TEMP == 1
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
    #else
    if (temperature != -127.00 && temperature != 85.00) {
    #endif
 
      // Send in the new temperature
      send(msg.setSensor(i).set(temperature,1));
      // Save new temperatures for next compare
      lastTemperature[i]=temperature;

      //Solar addons
    digitalWrite(runtimePin, LOW); // ???
 /* requestTempSensors();
  currentPanelTemp = logTempSensor(tempPanelSensorAddress, tempPanelSensorName);  
  currentTankTemp = logTempSensor(tempTankSensorAddress, tempTankSensorName);  
*/
 if (i==1)  currentPanelTemp = temperature;
 if (i==0)  currentTankTemp = temperature;
  
  processPanelPump();
  
  digitalWrite(runtimePin, HIGH); // ??? 
  delay(tempPollingDelay * 1000);     
    }
  }
  wait(SLEEP_TIME);
}
// All Relays and LED's are Active LOW
void processPanelPump(){
  float systemDifference = currentPanelTemp - currentTankTemp;

  if (currentTankTemp > systemOverheat){  
    digitalWrite(panelPumpPin, HIGH);  
    digitalWrite(panelPumpLED, HIGH);
    panelPumpStatus = false;
    return;
  }

  if (systemDifference > systemDiffOn && panelPumpStatus == false){
    digitalWrite(panelPumpPin, LOW);  
    Serial.println("PanelPump: On!");
    digitalWrite(panelPumpLED, LOW);
    panelPumpStatus = true;
  }
  else if(systemDifference < systemDiffOff && panelPumpStatus == true){
    digitalWrite(panelPumpPin, HIGH);  
    Serial.println("PanelPump: Off!");
    digitalWrite(panelPumpLED, HIGH);
    panelPumpStatus = false;
  }
}
//
void setupPins(){
  pinMode(panelPumpPin, OUTPUT);   
  digitalWrite(panelPumpPin, HIGH);  
  pinMode(runtimePin, OUTPUT);  
  digitalWrite(runtimePin, HIGH);
  pinMode(panelPumpLED, OUTPUT);
  digitalWrite(panelPumpPin, HIGH);  
}
//
float logTempSensor(const DeviceAddress address, const char* name){    
    Serial.print("Temperature for the device ");
    Serial.print(name);
    Serial.print(" is: ");
    float result = sensors.getTempF(address);
    Serial.println(result);  
    return result;
}
// function to print a device address
/*void printAddress(DeviceAddress deviceAddress)
{
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}*/



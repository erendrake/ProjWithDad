/**
 
 * Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
 * http://www.mysensors.org/build/temp
 */


// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24
#include "config.h"
#include <SPI.h>
#include <MySensors.h>  
#include <DallasTemperature.h>
#include <OneWire.h>


// Solar addons
//Temperature Thresholds
const int systemDiffOn = SYSTEM_DIFF_ON;
const int systemDiffOff = SYSTEM_DIFF_OFF;

// Timer
const int minimumPumpOnMs = MINIMUM_PUMP_ON_SECONDS * 1000;
const int fanDelayMs = FAN_DELAY_SECONDS * 1000;
const int loopSleepTimerMs = LOOP_SLEEP_TIMER_SECONDS * 1000;

// LED Pins
const int runtimePin = RUNTIME_LED_PIN;
const int generalAlarmPin = GENERAL_ALARM_LED_PIN;

// Relay Pins
const int intakeLouverPin = INTAKE_LOUVER_PIN;
const int atticFanPin = ATTIC_FAN_PIN;

// Sensor Pins
const int tempBusPin = TEMP_BUS_PIN;

// Sensor Settings
const int tempResolution = TEMP_9_BIT;
//const DeviceAddress tempPanelSensorAddress = TEMP_PANEL_SENSOR_ADDRESS;
//const DeviceAddress tempTankSensorAddress = TEMP_TANK_SENSOR_ADDRESS;

// OneWire  ds(tempBusPin);
// DallasTemperature sensors(&ds);

// Current Readings
float currentTankTemp = -127.0;
float currentPanelTemp = -127.0;

// system Status
bool atticFanStatus = false;
bool intakeLouverStatus = false;

// 
OneWire oneWire(tempBusPin); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
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
  TRANSPORT_DEBUG(PSTR("VERBOSE:B\n"));
  // Startup up the OneWire library
  sensors.begin();
}

void setup()  
{ 
  TRANSPORT_DEBUG(PSTR("VERBOSE:S\n"));
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  setupPins(); 
}



void presentation() {
  TRANSPORT_DEBUG(PSTR("VERBOSE:P\n"));
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Temperature Sensor", "1.1");
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
  TRANSPORT_DEBUG(PSTR("VERBOSE:L\n"));
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
 if (i==0)  currentAtticTemp = temperature;
 if (i==1)  currentShopTemp = temperature;
  
      processAtticFan();
  
      digitalWrite(runtimePin, HIGH); // ??? 
    }
  }
  wait(loopSleepTimerMs);
}

// All Relays and LED's are Active LOW
void processAtticFan(){
  float systemDifference = currentAtticTemp - currentShopTemp;

  if (systemDifference > systemDiffOn && atticFanStatus == false && intakeLouverStatus == false){
    digitalWrite(intakeLouverPin, LOW);  
    intakeLouverStatus = true;
    TRANSPORT_DEBUG(PSTR("Intake Louver: Opening!\n"));

    //wait for louvers to open
    delay(fanDelayMs);
    TRANSPORT_DEBUG(PSTR("Intake Louver: Open!\n"));

    digitalWrite(atticFanPin, LOW);  
    atticFanStatus = true;
    TRANSPORT_DEBUG(PSTR("Attic Fan: On!\n"));
    return;
  }
  else if(systemDifference < systemDiffOff && atticFanStatus == true && intakeLouverStatus == true){
    digitalWrite(intakeLouverPin, HIGH);  
    intakeLouverStatus = false;
    TRANSPORT_DEBUG(PSTR("Intake Louver: Closing!\n"));

    //wait for fan to stop 
    delay(fanDelayMs);

    digitalWrite(atticFanPin, HIGH);  
    atticFanStatus = false;
    TRANSPORT_DEBUG(PSTR("Attic Fan: Off!\n"));
    return;
  }
}


//
void setupPins(){
  pinMode(panelPumpPin, OUTPUT);   
  digitalWrite(panelPumpPin, HIGH);  
  pinMode(runtimePin, OUTPUT);  
  digitalWrite(runtimePin, HIGH);  
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



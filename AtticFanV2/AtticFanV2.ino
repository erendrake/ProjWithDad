/**
 
 * Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
 * http://www.mysensors.org/build/temp
 */


// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24
#define MY_NODE_ID 3
#include "config.h"
#include <SPI.h>
#include <MySensors.h>  
#include <DallasTemperature.h>
#include <OneWire.h>
#define attic_Louvers_Open LOW
#define attic_Louvers_Closed HIGH
#define attic_Fan_On LOW
#define attic_Fan_Off HIGH

//Temperature Thresholds
const int systemDiffOn = SYSTEM_DIFF_ON;
const int systemDiffOff = SYSTEM_DIFF_OFF;

// Timer
const int minimumFanOnMs = MINIMUM_FAN_ON_SECONDS * 1000;
const int fanDelayMs = FAN_DELAY_SECONDS * 1000;
const int loopSleepTimerMs = LOOP_SLEEP_TIMER_SECONDS * 1000;

// LED Pins
const int runtimePin = RUNTIME_LED_PIN;
const int generalAlarmPin = GENERAL_ALARM_LED_PIN;

// Relay Pins
const int attic_Louver_Pin = ATTIC_LOUVER_PIN;
const int attic_Fan_Pin = ATTIC_FAN_PIN;

// Sensor Pins
const int tempBusPin = TEMP_BUS_PIN;

// Sensor Settings
const int tempResolution = TEMP_9_BIT;
//const DeviceAddress tempPanelSensorAddress = TEMP_PANEL_SENSOR_ADDRESS;
//const DeviceAddress tempTankSensorAddress = TEMP_TANK_SENSOR_ADDRESS;

// Define Child ID's for MySensors GW
  const int Number_Child_IDs           =    4; // Number of Child ID's
  const int Number_Temp_Sensors        =    2; // Number of Temp Sensors
  const int Shop_Temp_ID               =    0; // Shop Temp Child ID
  const int Attic_Temp_ID              =    1; // Attic Temp Child ID
  const int Attic_Fan_ID               =   10; // Attic Fan on/off
  const int Attic_Louver_ID            =   11; // Attic Louver open/closed

// OneWire  ds(tempBusPin); ????
// DallasTemperature sensors(&ds);  ?????

// Current Readings
float ShopTemp = -127.0;
float AtticTemp = -127.0;
float currentShopTemp = -127.0;
float currentAtticTemp = -127.0;


// system Status
  bool attac_Fan = false;
  bool attic_Louver = false;
  bool atticFanStatus = false;
  bool atticLouverStatus = false;

DeviceAddress Attic_Temp_Addr       = {0x28, 0xB8, 0x3D, 0x29, 0x07, 0x00, 0x00, 0x49};
DeviceAddress Shop_Temp_Addr        = {0x28, 0x30, 0x12, 0x29, 0x07, 0x00, 0x00, 0x95};

// 
OneWire oneWire(tempBusPin); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 


//
float lastTemperature[MAX_ATTACHED_DS18B20];
int numSensors=0;
bool receivedConfig = false;
bool metric = true;



// Initialize temperature message
//MyMessage msg(0,V_TEMP);
  MyMessage msg_attic_temp(Attic_Temp_ID ,V_TEMP);
  MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
  MyMessage msg_attic_fan(Attic_Fan_ID ,V_LIGHT);
  MyMessage msg_attic_louver(Attic_Louver_ID, V_STATUS);
  
void before()
{
  TRANSPORT_DEBUG(PSTR("VERBOSE:B\n"));
  // Startup up the OneWire library
  const int tempResolution = 9;
  sensors.setResolution(Shop_Temp_Addr, tempResolution);
  sensors.setResolution(Attic_Temp_Addr, tempResolution);
  sensors.begin();

}

void setup()  
{ 
  TRANSPORT_DEBUG(PSTR("VERBOSE:S\n"));
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
//  setupPins(); 
  pinMode(attic_Fan_Pin, OUTPUT);   
  digitalWrite(attic_Fan_Pin, HIGH);  
  pinMode(attic_Louver_Pin, OUTPUT);   
  digitalWrite(attic_Louver_Pin, HIGH);
  pinMode(runtimePin, OUTPUT);  
  digitalWrite(runtimePin, HIGH);  
}

void presentation() {
  TRANSPORT_DEBUG(PSTR("VERBOSE:P\n"));
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Attic Fan/Louver Controller:","1.1");
  // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();
  // Present all sensors to controller
  // Array for storing temps
    present(Attic_Temp_ID, S_TEMP);
    present(Shop_Temp_ID, S_TEMP);
    present(Attic_Fan_ID, S_LIGHT);
    present(Attic_Louver_ID, S_BINARY);
//  MyMessage msg_attic_temp(Attic_Temp_ID ,V_TEMP);
//  MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
//  MyMessage msg_attic_fan(Attic_Fan_ID ,V_LIGHT);
//  MyMessage msg_attic_louver(Attic_Louver_ID, V_STATUS);    
}


  
//  float temperature = static_cast<float>(static_cast<int>(sensors.getTempFByIndex(index)) * 10.) / 10.;
//  //TRANSPORT_DEBUG(PSTR("Temp @ index: " + index + " " + temperature + "f\n"));
//  return temperature;

void loop()     
{ 
  TRANSPORT_DEBUG(PSTR("VERBOSE:L\n"));
  wait(1000);

  Serial.println();
  Serial.print("Number of Devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());   
  Serial.print("Getting temperatures... ");  
  Serial.println();   
    
  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

  Serial.print("Attic Temperature is:   ");
  printTemperature(sensors, Attic_Temp_Addr);
  Serial.println();

  Serial.print("Shop Temperature is:   ");
  printTemperature(sensors, Shop_Temp_Addr);
  Serial.println();

  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

// Get Attic Temp by Address
//  if (currentAtticTemp != Attic_Temp && Attic_Temp != -127.00 && Attic_Temp != 85.00){
    // Save new temperatures for next compare
    currentAtticTemp=AtticTemp;
    // Send GW the new Attic_Temp
    send(msg.setSensor(Attic_Temp_ID).set(Attic_Temp,1));
//    } 
    
// Get Shop Temp by Address
//  if (currentShopTemp != Shop_Temp && Shop_Temp != -127.00 && Shop_Temp != 85.00){ 
     // Save new temperatures for next compare
    currentShopTemp=ShopTemp;
    // Send GW the new Shop_Temp
    send(msg.setSensor(Shop_Temp_ID).set(Shop_Temp,1));
//    }
 
      // Send in the new temperature
      send(msg.setSensor(i).set(temperature,1));
      // Save new temperatures for next compare
      lastTemperature[i]=temperature;

      //
    digitalWrite(runtimePin, LOW); // ???
//    Open Louvers and Turn On Fan If Attic is Hotter Than The Shop
      processAtticFan();
        digitalWrite(runtimePin, HIGH); // ??? 


  //wait(loopSleepTimerMs);
}
// ******* End of viod loop ********

// All Relays and LED's are Active LOW
void processAtticFan(){
  float systemDifference = currentAtticTemp - currentShopTemp;

  if (systemDifference > systemDiffOn && atticFanStatus == false && atticLouverStatus == false){
    digitalWrite(attic_Louver_Pin, attic_Louver_Open);  
    atticLouverStatus = true;
    TRANSPORT_DEBUG(PSTR("Attic Louver: Opening!\n"));

    //wait for louvers to open
    delay(fanDelayMs);
    TRANSPORT_DEBUG(PSTR("Attic Louver: Open!\n"));
    digitalWrite(attic_Fan_Pin, attic_Fan_On);  
    atticFanStatus = true;
    TRANSPORT_DEBUG(PSTR("Attic Fan: On!\n"));
          send(msg.setSensor(i).set(temperature,1));
    return;
  }
  else if(systemDifference < systemDiffOff && atticFanStatus == true && atticLouverStatus == true){
    digitalWrite(attic_Louver_Pin, attic_Louver_Closed);  
    atticLouverStatus = false;
    TRANSPORT_DEBUG(PSTR("Attic Louver: Closing!\n"));

    //wait for fan to stop 
    delay(fanDelayMs);

    digitalWrite(attic_Fan_Pin, attic_Fan_On);  
    atticFanStatus = false;
    TRANSPORT_DEBUG(PSTR("Attic Fan: Off!\n"));
    return;
  }
}


/*-----( Declare User-written Functions )-----*/

void printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
   float tempC = sensors.getTempC(deviceAddress);
   if (tempC == -127.00) 
   {
   Serial.print("Error getting temperature  ");
   } 
   else
   {
   Serial.print("C: ");
   Serial.print(tempC);
   Serial.print(" F: ");
   Serial.print(DallasTemperature::toFahrenheit(tempC));
   }
}// End printTemperature

float logTempSensor(const DeviceAddress address, const char* name){    
    Serial.print("Temperature for the device ");
    Serial.print(name);
    Serial.print(" is: ");
    float result = sensors.getTempF(address);
    Serial.println(result);  
    return result;
}




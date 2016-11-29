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
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <MySensors.h>  
#include <DallasTemperature.h>
#include <OneWire.h>
#define MY_NODE_ID 25
#define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 16
#define I2C_ADDR    0x27 // <<- Add your address here.
#define Rs_pin  0
#define Rw_pin  1
#define En_pin  2
#define BACKLIGHT_PIN 3
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

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

// Relay Pins
const int panelPumpPin = PANEL_PUMP_PIN;

// Sensor Pins
const int tempBusPin = TEMP_BUS_PIN;

// Sensor Settings
const int tempResolution = TEMP_9_BIT;
//const DeviceAddress tempPanelSensorAddress = TEMP_PANEL_SENSOR_ADDRESS;
//const DeviceAddress tempTankSensorAddress = TEMP_TANK_SENSOR_ADDRESS;
const char blarg = "hi buddy!";

// Define Child ID's for MySensors GW
#define Number_Child_IDs      6 // Number of Child ID's
#define Panel_Temp            0 // Panel Temp Child ID
#define Tank_Temp             1 // Tank Temp Child ID
#define Shop_Temp             2 // Shop Temp Child ID
#define Panel_Pump            3 // Panel Pump Child ID
#define Panel_Pump_Pressure   4 // Panel Pump Pressure Child ID
#define Panel_Pump_Flow       5 // Panel Pump Flow Child ID

// OneWire  ds(tempBusPin);
// DallasTemperature sensors(&ds);

// Set Current Temp Sensor Readings to Not Found
float currentTankTemp = -127.0;
float currentPanelTemp = -127.0;
float currentShopTemp = -127.0;

// 
//  intPanelTemp = static_cast<int>(currentPanelTemp);
//  intTankTemp = static_cast<int>(currentTankTemp);  
//  intShopTemp = static_cast<int>(currentShopTemp);
//  intPumpPressure = static_cast<int>(pumpPressure);
  int intPanelTemp = 0;
  int intTankTemp = 0;
  int intShopTemp = 0;  
  int intPumpPressure = 0;

// Shop Pump Status as Off
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
// MyMessage msg(0,V_TEMP);
MySensor gw;
MyMessage msg_panel_temp(Panel_Temp,V_TEMP);
MyMessage msg_tank_temp(Tank_Temp,V_TEMP);
MyMessage msg_shop_temp(Shop_Temp,V_TEMP);
MyMessage msg_panel_pump(Panel_Pump,V_STATUS);
MyMessage msg_panel_pump(Panel_Pump_Pressure,V_PRESSURE);
MyMessage msg_flow(Pump_Flow,V_FLOW);
/* #define Number_Child_IDs      6 // Number of Child ID's
#define Panel_Temp            0 // Panel Temp Child ID
#define Tank_Temp             1 // Tank Temp Child ID
#define Shop_Temp             2 // Shop Temp Child ID
#define Panel_Pump            3 // Panel Pump Child ID
#define Panel_Pump_Pressure   4 // Panel Pump Pressure Child ID
#define Panel_Pump_Flow       5 // Panel Pump Flow Child ID
*/

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
//  setupPins();
  lcd.begin (16,2); // <<-- our LCD is a 20x4, change for your LCD if needed
  // LCD Backlight ON
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home on LCD
  // Pressure Sensor is on Pin A0
  int pumpPressure = 0;           // variable to store the value read   
}

void presentation() {
  TRANSPORT_DEBUG(PSTR("BLARG:P\n"));
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Solar Heater Controller", "1.1 RW/CW 11/11/16");
  // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();
  // Present all sensors to controller
  // Array for storing temps
    //  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {   
    //     present(i, S_TEMP);}

      present(Panel_Temp, S_TEMP);                // Panel Temp Child ID 0
      present(Tank_Temp, S_TEMP);                 // Tank Temp Child ID 1
      present(Shop_Temp, S_TEMP);                 // Shop Temp Child ID 2
      present(Panel_Pump, S_PUMP);                // Panel Pump Child ID 3
      present(Panel_Pump_Pressure, S_PRESSURE);   // Panel Pump Pressure Child ID 4
      present(Panel_Pump_Flow, S_FLOW);           // Panel Pump Flow Child ID 6
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
  //for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {
//  for (int 1=0;  
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
  if (i==2)  currentShopTemp = temperature;  

// Go see if the panel pump needs to get turned on?
  processPanelPump();

// Panel Pump Pressure Sensor
  int sensorValue = analogRead(Pressure_Sensor_Pin);    // read the input pin Value range 0-1024
  float voltage = sensorValue * (5.0/1023.0);
  Serial.print("Voltage= ");
  Serial.println(int(voltage));
  float pumpPressure = ((voltage-111)/7); // Should be in PSI
  Serial.print("Pump_Pressure= ");
  Serial.print(float(pumpPressure));
  Serial.println("PSI"); //  
  // Send in the the Current Pump Pressure
  //    send(msg.setSensor(numSensors+2).set(pumpPressure,1));
 
  // Convert sensor flots to int and display
//  intPanelTemp = static_cast<int>(currentPanelTemp);
//  intTankTemp = static_cast<int>(currentTankTemp);  
//  intShopTemp = static_cast<int>(currentShopTemp);
//  intPumpPressure = static_cast<int>(pumpPressure);
  lcd.setCursor (0,0); // go to start of 1st line
  lcd.print("                ");
  lcd.setCursor (0,1); // go to start of 1st line
  lcd.print("                ");
  lcd.setCursor (0,0); // go to start of 1st line
  lcd.print("PT:");     
  lcd.setCursor (3,0); // 
  lcd.print(int(currentPanelTemp));
  lcd.setCursor (7,0); // 
  lcd.print("TT:");
  lcd.setCursor (10,0); // 
  lcd.print(int(currentTankTemp));
  lcd.setCursor (0,1); // 
  lcd.print("ST:");
  lcd.setCursor (3,1); // 
  lcd.print(int(currentShopTemp));
  lcd.setCursor (7,1); // 
  lcd.print("PP:");
  lcd.setCursor (10,1); // 
  lcd.print(int(pumpPressure));  
  
  
  digitalWrite(runtimePin, HIGH); // ??? 
  delay(tempPollingDelay * 1000);     
    }
  }
//  wait(SLEEP_TIME);
}
// All Relays and LED's are Active LOW
void processPanelPump(){
  float systemDifference = currentPanelTemp - currentTankTemp;

  if (currentTankTemp > systemOverheat){  
    digitalWrite(panelPumpPin, HIGH);  
    return;
  }

  if (systemDifference > systemDiffOn && panelPumpStatus == false){
    digitalWrite(panelPumpPin, LOW);  
    Serial.println("PanelPump: On!");
    panelPumpStatus = true;
  }
  else if(systemDifference < systemDiffOff && panelPumpStatus == true){
    digitalWrite(panelPumpPin, HIGH);  
    Serial.println("PanelPump: Off!");
    panelPumpStatus = false;
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



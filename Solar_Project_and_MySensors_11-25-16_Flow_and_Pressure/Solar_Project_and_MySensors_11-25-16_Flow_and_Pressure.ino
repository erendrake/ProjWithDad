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
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 16

// LCD Display

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
const DeviceAddress tempPanelSensorAddress = TEMP_PANEL_SENSOR_ADDRESS;
const DeviceAddress tempTankSensorAddress = TEMP_TANK_SENSOR_ADDRESS;
const DeviceAddress tempShopSensorAddress = TEMP_SHOP_SENSOR_ADDRESS;
const char blarg = "hi buddy!";

// OneWire  ds(tempBusPin);
// DallasTemperature sensors(&ds);

// Current Readings
float currentTankTemp = -127.0;
float currentPanelTemp = -127.0;
float currentShopTemp = -127.0;

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
// Flow Sensor
/*
Liquid flow rate sensor -DIYhacking.com Arvind Sanjeev
Measure the liquid/water flow rate using this code. 
Connect Vcc and Gnd of sensor to arduino, and the 
signal line to arduino digital pin 2.
 */
byte statusLed    = 13;
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;
// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

// Pressure Sensor is on Pin A0
// pinMode(A0, INPUT);
int pumpPressure = 0;           // variable to store the value read


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
  
  //Flow Sensor Set up the status LED line as an output
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

// Start Up the LCD 
    lcd.begin (16,2); // <<-- our LCD is a 16x2
// LCD Backlight ON
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home (); // go home on LCD
    lcd.print("Solar Heater RW Ver:1.0"); 
}

void presentation() {
  TRANSPORT_DEBUG(PSTR("BLARG:P\n"));
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Solar Heater Controller", "1.1 RW/CW 11/11/16");
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
  // Flow Sensor
   if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    frac = (flowRate - int(flowRate)) * 10;
    Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    Serial.print("L/min");
    // Print the number of litres flowed in this second
    Serial.print("  Current Liquid Flowing: ");             // Output separator
    Serial.print(flowMilliLitres);
    Serial.print("mL/Sec");

    // Print the cumulative total of litres flowed since starting
    Serial.print("  Output Liquid Quantity: ");             // Output separator
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
   // Send in the Current Flow Rate to the Gateway
  //      send(msg.setSensor(numSensors+1).set(flowMilliLiters,1));
  
// Pressure Sensor
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
      
// Temp Sensor
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
 if (i==2)  currentShopTemp = temperature;
  processPanelPump();
  
 // Send Temps to LCD Display
 lcd.setCursor (0,0); // go to start of 1st line
 lcd.print("Solar Panel:");
 lcd.setCursor (13,0); 
 lcd.print(currentPanelTemp);
 lcd.setCursor (0,1);
 lcd.print("Tank:");
 lcd.setCursor (6,1); 
 lcd.print(currentTankTemp);
  
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

// Insterrupt Service Routine For Flow Meter
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
// function to print a device address
/*void printAddress(DeviceAddress deviceAddress)
{
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}*/



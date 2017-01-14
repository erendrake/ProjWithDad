/**
  "SolarHeaterControllerV4-2-2", "RW/CW 1/12/17"
   Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
   http://www.mysensors.org/build/temp
*/

// Running on Mega 1280
// Enable debug prints to serial monitor
#define MY_DEBUG
//  #define MY_SPECIAL_DEBUG
// Enable and select radio type attached
#define MY_RADIO_NRF24
#define MY_NODE_ID 25
// Pin Confuguration for Arduino Mega
#define MY_RF24_CE_PIN 40
#define MY_RF24_CS_PIN 53
#include <SPI.h>
#include <MySensors.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 24 // Pin where dallase sensors are connected 
#define MAX_ATTACHED_DS18B20 16
#include "config.h"
#include <LiquidCrystal.h>
//  #include <LCD.h>
//  #include <LiquidCrystal_I2C.h>
//test


// Solar addons
//Temperature Thresholds
const int systemDiffOn = SYSTEM_DIFF_ON;
const int systemDiffOff = SYSTEM_DIFF_OFF;
const int systemOverheat = SYSTEM_OVERHEAT;
float systemDifference = - 20.0;

// Thermostat
const int set_Temp_Pot_Pin = A4;

// **** Timers ****
const int minimumPumpOnSeconds = MINIMUM_PUMP_ON_SECONDS;
const int tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;
const int minimumFanOnMs = 60000;
const int fanDelayMs = 10000;
const int loopSleepTimerMs = 10000;
const int buttonPin = A5;    // the number of the pushbutton pin

// **** Tank Pressure Sensor Pin ****
const int tank_Pump_Pressure_Pin = A15;

// **** Relay Pins ****
const int tankPumpPin = TANK_PUMP_PIN;

// **** Flow Sensor Stuff ****
volatile int flowCounter; //measuring the rising edges of the signal
const int flowSensorPin = 21; //The pin location of the sensor
int calc_lpm; // Liters Per Minute
int calc_gpm; // Gallons Per Minute

// Test/Reset Button
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

//  General Alarm State
int errorState = 0;
int errorOverTemp = 1;
int errorTempGetFail = 2;
int errorLowTankPumpPressure = 3;
int errorHighTankPumpPressure = 4;
int errorLowTankPumpFlow = 5;
int errorHighTankPumpFlow = 6;
int errorLowHVACPumpPressure = 7;
int errorHighHVACPumpPressure = 8;
int errorLowHVACPumpFlow = 9;
int errorHighHVACPumpFlow = 10;
int errorTestButton = 11;


// **** Start OneWire and Dallas Temp ****
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature.
float lastTemperature[MAX_ATTACHED_DS18B20]; //?????
int numSensors = 0; //?????
bool receivedConfig = false;
bool metric = false;


// Sensor Settings
//  const char blarg = "hi buddy!";

// Define Child ID's for MySensors GW
const int Number_Child_IDs           =   15; // Number of Child ID's
const int Number_Temp_Sensors        =    4; // Panel Temp Child ID
const int Solar_Panel_Temp_ID        =    0; // Panel Temp Child ID
const int Tank_Temp_ID               =    1; // Tank Temp Child ID
const int Shop_Temp_ID               =    2; // Shop Temp Child ID
const int Attic_Temp_ID              =    3; // Panel Pump Flow Child ID
const int Tank_Pump_ID               =   10; // Panel Pump Child ID
const int Tank_Pump_Pressure_ID      =   11; // Panel Pump Pressure Child ID
const int Tank_Pump_Flow_ID          =   12; // Panel Pump Flow Child ID
const int Attic_Fan_ID               =   13; // Attic Fan on/off
const int Attic_Louver_ID            =   14; // Attic Louver open/closed
const int Tank_Heater_ID             =   15; // Storage Tank Heater on/off
const int HVAC_Fan_Pump_ID           =   20; // Pump and Fan for HVAC
const int General_Alarm_ID           =   30; // Alarms to Vera and Local Buzzer
const int Test_Button_ID             =   31; // Test Button
const int HVAC_Set_Point_ID          =   32; // Pot to Control Shop Temp (controls HVAV Pump and Fan)
const int Motion_Sensor_ID           =   33; // Moton Sensor for Alarm

// Set Current Sensor Readings to Zero or Not Found
float tankTemp = -127.0;
float currentTankTemp = -127.0;
float panelTemp = -127.0;
float currentPanelTemp = -127.0;
float shopTemp = -127.0;
float currentShopTemp = -127.0;
float atticTemp = -127.0;
float currentAtticTemp = -127.0;
float tankPumpFlow = 0.0;
//  float currentTankPumpFlow = 0.0;
float tankPumpPressure = 0.0;
float currentTankPumpPressure = 0.0;
float hvacSetPoint = 0.0;
float tankPumpFlowLPM = 0.0;
float tankPumpFlowGPM = 0.0;
float currentTankPumpFlowGPM = 0.0;

// Zero out variables used with LCD
int intPanelTemp = 0;
int intTankTemp = 0;
int intShopTemp = 0;
int intTankPumpPressure = 0;
int intAtticTemp = 0;

// Set Device Status as Off
bool tankPump = false;
bool tankHeater = false;
bool attac_Fan = false;
bool attic_Louver = false;
bool atticFanStatus = false;
bool atticLouverStatus = false;
bool generalAlarmStatus = false;
bool hvacFanPumpStatus = false;
bool motonSensorStatus = false;
bool tempGetFail = false;

// **** Hex Addresses of Dallas Temp Sensors ****
DeviceAddress Solar_Panel_Temp_Addr = {0x28, 0x30, 0x12, 0x29, 0x07, 0x00, 0x00, 0x95};
DeviceAddress Tank_Temp_Addr =        {0x28, 0x45, 0xA3, 0x1C, 0x07, 0x00, 0x00, 0xAD};
DeviceAddress Shop_Temp_Addr =        {0x28, 0xB8, 0x3D, 0x29, 0x07, 0x00, 0x00, 0x49};
DeviceAddress Attic_Temp_Addr =       {0x28, 0x9B, 0x44, 0x1D, 0x07, 0x00, 0x00, 0x7E};

/* 4 extra Temp Sensors

    Device A Address: 2880EA290700000A
    Device B Address: 283AAC2A0700003F
    Device C Address: 28D15E2907000043
    Device D Address: 28374929070000F5

*/
// **** Initialize sensor message to MySensors Gateway ****
MyMessage msg_solar_panel_temp(Solar_Panel_Temp_ID, V_TEMP);
MyMessage msg_tank_temp(Tank_Temp_ID, V_TEMP);
MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
MyMessage msg_attic_temp(Attic_Temp_ID , V_TEMP);
MyMessage msg_tank_pump(Tank_Pump_ID, V_STATUS);
MyMessage msg_tank_heater(Tank_Heater_ID, V_STATUS);
MyMessage msg_tank_pump_pressure(Tank_Pump_Pressure_ID, V_PRESSURE);
MyMessage msg_tank_pump_flow(Tank_Pump_Flow_ID, V_FLOW);
MyMessage msg_attic_fan(Attic_Fan_ID , V_STATUS);
MyMessage msg_attic_louver(Attic_Louver_ID, V_STATUS);
MyMessage msg_general_alarm(General_Alarm_ID, V_STATUS);
MyMessage msg_test_button(Test_Button_ID, V_STATUS);
MyMessage msg_HVAC_Fan_Pump(HVAC_Fan_Pump_ID, V_STATUS);
MyMessage msg_set_temp_pot_heat(HVAC_Set_Point_ID, V_HVAC_SETPOINT_HEAT);
MyMessage msg_set_temp_pot_cool(HVAC_Set_Point_ID, V_HVAC_SETPOINT_COOL);
MyMessage msg_motion_sensor(Motion_Sensor_ID, V_TRIPPED);


//************** Start of void before *****************
void before() // Who Calls This ???????????
{
  //  TRANSPORT_DEBUG(PSTR("BLARG:B\n"));
  // Startup up the OneWire library
  sensors.begin();

  // set the resolution to tempResolution (Can be 9 to 12 bits .. lower is faster)
  const int tempResolution = 9;
  sensors.setResolution(Solar_Panel_Temp_Addr, tempResolution);
  sensors.setResolution(Tank_Temp_Addr, tempResolution);
  sensors.setResolution(Shop_Temp_Addr, tempResolution);
  sensors.setResolution(Attic_Temp_Addr, tempResolution);
}
// **** End of void before ****

// **** 2 line LCD Stuff ****
// Initialize display. Google the correct settings for your display.
// The follwoing setting should work for the recommended display in the MySensors "shop".
//   LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal lcd(10, 11, 12, 13, A0, A1, A2);

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// **** Start of Void Setup ****
void setup()
{
  //  TRANSPORT_DEBUG(PSTR("BLARG:S\n"));
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  setupPins();
  lcd.begin(16, 2);
  lcd.clear(); // clear and cursor in upper left corner
  lcd.setCursor(0, 0);
  lcd.print("Hello, World");
  lcd.setCursor(0, 1);
  lcd.print("Hello, me");// Flow Sensor Setup

  // Test/Reset Button
  pinMode(buttonPin, INPUT);

  // Flow Sensor Setup
  pinMode(flowSensorPin, INPUT); //initializes digital pin 21 ON Mega as an input
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached

}

// **** End of Void Setup ****
void setupPins()
{
  // Digital
  pinMode(tankPumpPin, OUTPUT);
  digitalWrite(tankPumpPin, HIGH);
  pinMode(tank_Heater_Pin, OUTPUT);
  digitalWrite(tank_Heater_Pin, HIGH);
  pinMode(HVAC_Fan_Pump_Pin, OUTPUT);
  digitalWrite(HVAC_Fan_Pump_Pin, HIGH);
  pinMode(attic_Fan_Pin, OUTPUT);
  digitalWrite(attic_Fan_Pin, HIGH);
  pinMode(attic_Louver_Pin, OUTPUT);
  digitalWrite(attic_Louver_Pin, HIGH);
  pinMode(tank_Pump_Flow_Pin, INPUT);
  pinMode(general_Alarm_Pin, OUTPUT);
  digitalWrite(general_Alarm_Pin, LOW);
  pinMode(buttonPin, INPUT);
  pinMode(motion_Sensor_Pin, INPUT);
  // Analog
  pinMode(tank_Pump_Pressure_Pin, INPUT);
  pinMode(set_Temp_Pot_Pin, INPUT);
}
// For IC2 Display
/*void setupLCD()
  {
  // LCD Setup
  Serial.println("We made it to LCD Setup");
  lcd.begin (16,2); // <<-- our LCD is a 16x2, change for your LCD if needed
  lcd.clear (); // go home on LCD
  }
*/
// **** End of Void Setup ****

// **** Start of Presentation ****
void presentation()
{
  //  TRANSPORT_DEBUG(PSTR("BLARG:P\n"));

  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("SolarHeaterControllerV4-2-2", "RW/CW 1/12/17");

  // Fetch the number of attached temperature sensors
  numSensors = Number_Temp_Sensors;

  // Present all sensors to controller
  // Array for storing temps
  present(Solar_Panel_Temp_ID, S_TEMP, "PanelTemp");            // Panel Temp Child ID 0
  delay(100);
  present(Tank_Temp_ID, S_TEMP, "TankTemp");                    // Tank Temp Child ID 1
  delay(100);
  present(Shop_Temp_ID, S_TEMP, "ShopTemp");                    // Shop Temp Child ID 2
  delay(100);
  present(Attic_Temp_ID, S_TEMP, "AtticTemp");                  // Attic Temp Child ID 3
  delay(100);
  present(Tank_Heater_ID, S_BINARY, "TankHeater");              // Tank Heater Child ID 15
  delay(100);
  present(Tank_Pump_ID, S_BINARY, "TankPump");                  // Tank Pump Child ID 10
  delay(100);
  present(Tank_Pump_Pressure_ID, S_CUSTOM, "PumpPress");        // Panel Pump Pressure Child ID 4
  delay(100);
  present(Tank_Pump_Flow_ID, S_WATER, "PumpFlow");              // Panel Pump Flow Child ID 6
  delay(100);
  present(Attic_Fan_ID, S_BINARY, "AtticFan");                  // Attic Fan Child ID 13
  delay(100);
  present(Attic_Louver_ID, S_BINARY, "AtticLouver");            // Attic Louver Child ID 14
  delay(100);
  present(General_Alarm_ID, S_BINARY, "GeneralAlarm");         // General Alarm Over Heat and Test Button
  delay(100);
  present(Test_Button_ID, S_BINARY, "TestButton");             // Test Button Sets off the General Alarm
  delay(100);
  present(HVAC_Set_Point_ID, S_HEATER, "TempSetPoint");
  delay(100);
  present(Motion_Sensor_ID, S_MOTION, "Motion Sensor");


}
// **** End Of Presentation ****

// **** Routine to Compute RPM of Flow Sensor ****
// This function measures the rising and falling edge of the hall effect sensors signal
void rpm () //This is the function that the interupt calls
{
  flowCounter++;
}

// **** Start of void loop ****
void loop()
{
  Serial.println();
  Serial.print("Number of Devices found on bus = ");
  Serial.println(sensors.getDeviceCount());
  Serial.print("Getting temperatures... ");
  Serial.println();

  // **** Tell all Dallas Sensors to take their Temperature ****
  sensors.requestTemperatures();

  // Zero Flow Counter and Enable Interrupts
  flowCounter = 0; //Set NbTops to 0 ready for calculations
  sei(); //Enables interrupts

  delay(1000); // Wait 1 second for both Flow and Temp Conversion
  // Disable Interrupts Get Count Over the Passed 1 sec and Compute Flow
  cli(); //Disable interrupts
  tankPumpFlowLPM = (float(flowCounter) * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour
  tankPumpFlowGPM = (tankPumpFlowLPM * .26);
  currentTankPumpFlowGPM = (tankPumpFlowGPM, 2);
  Serial.println ();
  Serial.print ("Flow Counter= ");
  Serial.println (flowCounter);
  Serial.print ("Tank Pump Flow LPM= ");
  Serial.println (tankPumpFlowLPM, DEC); // prints the number calculated above
  Serial.print ("Gallons Per Min= ");
  Serial.println (currentTankPumpFlowGPM, DEC);
  Serial.println ("");

  Serial.print("Solar Panel Temperature= ");
  // Fetch Temperature for Solar Panel by Sensor Address
  if (tempGetFail = false)
  {
    panelTemp =  printTemperature(sensors, Solar_Panel_Temp_Addr);
    currentPanelTemp = panelTemp;
    Serial.print("Panel Temp=");
    Serial.println(panelTemp, 1);
    Serial.println();
    // Send in the new Solar_Panel_Temp
    send(msg_solar_panel_temp.setSensor(Solar_Panel_Temp_ID).set(panelTemp, 1));
  }

  Serial.print("Storage Tank Temperature is:   ");
  // Fetch Temperature for Solar Panel by Sensor Address
  tankTemp = printTemperature(sensors, Tank_Temp_Addr);
  if (tempGetFail = false)
  {
    currentTankTemp = tankTemp;
    Serial.print("Tank Temp=");
    Serial.println(tankTemp, 1);
    Serial.println();
    // Send in the new Tank_Temp
    send(msg_tank_temp.setSensor(Tank_Temp_ID).set(tankTemp, 1));
  }

  Serial.print("Shop Temperature is:   ");
  // Fetch Temperature for Solar Panel by Sensor Address
  shopTemp = printTemperature(sensors, Shop_Temp_Addr);
  if (tempGetFail = false)
  {
    currentShopTemp = shopTemp;
    Serial.print("Shop Temp=");
    Serial.println(shopTemp, 1);
    Serial.println();
    // Send in the new Shop_Temp
    send(msg_shop_temp.setSensor(Shop_Temp_ID).set(shopTemp, 1));
  }
  Serial.print("Attic Temperature is:   ");
  // Fetch Temperature for Attic by Sensor Address
  atticTemp = printTemperature(sensors, Attic_Temp_Addr);
  if (tempGetFail == false)
  {
    currentAtticTemp = atticTemp;
    Serial.print("Attic Temp=");
    Serial.println(atticTemp, 1);
    Serial.println();
    // Send GW the new Attic_Temp
    Serial.println();
    send(msg_attic_temp.setSensor(Attic_Temp_ID).set(atticTemp, 1));
  }

  // Go see if the panel pump needs to get turned on and if it is is there any water pressure and flow?
  readTankPumpPressure(); // Get Tank Pump Pressure
  currentTankPumpPressure = tankPumpPressure;
  send(msg_tank_pump_pressure.setSensor(Tank_Pump_Pressure_ID).set(currentTankPumpPressure, 1));
  send(msg_tank_pump_flow.setSensor(Tank_Pump_Flow_ID).set(currentTankPumpFlowGPM, 1));
  processTankPump();
  writeLCD();  // Send results to LCD Dispaly
  Test_Button ();
  processAlarm();
}
//******************** End Of Void Loop ************************

// All Relays and LED's are Active LOW
// Need to add tests for pressure and flow??????

// **** Start of processTankPump ****
void processTankPump()
{
  systemDifference = currentPanelTemp - currentTankTemp;

  if (currentTankTemp > systemOverheat) {
    Serial.println("System Over Temp and PanelPump: Off!");
    digitalWrite(tankPumpPin, HIGH);
    tankPump = false;
    errorState = errorOverTemp;
    return;
  }
  if (systemDifference > systemDiffOn && tankPump == false) {
    digitalWrite(tankPumpPin, LOW);
    Serial.println("Tank Pump: On!");
    tankPump = true;
  }
  else if (systemDifference < systemDiffOff && tankPump == true) {
    digitalWrite(tankPumpPin, HIGH);
    Serial.println("tankPump: Off!");
    tankPump = false;
  }
}
// **** End of processTankPump ****

/*-----( Declare User-written Functions )-----*/

//**** Start of printTemperature ****
float printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
  float tempF = sensors.getTempF(deviceAddress);
  if (tempF <= -50.00)
  {
    Serial.print("Error getting temperature  ");
    tempGetFail = true;
  }
  else
  {
    Serial.print("F: ");
    Serial.print(tempF);
    return (tempF);
  }
}
// ***** End printTemperature ********

// Panel Pump Pressure Sensor Check to see if it's changed
void readTankPumpPressure()
{
  // if after flow startup timer has expired and the flow is still to low do something
  int sensorValue = analogRead(tank_Pump_Pressure_Pin);    // read the input pin Value range 0-1023
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.print("Voltage= ");
  Serial.println(voltage, 1);
  tankPumpPressure = ((voltage - 111) / 7); // Should be in PSI
  currentTankPumpPressure = tankPumpPressure;
  Serial.print("Pump_Pressure= ");
  Serial.print(tankPumpPressure, 1);
  Serial.println(" PSI"); //

  //  return pumpPressure;

  // If Pressure has changed send in the the new Pump Pressure
  //    send(msg.setSensor(numSensors+2).set(pumpPressure,1));

  // if pressure is <20 PSI shut pump off and set an alarm
}

//OUTPUT STUFFS
void writeLCD()
{
  Serial.println ("We made it to writeLCD");
  // Convert sensor flots to int and display
  lcd.clear();
  lcd.setCursor (0, 0); // go to start of 1st line
  lcd.print("P");
  lcd.setCursor (1, 0); //
  lcd.print(int(currentPanelTemp));

  lcd.setCursor (4, 0); //
  lcd.print("T");
  lcd.setCursor (5, 0); //
  lcd.print(int(currentTankTemp));

  lcd.setCursor (8, 0); //
  lcd.print("S");
  lcd.setCursor (9, 0); //
  lcd.print(int(currentShopTemp));

  lcd.setCursor (12, 0); //
  lcd.print("A");
  lcd.setCursor (13, 0); //
  lcd.print(int(currentShopTemp));

  lcd.setCursor (0, 1); //
  lcd.print("PP");
  lcd.setCursor (2, 1); //
  lcd.print(int(currentTankPumpPressure));

  lcd.setCursor (4, 1);
  lcd.print ("F");
  lcd.setCursor (5, 1);
  lcd.print(currentTankPumpFlowGPM, 1);
}

void processAlarm ()
{
  String errorString = String("");
  if (errorState = 0)
  {
    Serial.println("No Problems Here");
  }
  //
  else if (errorState = 1)
  {
    //    errorOverTemp
    Serial.println("Storage Tank Over Heat");
    errorString = ("Over Temp");
  }
  else if (errorState = 2)
  {
    //    errorTempGetFail
    Serial.println("Failed to Get Temp Reading");
    errorString = ("Temp Read Error");
  }
  else if (errorState = 3)
  {
    //    errorLowTankPumpPressure
    Serial.println("Low Tank Pump Pressure");
    errorString = ("Low Tank Pump Press");
  }
  else if (errorState = 4)
  {
    //    errorHighTankPumpPressure
    Serial.println("High Tank Pump Pressure");
    errorString = ("Hi Tank Pump Press");
  }
  else if (errorState = 5)
  {
    //    errorLowTankPumpFlow
    Serial.println("Low Tank Pump Flow");
    errorString = ("Low Tank Pump Press");
  }
  else if (errorState = 6)
  {
    //    errorHighTankPumpFlow
    Serial.println("High Tank Pump Flow");
    errorString = ("High Tank Pump Press");
  }
  else if (errorState = 7)
  {
    //    errorLowHVACPumpPressure
    Serial.println("Low HVAC Pump Pressure");
    errorString = ("Low Tank Pump Press");
  }
  else if (errorState = 8)
  {
    //    errorHighHVACPumpPressure
    Serial.println("High HVAC Pump Pressure");
    errorString = ("high Tank Pump Press");
  }
  else if (errorState = 9)
  {
    //    errorLowHVACPumpFlow
    Serial.println("Low HVAC Pump Flow");
    errorString = ("Low HVAC Pump Press");
  }
  else if (errorState = 10)
  {
    //    errorHighHVACPumpFlow
    Serial.println("High HVAC Pump Flow");
    errorString = ("High HVAC Pump Flow");
  }
  else if (errorState = 11)
  {
    //    errorTestButton
    Serial.println("Test Button Pressed");
    errorString = ("Test Button Pressed");
  }
  else
  {
    Serial.println("No Problems Here Boss");
    errorState = 0;
  }
  alarm (errorString);
}
//  General Alarm State
/*
  //  General Alarm State
  int errorState = 0;
  int errorOverTemp = 1;
  int errorTempGetFail = 2;
  int errorLowTankPumpPressure = 3;
  int errorHighTankPumpPressure = 4;
  int errorLowTankPumpFlow = 5;
  int errorHighTankPumpFlow =6;
  int errorLowHVACPumpPressure = 7;
  int errorHighHVACPumpPressure = 8;
  int errorLowHVACPumpFlow = 9;
  int errorHighHVACPumpFlow = 10;
  int errorTestButton = 11;
*/

void Test_Button ()
{
  // Test Button Will Set off Alarm
  Serial.println("Start of Test Button");
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == HIGH)
      {
        errorState = 11;
      }
      else
      {
        errorState = 0;
      }
    }
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastButtonState = reading;
  }
}
void alarm(String errorString)
{
  if (errorState == 0)  // Turn off buzzer and return
  {
    digitalWrite(general_Alarm_Pin, LOW);
    generalAlarmStatus = false;
  }
  else
  {
    generalAlarmStatus = true;
    digitalWrite(general_Alarm_Pin, HIGH);
    Serial.println("There is an Error Somewhere Fix It!");
    //   send(msg_general_alarm.setSensor(General_Alarm_ID).set(errorString, 1));
  }
}


//*********( THE END )***********

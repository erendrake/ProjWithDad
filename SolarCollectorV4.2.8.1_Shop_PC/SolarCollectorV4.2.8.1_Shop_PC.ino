/**
  "SolarHeaterControllerV4.2.8.1", "RW/CW 1/22/17" From Shop PC
   Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
   http://www.mysensors.org/build/temp
*/

// Running on Mega 1280
// Enable debug prints to serial monitor
#define MY_DEBUG
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
#define COMPARE_TEMP 1              // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 24             // Pin where dallase sensors are connected 
#define MAX_ATTACHED_DS18B20 16
#include <LiquidCrystal.h>
#include "config.h"

//Temperature Thresholds
const int systemDiffOn = SYSTEM_DIFF_ON;
const int systemDiffOff = SYSTEM_DIFF_OFF;
const int systemOverheat = SYSTEM_OVERHEAT;
float systemDifference = - 20.0;


// **** Timers ****
unsigned long minimumPumpOnMilliSec = 120000;
unsigned long tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;
unsigned long minimumFanOnMs = 60000;
unsigned long atticFanDelayMs = 10000;
unsigned long loopSleepTimerMs = 10000;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// Test/Reset Button
int buttonState;                              // the current reading from the input pin
int lastButtonState = LOW;                    // the previous reading from the input pin


// **** Start OneWire and Dallas Temp ****
OneWire oneWire(ONE_WIRE_BUS);                // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);          // Pass the oneWire reference to Dallas Temperature.
float lastTemperature[MAX_ATTACHED_DS18B20];  //?????
byte numSensors = 0; //?????
bool receivedConfig = false;
bool metric = false;

// **** Start of void before ****
void before() // Who Calls This ???????????
{
  //  TRANSPORT_DEBUG(PSTR("BLARG:B\n"));
  // Startup up the OneWire library
  sensors.begin();
  // set the resolution to tempResolution (Can be 9 to 12 bits .. lower is faster)
  const byte tempResolution = 9;
  sensors.setResolution(Solar_Panel_Temp_Addr, tempResolution);
  sensors.setResolution(Tank_Temp_Addr, tempResolution);
  sensors.setResolution(Shop_Temp_Addr, tempResolution);
  sensors.setResolution(Attic_Temp_Addr, tempResolution);
}

// **** Initialize 2 Line Display ****
LiquidCrystal lcd(10, 11, 12, 13, A0, A1, A2);

// Define Child ID's for MySensors GW
const byte Number_Child_IDs           =   15; // Number of Child ID's
const byte Number_Temp_Sensors        =    4; // Panel Temp Child ID
const byte Solar_Panel_Temp_ID        =    0; // Panel Temp Child ID
const byte Tank_Temp_ID               =    1; // Tank Temp Child ID
const byte Shop_Temp_ID               =    2; // Shop Temp Child ID
const byte Attic_Temp_ID              =    3; // Panel Pump Flow Child ID
const byte Tank_Pump_ID               =   10; // Panel Pump Child ID
const byte Tank_Pump_Pressure_ID      =   11; // Panel Pump Pressure Child ID
const byte Tank_Pump_Flow_ID          =   12; // Panel Pump Flow Child ID
const byte Attic_Fan_ID               =   13; // Attic Fan on/off
const byte Attic_Louver_ID            =   14; // Attic Louver open/closed
const byte Tank_Heater_ID             =   15; // Storage Tank Heater on/off
const byte HVAC_Fan_Pump_ID           =   20; // Pump and Fan for HVAC
const byte General_Alarm_ID           =   30; // Alarms to Vera and Local Buzzer
const byte Test_Button_ID             =   31; // Test Button
const byte HVAC_Set_Point_ID          =   32; // Pot to Control Shop Temp (controls HVAV Pump and Fan)
const byte Motion_Sensor_ID           =   33; // Moton Sensor for Alarm

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

// **** Start of Void Setup ****
void setup()
{
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(true);
  setupPins();
  lcd.begin(16, 2);
  lcd.clear(); // clear and cursor in upper left corner
  lcd.setCursor(0, 0);
  lcd.print("Hello, World");
  lcd.setCursor(0, 1);
  lcd.print("Hello, me");
  // Flow Sensor Setup
  attachInterrupt(2, rpm, RISING);                        // The Interrupt is Attached to Mega Pin Pin 21 Interrupt 2 
}

// **** Start of Presentation ****
void presentation()
{
  sendSketchInfo("SolarHeaterControllerV4.2.8.1", "RW/CW 1/22/17"); // From Shop PC
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
  Serial.print("Number of Dallas Devices found on bus = ");
  Serial.println(sensors.getDeviceCount());
  Serial.print("Requsting temperatures... ");
  Serial.println();

  // Get Thermostat Setting
  thermostatSetting();                        // Go See Where the Thermostat is Set

  // **** Tell all Dallas Sensors to take their Temperature ****
  sensors.requestTemperatures();

  // Zero Flow Counter and Enable Interrupts
  flowCounter = 0; //Set NbTops to 0 ready for calculations
  sei(); //Enables Interrupts If Water is Running Start Counting

  wait(1000);                                // Wait 1 second for both Flow and Temp Conversion. Wait is Non Blocking According to MySensors

  cli();              // Disable Interrupts Stop Counting and Get Count Over the Passed 1 sec and Compute Flow

  // **** Go Get Temperatures Sensor Readings ****

  // Solar Panel
  Serial.print("Solar Panel Temperature= ");
  panelTemp =  printTemperature(sensors, Solar_Panel_Temp_Addr);
  if (tempGetFail == false)
  {
    currentPanelTemp = panelTemp;
    // Send in the new Solar_Panel_Temp
    send(msg_solar_panel_temp.setSensor(Solar_Panel_Temp_ID).set(panelTemp, 1));
    Serial.println ("");
  wait(500);
  }

  // Tank
  Serial.print("Storage Tank Temperature is: ");
  tankTemp = printTemperature(sensors, Tank_Temp_Addr);
  if (tempGetFail == false)
  {
    currentTankTemp = tankTemp;
    // Send in the new Tank_Temp
    send(msg_tank_temp.setSensor(Tank_Temp_ID).set(tankTemp, 1));
    Serial.println ("");
    wait(500);
  }

  // Shop
  Serial.print("Shop Temperature is: ");
  shopTemp = printTemperature(sensors, Shop_Temp_Addr);
  if (tempGetFail == false)
  {
    currentShopTemp = shopTemp;
    // Send in the new Shop_Temp
    send(msg_shop_temp.setSensor(Shop_Temp_ID).set(shopTemp, 1));
    Serial.println ("");
    wait(500);
  }

  // Attic
  Serial.print("Attic Temperature is: ");
  atticTemp = printTemperature(sensors, Attic_Temp_Addr);
  Serial.print("F: ");
  Serial.println(tempF);

  if (tempGetFail == false)
  {
    currentAtticTemp = atticTemp;
    // Send GW the new Attic_Temp
    send(msg_attic_temp.setSensor(Attic_Temp_ID).set(atticTemp, 1));
    Serial.println ("");
    wait(500);
  }
  else
  {
    atticTemp = -127.0;
  }

  readTankPumpFlow();                         // Compute Tank Pump Flow 
  readTankPumpPressure();                     // Go see if the panel pump needs to get turned                    // Get Tank Pump Pressure
  processTankPump();                          // Pump Water Through the Solar Panel
  processAtticFan();                          //
  Serial.print("Attic Fan is ");
  Serial.println(atticFanStatus);
  Serial.print("Attic Louvers are ");
  Serial.println(atticLouverStatus);
  processHVACPumpFan();                       // Pump Water Through the HVAC Heat Exchanger and Turn on the HVAC Fan
  processTankHeater();                        // Turn On Tank Heater if Shop Gets To Cold
  writeLCD();                                 // Send results to LCD Dispaly
  Test_Button ();
  processAlarm();
  wait(30000);
  // Go Do It All Again
}

// **** ( THE END ) ****

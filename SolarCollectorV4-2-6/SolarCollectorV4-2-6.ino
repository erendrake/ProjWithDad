/**
  "SolarHeaterControllerV4-2-6", "RW/CW 1/19/17"
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
const int minimumPumpOnMilliSec = (MINIMUM_PUMP_ON_SECONDS * 1000);
const int tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;
const int minimumFanOnMs = 60000;
const int fanDelayMs = 10000;
const int loopSleepTimerMs = 10000;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// Test/Reset Button
int buttonState;                              // the current reading from the input pin
int lastButtonState = LOW;                    // the previous reading from the input pin


// **** Start OneWire and Dallas Temp ****
OneWire oneWire(ONE_WIRE_BUS);                // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);          // Pass the oneWire reference to Dallas Temperature.
float lastTemperature[MAX_ATTACHED_DS18B20];  //?????
int numSensors = 0; //?????
bool receivedConfig = false;
bool metric = false;

// **** Start of void before ****
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

// **** 2 line LCD Stuff ****
// Initialize display. Google the correct settings for your display.
// The follwoing setting should work for the recommended display in the MySensors "shop".
//   LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal lcd(10, 11, 12, 13, A0, A1, A2);

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
  sensors.setWaitForConversion(false);
  setupPins();
  lcd.begin(16, 2);
  lcd.clear(); // clear and cursor in upper left corner
  lcd.setCursor(0, 0);
  lcd.print("Hello, World");
  lcd.setCursor(0, 1);
  lcd.print("Hello, me");// Flow Sensor Setup
  // Flow Sensor Setup
  attachInterrupt(0, rpm, RISING);            //and the interrupt is attached
}
// MySensors Defines

//  define_child_ids();
//  messages();
//  present_sensors();

// **** Start of Presentation ****
void presentation()
{
  sendSketchInfo("SolarHeaterControllerV4-2-6", "RW/CW 1/19/17");
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
  Serial.print("Number of Devices found on bus = ");
  Serial.println(sensors.getDeviceCount());
  Serial.print("Getting temperatures... ");
  Serial.println();

  thermostatSetting();                        // Go See Where the Thermostat is Set

  // **** Tell all Dallas Sensors to take their Temperature ****
  sensors.requestTemperatures();

  // Zero Flow Counter and Enable Interrupts
  flowCounter = 0; //Set NbTops to 0 ready for calculations
  sei(); //Enables interrupts                 // If Water is Running Start Counting

  delay(1000);                                // Wait 1 second for both Flow and Temp Conversion

  // Disable Interrupts Stop Counting and Get Count Over the Passed 1 sec and Compute Flow
  cli();
  Serial.print("Flow Counter=");
  Serial.println(flowCounter);
  tankPumpFlowLPM = (float(flowCounter) * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour
  tankPumpFlowGPM = (tankPumpFlowLPM * .26);
  currentTankPumpFlowGPM = (tankPumpFlowGPM, 2);
  Serial.print ("Tank Pump Flow LPM= ");
  Serial.println (tankPumpFlowLPM, DEC);      // prints the number calculated above
  Serial.print ("Gallons Per Min= ");
  Serial.println (currentTankPumpFlowGPM, DEC);
  Serial.println ();

  Serial.print("Solar Panel Temperature= ");
  // Fetch Temperature for Solar Panel by Sensor Address
  panelTemp =  printTemperature(sensors, Solar_Panel_Temp_Addr);
  if (tempGetFail == false)
  {
    currentPanelTemp = panelTemp;
    Serial.print("Panel Temp=");
    Serial.println(panelTemp, 1);
    Serial.println();
    // Send in the new Solar_Panel_Temp
    send(msg_solar_panel_temp.setSensor(Solar_Panel_Temp_ID).set(panelTemp, 1));
  }

  Serial.print("Storage Tank Temperature is: ");
  // Fetch Temperature for Solar Panel by Sensor Address
  tankTemp = printTemperature(sensors, Tank_Temp_Addr);
  if (tempGetFail == false)
  {
    currentTankTemp = tankTemp;
    Serial.print("Tank Temp=");
    Serial.println(tankTemp, 1);
    Serial.println();
    // Send in the new Tank_Temp
    send(msg_tank_temp.setSensor(Tank_Temp_ID).set(tankTemp, 1));
  }

  Serial.print("Shop Temperature is: ");
  // Fetch Temperature for Solar Panel by Sensor Address
  shopTemp = printTemperature(sensors, Shop_Temp_Addr);
  if (tempGetFail == false)
  {
    currentShopTemp = shopTemp;
    Serial.print("Shop Temp=");
    Serial.println(shopTemp, 1);
    Serial.println();
    // Send in the new Shop_Temp
    send(msg_shop_temp.setSensor(Shop_Temp_ID).set(shopTemp, 1));
  }
  Serial.print("Attic Temperature is: ");
  // Fetch Temperature for Attic by Sensor Address
  atticTemp = printTemperature(sensors, Attic_Temp_Addr);
  if (tempGetFail == false)
  {
    currentAtticTemp = atticTemp;
    Serial.print("Attic Temp=");
    Serial.println(atticTemp, 1);
    Serial.println();
    // Send GW the new Attic_Temp
    send(msg_attic_temp.setSensor(Attic_Temp_ID).set(atticTemp, 1));
  }

  // Go see if the panel pump needs to get turned on and if it is is there any water pressure and flow?
  readTankPumpPressure();                     // Get Tank Pump Pressure
  currentTankPumpPressure = tankPumpPressure;
  processTankPump();                          // Pump Water Through the Solar Panel
  processHVACPumpFan();                       // Pump Water Through the HVAC Heat Exchanger and Turn on the HVAC Fan
  processTankHeater();                        // Turn On Tank Heater if Shop Gets To Cold
  writeLCD();                                 // Send results to LCD Dispaly
  Test_Button ();
  processAlarm();
  send(msg_tank_pump_pressure.setSensor(Tank_Pump_Pressure_ID).set(currentTankPumpPressure, 1));
  send(msg_tank_pump_flow.setSensor(Tank_Pump_Flow_ID).set(currentTankPumpFlowGPM, 1));
  // Go Do It All Again
}
// **** End Of Void Loop ****

/*-----( Declare User-written Functions )-----*/

// **** ( THE END ) ****

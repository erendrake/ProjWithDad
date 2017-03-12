/* Solar Project

  "Solar Collector V5.0.3", "RW/CW 3/11/17"
   Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
   http://www.mysensors.org/build/temp
*/
// "*" = Move to ESP8266 NodeMCU 1.0(ESP-12E Module) No Analog Ports
// Running on Mega 1280
// Enable debug prints to serial monitor
#define MY_DEBUG
#define MY_RADIO_NRF24                          // Enable and select radio type attached
#define MY_NODE_ID 25                           // Define Parent ID
#define MY_RF24_CE_PIN 40                       // * Pin Confuguration for Arduino Mega
#define MY_RF24_CS_PIN 53                       // *
#include <SPI.h>
#include <MySensors.h>
#include <avr/wdt.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#define COMPARE_TEMP 1                          // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 22                         // * Pin Where Dallase Sensors are Connected. Has 4.7K ohm Pullup.
#define MAX_ATTACHED_DS18B20 16
#include <LiquidCrystal.h>
#include "config.h"

// **** Start OneWire and Dallas Temp ****
OneWire oneWire(ONE_WIRE_BUS);                // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);          // Pass the oneWire reference to Dallas Temperature.
float lastTemperature[MAX_ATTACHED_DS18B20];  //?????
uint8_t numSensors = 0; //?????
bool receivedConfig = false;
bool metric = false;

// **** Start of void before ****
void before() // Who Calls This ???????????
{
  //  TRANSPORT_DEBUG(PSTR("BLARG:B\n"));
  // Startup up the OneWire library
  sensors.begin();
  // set the resolution to tempResolution (Can be 9 to 12 bits .. lower is faster)
  const uint8_t tempResolution = 9;
  sensors.setResolution(Solar_Panel_Temp_Addr, tempResolution);
  sensors.setResolution(Tank_Temp_Addr, tempResolution);
  sensors.setResolution(Shop_Temp_Addr, tempResolution);
  sensors.setResolution(Attic_Temp_Addr, tempResolution);
}

// **** Initialize 2 Line Display ****
LiquidCrystal lcd(10, 11, 12, 13, A0, A1, A2);// Plugged Into Uno Style Sensor Board

// Define Child ID's for MySensors GW
const uint8_t Number_Child_IDs           =   18; // Number of Child ID's
const uint8_t Number_Temp_Sensors        =    5; // Panel Temp Child ID
const uint8_t Solar_Panel_Temp_ID        =    0; // Panel Temp Child ID
const uint8_t Tank_Temp_ID               =    1; // Tank Temp Child ID
const uint8_t Shop_Temp_ID               =    2; // Shop Temp Child ID
const uint8_t Attic_Temp_ID              =    3; // Panel Pump Flow Child ID
const uint8_t Lower_Panel_Inlet_Temp_ID  =    4; // Solar Panel Lower Inlet Temp Sensor
const uint8_t Tank_Pump_ID               =   10; // Panel Pump Child ID
const uint8_t Tank_Pump_Pressure_ID      =   11; // Panel Pump Pressure Child ID
const uint8_t Tank_Pump_Flow_ID          =   12; // Panel Pump Flow Child ID
const uint8_t Attic_Fan_ID               =   13; // Attic Fan on/off
const uint8_t Attic_Louver_ID            =   14; // Attic Louver open/closed
const uint8_t Attic_Gable_Louver_ID      =   15; // Attic Gable Louver South End (for summer cooling)
const uint8_t Tank_Heater_ID             =   16; // Storage Tank Heater on/off
const uint8_t HVAC_Fan_Pump_ID           =   20; // Pump and Fan for HVAC
const uint8_t General_Alarm_ID           =   30; // Alarms to Vera and Local Buzzer
const uint8_t Test_Button_ID             =   31; // Test Button
const uint8_t HVAC_Set_Point_ID          =   32; // Pot to Control Shop Temp (controls HVAV Pump and Fan)
const uint8_t Motion_Sensor_ID           =   33; // Moton Sensor for Alarm
const uint8_t Summer_Winter_Mode_ID      =   41; // Summer Winter Mode Switch


// **** Initialize sensor message to MySensors Gateway ****
MyMessage msg_solar_panel_temp(Solar_Panel_Temp_ID, V_TEMP);
MyMessage msg_tank_temp(Tank_Temp_ID, V_TEMP);
MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
MyMessage msg_attic_temp(Attic_Temp_ID , V_TEMP);
MyMessage msg_lower_inlet_temp(Lower_Panel_Inlet_Temp_ID, V_TEMP);
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
//  MyMessage msg_shop_sup_heater(Shop_Sup_Heater_PID, V_HVAC_FLOW_STATE);
MyMessage msg_summer_wintermode(Summer_Winter_Mode_ID, V_STATUS);
//  MyMessage msg_Sup_Heater_Status(Shop_Sup_Heater_CID, V_STATUS);   //
//  heaterStatus.setDestination(Shop_Sup_Heater_PID);
//  MyMessage msg_Sup_Heater_Control(HEATER_CHILD_CONTROL_ID, V_STATUS);   //
//  heaterControl.setDestination(Shop_Sup_Heater_CID);


// **** Start of Void Setup ****
void setup()
{
  //  wdt_disable();                                                  //always good to disable WatchDogTimer, if it was left 'on' or you need init time
  //  Serial.begin(112500);
  Serial.println("Solar Collector V5.0.2, RW/CW 3/10/17");
  sensors.begin();
  Serial.println("Dallas Temperature Control Library - Async Demo");
  Serial.print("Library Version: ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.println("\n");
  // set the resolution to tempResolution (Can be 9 to 12 bits .. lower is faster)
  const uint8_t tempResolution = 9;
  sensors.setResolution(Solar_Panel_Temp_Addr, tempResolution);
  sensors.setResolution(Tank_Temp_Addr, tempResolution);
  sensors.setResolution(Shop_Temp_Addr, tempResolution);
  sensors.setResolution(Attic_Temp_Addr, tempResolution);

  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  setupPins();
  lcd.begin(16, 2);
  lcd.clear(); // clear and cursor in upper left corner
  lcd.setCursor(0, 0);
  lcd.print("Hello, World");
  lcd.setCursor(0, 1);
  lcd.print("Hello, me");
  //  Flow Sensor Setup
  attachInterrupt(2, rpm, RISING);                              // and the interrupt is attached
  //  wdt_enable(WDTO_8S);                                              //enable WatchDogTimer, and set it to 8s
}

// **** Start of Presentation ****
void presentation()
{
  sendSketchInfo("Solar Collector V5.0.3", "RW/CW 3/11/17", ack);
  // Fetch the number of attached temperature sensors
  numSensors = Number_Temp_Sensors;
  // Present all sensors to controller
  // Array for storing temps
  present(Solar_Panel_Temp_ID, S_TEMP, "PanelTemp", ack);                      // Panel Temp Child ID 0
  wait(100);
  present(Tank_Temp_ID, S_TEMP, "TankTemp", ack);                              // Tank Temp Child ID 1
  wait(100);
  present(Shop_Temp_ID, S_TEMP, "ShopTemp", ack);                              // Shop Temp Child ID 2
  wait(100);
  present(Attic_Temp_ID, S_TEMP, "AtticTemp", ack);                            // Attic Temp Child ID 3
  wait(100);
  present(Lower_Panel_Inlet_Temp_ID, S_TEMP, "LowerInletTemp", ack);           // Solar Panel Lower Inlet Temp
  wait(100);
  present(Tank_Heater_ID, S_BINARY, "TankHeater", ack);                        // Tank Heater Child ID 15
  wait(100);
  present(Tank_Pump_ID, S_BINARY, "TankPump", ack);                            // Tank Pump Child ID 10
  wait(100);
  present(Tank_Pump_Pressure_ID, S_CUSTOM, "PumpPress", ack);                  // Panel Pump Pressure Child ID 4
  wait(100);
  present(Tank_Pump_Flow_ID, S_WATER, "PumpFlow", ack);                        // Panel Pump Flow Child ID 6
  wait(100);
  present(Attic_Fan_ID, S_BINARY, "AtticFan", ack);                            // Attic Fan Child ID 13
  wait(100);
  present(Attic_Louver_ID, S_BINARY, "AtticLouver", ack);                      // Attic Louver Child ID 14
  wait(100);
  present(General_Alarm_ID, S_BINARY, "GeneralAlarm", ack);                    // General Alarm Over Heat and Test Button
  wait(100);
  present(Test_Button_ID, S_BINARY, "TestButton", ack);                        // Test Button Sets off the General Alarm
  wait(100);
  present(HVAC_Set_Point_ID, S_HEATER, "TempSetPoint", ack);                   // Thermostat Setting
  wait(100);
  present(HVAC_Fan_Pump_ID, S_HEATER, "HVAC Fan Pump", ack);                   // HVAC Fan Pump On/Off
  wait(100);
  present(Motion_Sensor_ID, S_MOTION, "Motion Sensor", ack);                   // Motion Sensor
  wait(100);
  //  present(Shop_Sup_Heater_Parent_ID, S_HVAC, "Supplemental Heater", ack);      // Shop Supplemental Electric Heater
  //  wait(100);
  present(Summer_Winter_Mode_ID, S_BINARY, "Summer/Winter Switch", ack);       // Summer/Winter Mode Switch
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
  heartBeatNo = 0.0;
  writeHeartBeatNoLCD();
  Serial.print("Number of Dallas Devices found on bus = ");
  Serial.println(sensors.getDeviceCount());

  // Is it Summer or Winter?
  heartBeatNo = 0.1;
  writeHeartBeatNoLCD();
  //  processSummerWinterMode();           // Read Winter Summer Mode Switch

  // Get Thermostat Setting
  heartBeatNo = 0.2;
  writeHeartBeatNoLCD();
  thermostatSetting();                        // Go See Where the Thermostat is Set

  // Tell all Dallas Sensors to take their Temperatures
  heartBeatNo = 0.3;
  writeHeartBeatNoLCD();
  sensors.requestTemperatures();

  // Zero Flow Counter and Enable Interrupts
  heartBeatNo = 0.4;
  writeHeartBeatNoLCD();
  flowCounter = 0; //Set NbTops to 0 ready for calculations
  sei(); //Enables Interrupts If Water is Running Start Counting


  // wait 1 Sec
  heartBeatNo = 0.5;
  writeHeartBeatNoLCD();

  wait(1000);                                // Wait 1 second for both Flow and Temp Conversion

  // Disable Interrupts
  heartBeatNo = 0.6;
  writeHeartBeatNoLCD();

  cli();              // Disable Interrupts Stop Counting and Get Count Over the Passed 1 sec and Compute Flow

  // **** Go Get Temperatures Sensor Readings ****
  /*
    Serial.print("Current Panel Temp ");
    Serial.println(currentPanelTemp, 1);
    Serial.print("Current Tank Temp ");
    Serial.println(currentTankTemp, 1);
    Serial.println(systemDifference, 1);
  */
  // Get Solar Panel Temp
  heartBeatNo = 1.0;
  writeHeartBeatNoLCD();
  Serial.print("Solar Panel Temperature= ");
  panelTemp =  printTemperature(sensors, Solar_Panel_Temp_Addr);
  if (tempGetFail == false)                                 // If sensor read error use last good read.
  {
    currentPanelTemp = panelTemp;
    Serial.print("Current Panel Temp ");
    Serial.println(currentPanelTemp, 1);
    // Send in the new Solar_Panel_Temp
    send(msg_solar_panel_temp.setSensor(Solar_Panel_Temp_ID).set(panelTemp, 1));
    wait(500);
  }
  else if (tempGetFail == true)
  {
    Serial.println("Bad Sensor Read");
  }
  //  Serial.print("Current Panel Temp ");
  //  Serial.println(currentPanelTemp, 1);

  //  Serial.println("");

  // Get Solar Panel Lower Inlet Temp
  heartBeatNo = 1.1;
  writeHeartBeatNoLCD();
  Serial.print("Solar Panel Lower Inlet Temperature= ");
  lowerInletTemp =  printTemperature(sensors, Lower_Inlet_Temp_Addr);
  if (tempGetFail == false)
  {
    currentLowerInletTemp = lowerInletTemp;
    // Send in the new Lower Inlet Temp
    send(msg_lower_inlet_temp.setSensor(Lower_Panel_Inlet_Temp_ID).set(lowerInletTemp, 1));
    wait(500);
  }
  else if (tempGetFail == true)
  {
    Serial.println("Bad Sensor Read");
  }
  Serial.print("Current Panel Temp ");
  Serial.println(currentPanelTemp, 1);

  // Get Tank Temp
  heartBeatNo = 1.2;
  writeHeartBeatNoLCD();
  Serial.print("Tank Temp= ");
  tankTemp = printTemperature(sensors, Tank_Temp_Addr);
  if (tempGetFail == false)
  {
    currentTankTemp = tankTemp;
    // Send in the new Tank_Temp
    send(msg_tank_temp.setSensor(Tank_Temp_ID).set(tankTemp, 1));
    wait(500);
  }

  Serial.print("Current Panel Temp ");
  Serial.println(currentPanelTemp, 1);
  Serial.print("Current Tank Temp ");
  Serial.println(currentTankTemp, 1);

  // Get Shop Temp
  heartBeatNo = 1.3;
  writeHeartBeatNoLCD();
  Serial.print("Shop Temperature is: ");
  shopTemp = printTemperature(sensors, Shop_Temp_Addr);
  if (tempGetFail == false)
  {
    currentShopTemp = shopTemp;
    // Send in the new Shop_Temp
    send(msg_shop_temp.setSensor(Shop_Temp_ID).set(shopTemp, 1));
    wait(500);
  }
  else if (tempGetFail == true)
  {
    Serial.println("Bad Sensor Read");
  }
  Serial.print("Current Panel Temp ");
  Serial.println(currentPanelTemp, 1);
  Serial.print("Current Tank Temp ");
  Serial.println(currentTankTemp, 1);

  // Tank Pump Flow
  heartBeatNo = 1.4;
  writeHeartBeatNoLCD();
  readTankPumpFlow();                         // Compute Tank Pump Flow

  // Read Tank Pressure
  heartBeatNo = 1.5;
  writeHeartBeatNoLCD();
  readTankPumpPressure();                     // Go see if the panel pump needs to get turned                    // Get Tank Pump Pressure

  // Process Tank Pump
  heartBeatNo = 1.6;
  writeHeartBeatNoLCD();
  processTankPump();                          // Pump Water Through the Solar Panel

  Serial.print("Current Panel Temp ");
  Serial.println(currentPanelTemp, 1);
  Serial.print("Current Tank Temp ");
  Serial.println(currentTankTemp, 1);

  // Get Attic Temp
  heartBeatNo = 1.7;
  writeHeartBeatNoLCD();
  Serial.print("Attic Temperature is: ");
  atticTemp = printTemperature(sensors, Attic_Temp_Addr);
  if (tempGetFail == false)
  {
    currentAtticTemp = atticTemp;
    // Send GW the new Attic_Temp
    send(msg_attic_temp.setSensor(Attic_Temp_ID).set(atticTemp, 1));
    wait(500);
  }
  else if (tempGetFail == true)
  {
    Serial.println("Bad Sensor Read");
  }

  // Write To LCD
  heartBeatNo = 2.0;
  writeHeartBeatNoLCD();
  writeLCD();

  // Coppula Fan
  heartBeatNo = 3.0;
  writeHeartBeatNoLCD();
  //    processCopulaFan();                         // Nothing there yet

  // Attic Fan and Louver
  heartBeatNo = 4.0;
  writeHeartBeatNoLCD();
  processAtticFan();

  // HVAC Fan and Pump
  processHVACPumpFan();                       // Pump Water Through the HVAC Heat Exchanger and Turn on the HVAC Fan
  heartBeatNo = 5.0;
  writeHeartBeatNoLCD();

  //    processTankHeater();                        // Turn On Tank Heater if Shop Gets To Cold
  // Nothing Here Yet

  /*  // Supplamental Electric Shop Heater
    heartBeatNo = 6.0;
    writeHeartBeatNoLCD();
    //    processShopSupHeater();                     // Do We Need to Turn On Extra Heat?

    // Test Button
    heartBeatNo = 7.0;
    writeHeartBeatNoLCD();
    //    Test_Button ();
    //  Serial.println("");

    // Process Any Alarms
    heartBeatNo = 8.0;
    writeHeartBeatNoLCD();
    //    processAlarm();
  */
  // 3 Sec Loop Deplay
  heartBeatNo = 9.0;
  writeHeartBeatNoLCD();

  // unsigned long loopSleepTimerMs = 3000;
  //  loopSleepTimerMs = millis();
  //  }
  //  if ((millis() - lastDebounceTime) > debounceDelay)
  wait(10000); // 10 Sec

  Serial.println("Go Do It All Again!");
  Serial.println("");
  // Go Do It All Again
}


// **** ( THE END ) ****

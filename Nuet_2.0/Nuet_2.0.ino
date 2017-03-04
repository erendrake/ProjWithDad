/* Solar Project

  "Solar Nuet V1.9", "RW/CW 2/25/18"
   Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
   http://www.mysensors.org/build/temp
*/
// "*" = Move to ESP8266 NodeMCU 1.0(ESP-12E Module) No Analog Ports
// Running on Mega 1280
// Enable debug prints to serial monitor
// #define MY_DEBUG
// #define MY_RADIO_NRF24                          // Enable and select radio type attached
// #define MY_NODE_ID 25                           // Define Parent ID
// #define MY_RF24_CE_PIN 40                       // * Pin Confuguration for Arduino Mega
// #define MY_RF24_CS_PIN 53                       // *
// #include <SPI.h>
// #include <MySensors.h>
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
/* void before() // Who Calls This ???????????
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
*/
// **** Initialize 2 Line Display ****
LiquidCrystal lcd(10, 11, 12, 13, A0, A1, A2);// Plugged Into Uno Style Sensor Board

/* // Define Child ID's for MySensors GW
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
  const uint8_t Shop_Sup_Heater_ID         =   40; // Suplamental Electric Heater
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
  MyMessage msg_shop_sup_heater(Shop_Sup_Heater_ID, V_HVAC_FLOW_STATE);
  MyMessage msg_summer_wintermode(Summer_Winter_Mode_ID, V_STATUS);
*/
// **** Start of Void Setup ****
void setup()
{
  Serial.begin(112500);
  Serial.println("Solar ESP-12E Module Nuet V1.9, RW/CW 2/25/17");
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
  // Flow Sensor Setup
  //  attachInterrupt(2, rpm, RISING);                              // and the interrupt is attached
}

/* // **** Start of Presentation ****
  void presentation()
  {
  sendSketchInfo("Solar Nuet V1.8 8266", "RW/CW 2/22/18", ack);
  // Fetch the number of attached temperature sensors
  numSensors = Number_Temp_Sensors;
  // Present all sensors to controller
  // Array for storing temps
  present(Solar_Panel_Temp_ID, S_TEMP, "PanelTemp", ack);                      // Panel Temp Child ID 0
  delay(100);
  present(Tank_Temp_ID, S_TEMP, "TankTemp", ack);                              // Tank Temp Child ID 1
  delay(100);
  present(Shop_Temp_ID, S_TEMP, "ShopTemp", ack);                              // Shop Temp Child ID 2
  delay(100);
  present(Attic_Temp_ID, S_TEMP, "AtticTemp", ack);                            // Attic Temp Child ID 3
  delay(100);
  present(Lower_Panel_Inlet_Temp_ID, S_TEMP, "LowerInletTemp", ack);           // Solar Panel Lower Inlet Temp
  delay(100);
  present(Tank_Heater_ID, S_BINARY, "TankHeater", ack);                        // Tank Heater Child ID 15
  delay(100);
  present(Tank_Pump_ID, S_BINARY, "TankPump", ack);                            // Tank Pump Child ID 10
  delay(100);
  present(Tank_Pump_Pressure_ID, S_CUSTOM, "PumpPress", ack);                  // Panel Pump Pressure Child ID 4
  delay(100);
  present(Tank_Pump_Flow_ID, S_WATER, "PumpFlow", ack);                        // Panel Pump Flow Child ID 6
  delay(100);
  present(Attic_Fan_ID, S_BINARY, "AtticFan", ack);                            // Attic Fan Child ID 13
  delay(100);
  present(Attic_Louver_ID, S_BINARY, "AtticLouver", ack);                      // Attic Louver Child ID 14
  delay(100);
  present(General_Alarm_ID, S_BINARY, "GeneralAlarm", ack);                    // General Alarm Over Heat and Test Button
  delay(100);
  present(Test_Button_ID, S_BINARY, "TestButton", ack);                        // Test Button Sets off the General Alarm
  delay(100);
  present(HVAC_Set_Point_ID, S_HEATER, "TempSetPoint", ack);                  // Thermostat Setting
  delay(100);
  present(Motion_Sensor_ID, S_MOTION, "Motion Sensor", ack);                   // Motion Sensor
  delay(100);
  present(Shop_Sup_Heater_ID, S_HVAC, "Supplemental Heater", ack);  // Shop Supplemental Electric Heater
  delay(100);
  present(Summer_Winter_Mode_ID, S_BINARY, "Summer/Winter Switch", ack);       // Summer/Winter Mode Switch
  }
*/
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
  writeHeartBeatNo();
  Serial.print("HeartBeat Start of Void Loop & Get Num of Sensors ");
  Serial.println(heartBeatNo, 1);

  Serial.print("Number of Dallas Devices found on bus = ");
  Serial.println(sensors.getDeviceCount());
  Serial.print("Requsting temperatures... ");
  Serial.println("");

  heartBeatNo = 0.1;
  writeHeartBeatNo();
  Serial.print("HeartBeat Summer/Winter ");
  Serial.println(heartBeatNo, 1);

  // Is it Summer or Winter?
  //  processSummerWinterMode();           // Read Winter Summer Mode Switch
  Serial.println("");

  heartBeatNo = 0.2;
  writeHeartBeatNo();
  Serial.print("HeartBeat Thermostat Setting ");
  Serial.println(heartBeatNo, 1);

  // Get Thermostat Setting
  thermostatSetting();                        // Go See Where the Thermostat is Set
  Serial.println("");

  heartBeatNo = 0.3;
  writeHeartBeatNo();
  Serial.print("HeartBeat Temp Request ");
  Serial.println(heartBeatNo, 1);

  // **** Tell all Dallas Sensors to take their Temperature ****
  sensors.requestTemperatures();
  Serial.println("");

  heartBeatNo = 0.4;
  writeHeartBeatNo();
  Serial.print("HeartBeat Enable Interrupts ");
  Serial.println(heartBeatNo, 1);

  // Zero Flow Counter and Enable Interrupts
  //  flowCounter = 0; //Set NbTops to 0 ready for calculations
  //  sei(); //Enables Interrupts If Water is Running Start Counting
  Serial.println("");

  heartBeatNo = 0.5;
  writeHeartBeatNo();
  Serial.print("HeartBeat 1 Sec Delay ");
  Serial.println(heartBeatNo, 1);

  delay(1000);                                // Wait 1 second for both Flow and Temp Conversion
  Serial.println("");

  heartBeatNo = 0.6;
  writeHeartBeatNo();
  Serial.print("HeartBeat Disbale Interrupts ");
  Serial.println(heartBeatNo, 1);

  //  cli();              // Disable Interrupts Stop Counting and Get Count Over the Passed 1 sec and Compute Flow
  Serial.println("");

  // **** Go Get Temperatures Sensor Readings ****

  heartBeatNo = 1.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Getting Panel Temp ");
  Serial.println(heartBeatNo, 1);

  // Solar Panel Temp
  Serial.print("Solar Panel Temperature= ");
  panelTemp =  printTemperature(sensors, Solar_Panel_Temp_Addr);
  if (tempGetFail == false)
  {
    currentPanelTemp = panelTemp;
    // Send in the new Solar_Panel_Temp
    //    send(msg_solar_panel_temp.setSensor(Solar_Panel_Temp_ID).set(panelTemp, 1));
    delay(500);
  }
  Serial.println("");

  heartBeatNo = 1.1;
  writeHeartBeatNo();
  Serial.print("HeartBeat Getting Lower Panel Temp ");
  Serial.println(heartBeatNo, 1);

  // Solar Panel Lower Inlet Temp
  Serial.print("Solar Panel Lower Inlet Temperature= ");
  lowerInletTemp =  printTemperature(sensors, Lower_Inlet_Temp_Addr);
  if (tempGetFail == false)
  {
    currentLowerInletTemp = lowerInletTemp;
    // Send in the new Lower Inlet Temp
    //    send(msg_lower_inlet_temp.setSensor(Lower_Panel_Inlet_Temp_ID).set(lowerInletTemp, 1));
    delay(500);
  }
  Serial.println("");

  heartBeatNo = 1.2;
  writeHeartBeatNo();
  Serial.print("HeartBeat Tank Temp ");
  Serial.println(heartBeatNo, 1);

  // Tank Temp
  Serial.print("Tank Temperature is: ");
  tankTemp = printTemperature(sensors, Tank_Temp_Addr);
  if (tempGetFail == false)
  {
    currentTankTemp = tankTemp;
    // Send in the new Tank_Temp
    //    send(msg_tank_temp.setSensor(Tank_Temp_ID).set(tankTemp, 1));
    delay(500);
  }
  Serial.println("");

  heartBeatNo = 1.3;
  writeHeartBeatNo();
  Serial.print("HeartBeat Shop Temp ");
  Serial.println(heartBeatNo, 1);

  // Shop Temp
  Serial.print("Shop Temperature is: ");
  shopTemp = printTemperature(sensors, Shop_Temp_Addr);
  if (tempGetFail == false)
  {
    currentShopTemp = shopTemp;
    // Send in the new Shop_Temp
    //    send(msg_shop_temp.setSensor(Shop_Temp_ID).set(shopTemp, 1));
    delay(500);
  }
  Serial.println("");

  heartBeatNo = 1.4;
  writeHeartBeatNo();
  Serial.print("HeartBeat Attic Temp ");
  Serial.println(heartBeatNo, 1);

  // Attic Temp
  Serial.print("Attic Temperature is: ");
  atticTemp = printTemperature(sensors, Attic_Temp_Addr);
  if (tempGetFail == false)
  {
    currentAtticTemp = atticTemp;
    // Send GW the new Attic_Temp
    //    send(msg_attic_temp.setSensor(Attic_Temp_ID).set(atticTemp, 1));
    delay(500);
  }
//  Serial.println("");

  heartBeatNo = 2.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Coppula Fan ");
  Serial.println(heartBeatNo, 1);

  //    processCopulaFan();                         // Nothing there yet
//  Serial.println("");

  heartBeatNo = 3.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Pump Flow ");
  Serial.println(heartBeatNo, 1);

  //  readTankPumpFlow();                         // Compute Tank Pump Flow
//  Serial.println("");

  heartBeatNo = 4.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Tank Pressure ");
  Serial.println(heartBeatNo, 1);

  //  readTankPumpPressure();                     // Go see if the panel pump needs to get turned                    // Get Tank Pump Pressure
//  Serial.println("");

  heartBeatNo = 5.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Tank Pump");
  Serial.println(heartBeatNo, 1);

  processTankPump();                          // Pump Water Through the Solar Panel
//  Serial.println("");


  heartBeatNo = 6.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Attic Fan/Louver ");
  Serial.println(heartBeatNo, 1);

  processAtticFan();
//  Serial.println("");

  processHVACPumpFan();                       // Pump Water Through the HVAC Heat Exchanger and Turn on the HVAC Fan
  heartBeatNo = 7.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Tank Heater ");
  Serial.println(heartBeatNo, 1);

  //    processTankHeater();                        // Turn On Tank Heater if Shop Gets To Cold

  heartBeatNo = 8.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Shop Heater ");
  Serial.println(heartBeatNo, 1);

  //    processShopSupHeater();                     // Do We Need to Turn On Extra Heat?
//  Serial.println("");

  heartBeatNo = 9.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Test Button ");
  Serial.println(heartBeatNo, 1);

  //    Test_Button ();
//  Serial.println("");

  heartBeatNo = 10.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Alarm ");
  Serial.println(heartBeatNo, 1);

  //    processAlarm();
//  Serial.println("");

  heartBeatNo = 11.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat Write LCD ");
  Serial.println(heartBeatNo, 1);

  writeLCD();
//  Serial.println("");

  heartBeatNo = 12.0;
  writeHeartBeatNo();
  Serial.print("HeartBeat 3 Sec Delay ");
  Serial.println(heartBeatNo, 1);
// unsigned long loopSleepTimerMs = 3000;
//  loopSleepTimerMs = millis();
//  }
//  if ((millis() - lastDebounceTime) > debounceDelay)
  delay(30000);
  Serial.println("");
  Serial.println("");
  // Go Do It All Again
}


// **** ( THE END ) ****

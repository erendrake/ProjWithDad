/**
"Solar Heater Controller", "1.1 RW/CW 11/11/16" 
 * Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
 * http://www.mysensors.org/build/temp
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
  #define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
  #define ONE_WIRE_BUS 22 // Pin where dallase sensor is connected 
  #define MAX_ATTACHED_DS18B20 16
  #include "config.h"
  #include <LiquidCrystal.h>; 
//  #include <LCD.h>
//  #include <LiquidCrystal_I2C.h>
  #define attic_Louver_Open LOW
  #define attic_Louver_Closed HIGH
  #define attic_Fan_On LOW
  #define attic_Fan_Off HIGH

// Solar addons
//Temperature Thresholds
  const int systemDiffOn = SYSTEM_DIFF_ON;
  const int systemDiffOff = SYSTEM_DIFF_OFF;
  const int systemOverheat = SYSTEM_OVERHEAT;
  float systemDifference = - 20; 

// **** Timers ****
  const int minimumPumpOnSeconds = MINIMUM_PUMP_ON_SECONDS;
  const int tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;
  const int minimumFanOnMs = 60000;
  const int fanDelayMs = 10000;
  const int loopSleepTimerMs = 10000;

// **** Temp Sensor Pin ****
  const int tempBusPin = TEMP_BUS_PIN;

// **** Relay Pins ****
  const int tankPumpPin = TANK_PUMP_PIN;

// **** Flow Sensor Stuff **** 
  volatile int flowCounter; //measuring the rising edges of the signal
  const int flowSensorPin = 21; //The pin location of the sensor

// **** Start OneWire and Dallas Temp ****
  OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 
  float lastTemperature[MAX_ATTACHED_DS18B20]; //?????
  int numSensors=0;  //?????
  bool receivedConfig = false;
  bool metric = false;


// Sensor Settings
  const char blarg = "hi buddy!";

// Define Child ID's for MySensors GW
  const int Number_Child_IDs           =    9; // Number of Child ID's
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

// Set Current Sensor Readings to Zero or Not Found
  float TankTemp = -127.0;
  float currentTankTemp = -127.0;
  float PanelTemp = -127.0;
  float currentPanelTemp = -127.0;
  float ShopTemp = -127.0;
  float currentShopTemp = -127.0;
  float AtticTemp = -127.0;
  float currentAtticTemp = -127.0;
  float currernTankPumpPressure = 0.0;           // variable to store the value read  
  float tankPumpFlow = 0.0;
  float currentTankPumpFlow = 0.0;
  float tankPumpPressure = 0.0;
  float currentTankPumpPressure = 0.0;

// Zero out variables used with LCD
  int intSolarPanelTemp = 0;
  int intTankTemp = 0;
  int intShopTemp = 0;  
  int intTankPumpPressure = 0;
  int intAtticTemp = 0;

// Set Device Status as Off
  bool tankPump = false;
  bool tank_Heater = false;
  bool attac_Fan = false;
  bool attic_Louver = false;
  bool atticFanStatus = false;
  bool atticLouverStatus = false;
  
// **** Hex Addresses of Dallas Temp Sensors ****
  DeviceAddress Solar_Panel_Temp_Addr = {0x28, 0x80, 0xEA, 0x29, 0x07, 0x00, 0x00, 0x0A};
  DeviceAddress Tank_Temp_Addr =        {0x28, 0x03, 0xA2, 0x29, 0x07, 0x00, 0x00, 0x6E};
  DeviceAddress Shop_Temp_Addr =        {0x28, 0x37, 0x49, 0x29, 0x07, 0x00, 0x00, 0xF5};
  DeviceAddress Attic_Temp_Addr =       {0x28, 0xD9, 0x5E, 0x29, 0x07, 0x00, 0x00, 0xE2};

// **** Initialize sensor message to MySensors Gateway ****
  MyMessage msg_solar_panel_temp(Solar_Panel_Temp_ID, V_TEMP);
//  delay(50);
  MyMessage msg_tank_temp(Tank_Temp_ID, V_TEMP);
//  delay(50);
  MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
//  delay(50);
  MyMessage msg_attic_temp(Attic_Temp_ID ,V_TEMP);
//  delay(50);
  MyMessage msg_tank_pump(Tank_Pump_ID, V_STATUS);
  MyMessage msg_tank_heater(Tank_Heater_ID, V_STATUS);
//  delay(50);
  MyMessage msg_tank_pump_pressure(Tank_Pump_Pressure_ID, V_PRESSURE);
//  delay(50);
  MyMessage msg_tank_pump_flow(Tank_Pump_Flow_ID, V_FLOW);
//  delay(50);
  MyMessage msg_attic_fan(Attic_Fan_ID ,V_STATUS);
//  delay(50);
  MyMessage msg_attic_louver(Attic_Louver_ID, V_STATUS);
//  delay(50);

//************** Start of void before *****************
void before() // ??????
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
//  LiquidCrystal lcd(10, 11, 12, 13, 14, 15, 16);
  LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
// **** Start of Void Setup ****
void setup()  
{ 
//  TRANSPORT_DEBUG(PSTR("BLARG:S\n"));
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  setupPins(); 
//  setupLCD(); Bus 2
//  lcd.begin (16,2); // <<-- our LCD is a 16x2, change for your LCD if needed
//  lcd.clear (); // go home on LCD
//  lcd.print ("  LCD  ");
  lcd.begin(16, 2);
  lcd.clear();//clears the LCD and positions the cursor in the upper-left corner
  lcd.print("hello, world!");// Print a message to the LCD.
  lcd.setCursor(2,1); // set to the 3th column and 2nd row
  lcd.print("Seeedstudio");// Print a message to the LCD.
// Flow Sensor Setup
pinMode(flowSensorPin, INPUT); //initializes digital pin 2 as an input
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
  pinMode(HVAC_Blower_Fan_and_Pump, OUTPUT);
  digitalWrite(HVAC_Blower_Fan_and_Pump, HIGH);  
  pinMode(attic_Fan_Pin, OUTPUT);   
  digitalWrite(attic_Fan_Pin, HIGH);  
  pinMode(attic_Louver_Pin, OUTPUT);   
  digitalWrite(attic_Louver_Pin, HIGH);
  pinMode(tank_Pump_Flow_Pin, INPUT);    
  // Analog
  pinMode(tank_Pump_Pressure_Pin, INPUT);  
}

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
  sendSketchInfo("SolarHeaterControllerV4", "1.2 RW/CW 12/29/16");

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
  tankPumpFlow = (flowCounter * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour
  currentTankPumpFlow = tankPumpFlow;
  Serial.println ();
  Serial.print ("Tank Pump Flow= ");
  Serial.print (tankPumpFlow, DEC); //Prints the number calculated above
  Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a new line  
  
  Serial.print("Solar Panel Temperature is:   ");
  // Fetch Temperature for Solar Panel by Sensor Address
  PanelTemp =  printTemperature(sensors, Solar_Panel_Temp_Addr);
  currentPanelTemp = PanelTemp;
  Serial.print("Panel Temp=");
  Serial.println(int(PanelTemp));
  Serial.println();
  // Send in the new Solar_Panel_Temp
  send(msg_solar_panel_temp.setSensor(Solar_Panel_Temp_ID).set(PanelTemp,1));

  Serial.print("Storage Tank Temperature is:   ");
  // Fetch Temperature for Solar Panel by Sensor Address
  TankTemp = printTemperature(sensors, Tank_Temp_Addr);
  currentTankTemp = TankTemp;
  Serial.print("Tank Temp=");
  Serial.println(int(TankTemp));  
  Serial.println();
  // Send in the new Tank_Temp
  send(msg_tank_temp.setSensor(Tank_Temp_ID).set(TankTemp,1));
     
  Serial.print("Shop Temperature is:   ");
  // Fetch Temperature for Solar Panel by Sensor Address
  ShopTemp = printTemperature(sensors, Shop_Temp_Addr);
  currentShopTemp = ShopTemp;
  Serial.print("Shop Temp=");
  Serial.println(int(ShopTemp));
  Serial.println();
  // Send in the new Shop_Temp
  send(msg_shop_temp.setSensor(Shop_Temp_ID).set(ShopTemp,1));

  Serial.print("Attic Temperature is:   ");
  AtticTemp = printTemperature(sensors, Attic_Temp_Addr);
  Serial.println();
  currentAtticTemp=AtticTemp;
  // Send GW the new Attic_Temp
  Serial.println(); 
  send(msg_attic_temp.setSensor(Attic_Temp_ID).set(AtticTemp,1));

// Go see if the panel pump needs to get turned on and if it is is there any water pressure and flow?
  readTankPumpPressure(); // Get Tank Pump Pressure
  currentTankPumpPressure = tankPumpPressure;
  send(msg_tank_pump_pressure.setSensor(Tank_Pump_Pressure_ID).set(currentTankPumpPressure,1));
  send(msg_tank_pump_flow.setSensor(Tank_Pump_Flow_ID).set(currentTankPumpFlow,1)); 
  processTankPump();
  writeLCD();  // Send results to LCD Dispaly 
}
//******************** End Of Void Loop ************************

// All Relays and LED's are Active LOW
// Need to add tests for pressure and flow??????

// **** Start of processTankPump ****
void processTankPump()
{
  systemDifference = currentPanelTemp - currentTankTemp;

  if (currentTankTemp > systemOverheat){  
    Serial.println("System Over Temp and PanelPump: Off!");
    digitalWrite(tankPumpPin, HIGH);
    tankPump = false;  
    return;
  }
  if (systemDifference > systemDiffOn && tankPump == false){
    digitalWrite(tankPumpPin, LOW);  
    Serial.println("Tank Pump: On!");
    tankPump = true;
  }
  else if(systemDifference < systemDiffOff && tankPump == true){
    digitalWrite(tankPumpPin, HIGH);  
    Serial.println("tankPump: Off!");
    tankPump = false;
  }
}
// **** End of processTankPump ****

/*-----( Declare User-written Functions )-----*/

//******** Start of printTemperature ***********
float printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
   float tempF = sensors.getTempF(deviceAddress);
   if (tempF == -127.00) 
   {
   Serial.print("Error getting temperature  ");
   } 
   else
   {
   Serial.print("F: ");
   Serial.print(tempF);
   return(tempF);
   }
}
// ***** End printTemperature ********



// Panel Pump Pressure Sensor Check to see if it's changed
void readTankPumpPressure()
{
// if after flow startup timer has expired and the flow is still to low do something 
  int sensorValue = analogRead(tank_Pump_Pressure_Pin);    // read the input pin Value range 0-1023
  float voltage = sensorValue * (5.0/1023.0);
  Serial.print("Voltage= ");
  Serial.println(int(voltage));
  float pumpPressure = ((voltage-111)/7); // Should be in PSI
  Serial.print("Pump_Pressure= ");
  Serial.print(float(pumpPressure));
  Serial.println("PSI"); //  

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
//  lcd.setCursor (0,0); // go to start of 1st line
//  lcd.print("                ");
//  lcd.setCursor (0,1); // go to start of 1st line
//  lcd.print("                ");
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
  lcd.print(int(currentTankPumpPressure));  
}
//*********( THE END )***********

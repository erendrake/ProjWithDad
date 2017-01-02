/**
"SolarHeaterControllerV4", "1.2 RW/CW 12/30/16"
 * Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
 * http://www.mysensors.org/build/temp
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
  #define ONE_WIRE_BUS 22 // Pin where dallase sensors are connected 
  #define MAX_ATTACHED_DS18B20 16
  #include "config.h"
  #include <LiquidCrystal.h>; 
//  #include <LCD.h>
//  #include <LiquidCrystal_I2C.h>
/*  #define attic_Louver_Open LOW
  #define attic_Louver_Closed HIGH
  #define attic_Fan_On LOW
  #define attic_Fan_Off HIGH*/  
 
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
  const int buttonPin = A5;    // the number of the pushbutton pin
  
// **** Temp Sensor Pin ****
  const int tempBusPin = TEMP_BUS_PIN;

// **** Relay Pins ****
  const int tankPumpPin = TANK_PUMP_PIN;
  const int gableLouverPin = GABLE_LOUVER_PIN;
  
// **** Flow Sensor Stuff **** 
  volatile int flowCounter; //measuring the rising edges of the signal
  const int flowSensorPin = FLOW_SENSOR_PIN; //The pin location of the sensor

  int buttonState;             // the current reading from the input pin
  int lastButtonState = LOW;   // the previous reading from the input pin

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
  const int Number_Child_IDs           =   18; // Number of Child ID's
  const int Number_Temp_Sensors        =    4; // Number of Temp Sensors
  const int Solar_Panel_Temp_ID        =    0; // Panel Temp Child ID
  const int Tank_Temp_ID               =    1; // Tank Temp Child ID
  const int Shop_Temp_ID               =    2; // Shop Temp Child ID
  const int Tank_Pump_ID               =   10; // Panel Pump Child ID
  const int Tank_Pump_Pressure_ID      =   11; // Panel Pump Pressure Child ID
  const int Tank_Pump_Flow_ID          =   12; // Panel Pump Flow Child ID
  const int Tank_Heater_ID             =   13; // Storage Tank Heater on/off
  const int SHVAC_Fan_Pump_ID          =   14; // Solar HVAC Pump and Fan
  const int Chiller_Pump_ID            =   15; // Swamp Cooler Water Pump
  const int SHVAC_Set_Point_ID         =   32; // Pot to Control Shop Temp (controls HVAV Pump and Fan)
    
  const int Attic_Temp_ID              =   20; // Attic Temp Child ID
  const int Attic_Fan_ID               =   21; // Attic Fan on/off
  const int Attic_Louver_ID            =   22; // Attic Louver open/closed
  const int Gable_Louver_ID            =   23; // Attic Louver open closed
  const int Coulpa_Fan_Parent_ID       =   50; // Parent ID of Coupla Controller #1
  const int Coulpa_Fan_Child_ID        =    1; // Child ID of the Fan in the Coupla #1

  const int General_Alarm_ID           =   30; // Alarms to Vera and Local Buzzer
  const int Test_Button_ID             =   31; // Test Button
  const int Moton_Sensor_ID            =   33; // Moton Sensor for Alarm 

  const int SHeater_Parent_ID          =   21; // Parent ID of Supplemently Heater Controller #2
  const int SHeater_Child_ID           =    1; // Child ID of Supplemantly Heater #2
  const int SHeater_Set_Point_ID       =   32; // Supplemently Heater Thermostat Set Point

  // #1, #2 Parent and Child ID's of Exturnal Controllers

  
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
  float hvacSetPoint = 0.0;
  float systemDifference = 0.0;

// **** Zero out variables used with LCD ****
  int intSolarPanelTemp = 0;
  int intTankTemp = 0;
  int intShopTemp = 0;  
  int intTankPumpPressure = 0;
  int intAtticTemp = 0;

// **** Set Device Status as Off ****
  bool tankPump           = false;
  bool tank_Heater        = false;
  bool attac_Fan          = false;
  bool attic_Louver       = false;
  bool atticFanStatus     = false;
  bool atticLouverStatus  = false;
  bool generalAlarmStatus = false;
  bool hvacFanPumpStatus  = false;
  bool motonSensorStatus  = false;
  bool gableLouverStatus  = false;
  bool couplaFanStatus    = false;
  bool sHeaterStatus      = false; // Supplamently Heater on/off    

// **** Hex Addresses of Dallas Temp Sensors ****
  DeviceAddress Solar_Panel_Temp_Addr = {0x28, 0x80, 0xEA, 0x29, 0x07, 0x00, 0x00, 0x0A};
  DeviceAddress Tank_Temp_Addr =        {0x28, 0x03, 0xA2, 0x29, 0x07, 0x00, 0x00, 0x6E};
  DeviceAddress Shop_Temp_Addr =        {0x28, 0x37, 0x49, 0x29, 0x07, 0x00, 0x00, 0xF5};
  DeviceAddress Attic_Temp_Addr =       {0x28, 0xD9, 0x5E, 0x29, 0x07, 0x00, 0x00, 0xE2};

// **** Initialize sensor message to MySensors Gateway ****
  MyMessage msg_solar_panel_temp(Solar_Panel_Temp_ID, V_TEMP);
  MyMessage msg_tank_temp(Tank_Temp_ID, V_TEMP);
  MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
  MyMessage msg_attic_temp(Attic_Temp_ID ,V_TEMP);
  MyMessage msg_tank_pump(Tank_Pump_ID, V_STATUS);
  MyMessage msg_tank_heater(Tank_Heater_ID, V_STATUS);
  MyMessage msg_tank_pump_pressure(Tank_Pump_Pressure_ID, V_PRESSURE);
  MyMessage msg_tank_pump_flow(Tank_Pump_Flow_ID, V_FLOW);
  MyMessage msg_attic_fan(Attic_Fan_ID ,V_STATUS);
  MyMessage msg_attic_louver(Attic_Louver_ID, V_STATUS);
  MyMessage msg_general_alarm(General_Alarm_ID, V_STATUS);
  MyMessage msg_test_button(Test_Button_ID, V_STATUS);
  MyMessage msg_HVAC_Fan_Pump(HVAC_Fan_Pump_ID, V_STATUS);
  MyMessage msg_set_temp_pot_heat(HVAC_Set_Point_ID, V_HVAC_SETPOINT_HEAT);
  MyMessage msg_set_temp_pot_cool(HVAC_Set_Point_ID, V_HVAC_SETPOINT_COOL);
  MyMessage msg_moton_sensor(Moton_Sensor_ID, V_TRIPPED);
  MyMessage msg_gable_louver(Gable_Louver_ID, V_STATUS);
  MyMessage msg_coupla_fan(Coupla_Fan_ID, V_STATUS);
  MyMessage msg_
  
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
  pinMode(buttonPin, INPUT);
// Flow Sensor Setup
  pinMode(flowSensorPin, INPUT); //initializes digital pin 21 ON Mega as an input
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
}

// **** Start of setupPins ****
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
// Analog
  pinMode(tank_Pump_Pressure_Pin, INPUT);  
  pinMode(Set_Temp_Pot_Pin, INPUT);
}
// Save For IC2 Display
/*void setupLCD()
{
// LCD Setup
  Serial.println("We made it to LCD Setup");
  lcd.begin (16,2); // <<-- our LCD is a 16x2, change for your LCD if needed
  lcd.clear (); // go home on LCD
}
*/

// **** Start of Presentation ****
void presentation() 
{
//  TRANSPORT_DEBUG(PSTR("BLARG:P\n"));

// Send the sketch version information to the gateway and Controller
  sendSketchInfo("SolarHeaterControllerV4", "1.2 RW/CW 12/30/16");

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
  currentTankPumpFlow = (tankPumpFlow,2);
  Serial.println ();
  Serial.print ("Tank Pump Flow= ");
  Serial.print (tankPumpFlow, 2); //Prints the number calculated above
  Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a new line  
  
  // Fetch Temperature for Solar Panel by Sensor Address
  Serial.print("Panel Temp=");
  PanelTemp =  printTemperature(sensors, Solar_Panel_Temp_Addr);
  currentPanelTemp = tempF;

  // Fetch Temperature for Solar Panel by Sensor Address
  Serial.print("Tank Temp=");
  TankTemp = printTemperature(sensors, Tank_Temp_Addr);
  currentTankTemp = tempF;
     
  // Fetch Temperature for Solar Panel by Sensor Address
  Serial.print("Shop Temp=");
  ShopTemp = printTemperature(sensors, Shop_Temp_Addr);
  currentShopTemp = tempF;
  
  // Fetch Temperature for Attic by Sensor Address
  Serial.print("Attic Temp=");
  AtticTemp = printTemperature(sensors, Attic_Temp_Addr);
  currentAtticTemp = tempF;

// Go see if the panel pump needs to get turned on and if it is is there any water pressure and flow?
  readTankPumpPressure(); // Get Tank Pump Pressure
  currentTankPumpPressure = tankPumpPressure;
  processTankPump();
  processAtticFan();
  writeLCD();  // Send results to LCD Dispaly and update MySenors GW 
  Test_Button ();
}
//******************** End Of Void Loop ************************

/*-----( Declare User-written Functions )-----*/

// All Relays and LED's are Active LOW  ??? Not sure of this.
// Need to add tests for pressure and flow??????

// **** Start of processTankPump ****
void processTankPump()
{
  systemDifference = currentPanelTemp - currentTankTemp;

  if (currentTankTemp > systemOverheat){  
    Serial.println("System Over Temp and PanelPump: Off!");
    digitalWrite(tankPumpPin, HIGH);
    tankPump = false;  
    generalAlarmStatus = true;      
    digitalWrite(general_Alarm_Pin, HIGH);
    send(msg_general_alarm.setSensor(General_Alarm_ID).set(generalAlarmStatus,1));    
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


//*********** Stat of processAtticFan *************
// All Relays and LED's are Active LOW ???? Not sure of this

void processAtticFan(){
  Serial.println("We made it to the processAtticFan");
  float systemDifference = AtticTemp - ShopTemp;
  if (systemDifference > systemDiffOn && atticFanStatus == false && atticLouverStatus == false){
    digitalWrite(attic_Louver_Pin, attic_Louver_Open);  
    atticLouverStatus = true;
//    TRANSPORT_DEBUG(PSTR("Attic Louver: Opening!\n"));
    //wait for Louver to open
    delay(fanDelayMs);
//    TRANSPORT_DEBUG(PSTR("Attic Louver: Open!\n"));
//  Send GW New Status
    digitalWrite(attic_Fan_Pin, attic_Fan_On);  
    atticFanStatus = true;
//    TRANSPORT_DEBUG(PSTR("Attic Fan: On!\n"));
//  Send GW New Status
	updateSystemStats();
    return;
  }
  else if(systemDifference < systemDiffOff && atticFanStatus == true && atticLouverStatus == true){
    digitalWrite(attic_Louver_Pin, attic_Louver_Closed);  
    atticLouverStatus = false;
//    TRANSPORT_DEBUG(PSTR("Attic Louver: Closing!\n"));
    //wait for fan to stop 
    delay(fanDelayMs);
    digitalWrite(attic_Fan_Pin, attic_Fan_Off);  
    atticFanStatus = false;
//    TRANSPORT_DEBUG(PSTR("Attic Fan: Off!\n"));
//  Send GW New Status
	updateSystemStats();
    return;
  }
}// ********** End processAtticFan ************


//******** Start of printTemperature ***********
float printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
   tempF = sensors.getTempF(deviceAddress);
   if (tempF == -127.00) 
   {
   Serial.print("Error getting temperature  ");
   generalAlarmStatus = true;
   updateSystemStatus();
   } 
   else
   {
   Serial.print("F: ");
   Serial.println(tempF);
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

void updateSystemStats ();
{
  send(msg_attic_louver.setSensor(Attic_Louver_ID).set(atticFanStatus));
  send(msg_attic_fan.setSensor(Attic_Fan_ID).set(atticFanStatus));
  send(msg_attic_temp.setSensor(Attic_Temp_ID).set(AtticTemp,1));
  send(msg_shop_temp.setSensor(Shop_Temp_ID).set(ShopTemp,1));
  Serial.println(int(ShopTemp));
  send(msg_tank_temp.setSensor(Tank_Temp_ID).set(TankTemp,1));
  send(msg_solar_panel_temp.setSensor(Solar_Panel_Temp_ID).set(PanelTemp,1));
  send(msg_tank_pump_pressure.setSensor(Tank_Pump_Pressure_ID).set(currentTankPumpPressure,1));
  send(msg_tank_pump_flow.setSensor(Tank_Pump_Flow_ID).set(currentTankPumpFlow,1)); 
  send(msg_general_alarm.setSensor(General_Alarm_ID).set(generalAlarmStatus,1)); 
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
  lcd.setCursor (13,1);
  lcd.print ("F");
  lcd.setCursor (14,1);
  lcd.print(currentTankPumpFlow, 1);
}

void Test_Button ()
{
if (generalAlarmStatus == true) return ; // If True Return Because System is Already in Alarm
// Test Button Will Set off Alarm
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the Alarm if the new button state is HIGH
      if (buttonState == HIGH) {
        generalAlarmStatus = true;      
        digitalWrite(general_Alarm_Pin, HIGH);
		updateSystemStatus();
      }
    }
  }
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}
//*********( THE END )***********

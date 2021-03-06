/**
"Solar Heater Controller", "1.1 RW/CW 11/11/16" 
 * Example sketch showing how to send in DS1820B OneWire temperature readings back to the controller
 * http://www.mysensors.org/build/temp
 */


// Enable debug prints to serial monitor
  #define MY_DEBUG 

// Enable and select radio type attached
  #define MY_RADIO_NRF24
  #define MY_NODE_ID 25
  #include <SPI.h>
  #include <MySensors.h>  
  #include <DallasTemperature.h>
  #include <OneWire.h>
  #define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
  #define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 
  #define MAX_ATTACHED_DS18B20 16
  #include "config.h"
  #include <LCD.h>
  #include <LiquidCrystal_I2C.h>

// 2 line LCD Stuff
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
  const float systemDiffOn = SYSTEM_DIFF_ON;
  const float systemDiffOff = SYSTEM_DIFF_OFF;
  const float systemOverheat = SYSTEM_OVERHEAT;
  
// Timer
  const int minimumPumpOnSeconds = MINIMUM_PUMP_ON_SECONDS;
  const int tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;

// LED Pins


// Relay Pins
  const int tankPumpPin = TANK_PUMP_PIN;

// Sensor Pins
  const int tempBusPin = TEMP_BUS_PIN;
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
  float currentTank_Temp = -127.0;
  float currentSolar_Panel_Temp = -127.0;
  float currentShop_Temp = -127.0;
  float currentAttic_Temp = -127.0;
  float cutternTank_Pump_Pressure = 0.0;           // variable to store the value read  
  float currentTank_Pump_Flow = 0.0;
  
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

  
// Start OneWire and Dallas Temp 
  OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 
  float lastTemperature[MAX_ATTACHED_DS18B20]; //?????
  int numSensors=0;  //?????
  bool receivedConfig = false;
  bool metric = false;

//DeviceAddress Solar_Panel_Temp = {0x28, 0x9B, 0x44, 0x1D, 0x07, 0x00, 0x00, 0x7E};
//DeviceAddress Tank_Temp =        {0x28, 0x45, 0xA3, 0x1C, 0x07, 0x00, 0x00, 0xAD};
//DeviceAddress Shop_Temp =        {0x28, 0x45, 0xA3, 0x1C, 0x07, 0x00, 0x00, 0xAD};
//DeviceAddress Attic_Temp =       {0x28, 0xD9, 0x5E, 0x29, 0x07, 0x00, 0x00, 0xE2};

DeviceAddress Solar_Panel_Temp = {0x28, 0x80, 0xEA, 0x29, 0x07, 0x00, 0x00, 0x0A};
DeviceAddress Tank_Temp =        {0x28, 0x03, 0xA2, 0x29, 0x07, 0x00, 0x00, 0x6E};
DeviceAddress Shop_Temp =        {0x28, 0x37, 0x49, 0x29, 0x07, 0x00, 0x00, 0xF5};
//DeviceAddress Attic_Temp =       {0x28, 0xD9, 0x5E, 0x29, 0x07, 0x00, 0x00, 0xE2};

// Initialize sensor message to MySensors Gateway
// MyMessage msg(0,V_TEMP);
  MyMessage msg_solar_panel_temp(Solar_Panel_Temp_ID, V_TEMP);
  MyMessage msg_tank_temp(Tank_Temp_ID, V_TEMP);
  MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
  MyMessage msg_attic_temp(Attic_Temp_ID ,V_TEMP);
  MyMessage msg_tank_pump(Tank_Pump_ID, V_STATUS);
  MyMessage msg_tank_pump_pressure(Tank_Pump_Pressure_ID, V_PRESSURE);
  MyMessage msg_tank_pump_flow(Tank_Pump_Flow_ID, V_FLOW);
  MyMessage msg_attic_fan(Attic_Fan_ID ,V_STATUS);
  MyMessage msg_attic_louver(Attic_Louver_ID, V_STATUS);


//************** Start of void before *****************
void before() // ??????
{
  TRANSPORT_DEBUG(PSTR("BLARG:B\n"));
  // Startup up the OneWire library
  sensors.begin();

// set the resolution to tempResolution (Can be 9 to 12 bits .. lower is faster)
  const int tempResolution = 9;
  sensors.setResolution(Solar_Panel_Temp, tempResolution);
  sensors.setResolution(Tank_Temp, tempResolution);
  sensors.setResolution(Shop_Temp, tempResolution);
//  sensors.setResolution(Attic_Temp, tempResolution);
}
//************** End of void before *****************

//************** Start of Void Setup *****************
void setup()  
{ 
  TRANSPORT_DEBUG(PSTR("BLARG:S\n"));
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
  setupPins(); 
//  setupLCD();
}

void setupPins(){
  // Digital
  pinMode(tankPumpPin, OUTPUT);   
  digitalWrite(tankPumpPin, HIGH);  
  pinMode(tank_Heater_pin, OUTPUT);
  digitalWrite(tank_Heater_pin, HIGH);
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

//************** End of Void Setup *****************

//************** Start of Presentation ********************
void presentation() {
  TRANSPORT_DEBUG(PSTR("BLARG:P\n"));

// Send the sketch version information to the gateway and Controller
  sendSketchInfo("Solar Heater Controller", "1.1 RW/CW 11/11/16");

// Fetch the number of attached temperature sensors  
  numSensors = Number_Temp_Sensors;

// Present all sensors to controller
// Array for storing temps

  present(Solar_Panel_Temp_ID, S_TEMP);          // Panel Temp Child ID 0
  present(Tank_Temp_ID, S_TEMP);                 // Tank Temp Child ID 1
  present(Shop_Temp_ID, S_TEMP);                 // Shop Temp Child ID 2
  present(Attic_Temp_ID, S_TEMP);                // Attic Temp Child ID 3
  present(Tank_Heater_ID, S_BINARY);             // Tank Heater Child ID 15
  present(Tank_Pump_ID, S_BINARY);               // Tank Pump Child ID 10 
  present(Tank_Pump_Pressure_ID, S_CUSTOM);      // Panel Pump Pressure Child ID 4
  present(Tank_Pump_Flow_ID, S_WATER);            // Panel Pump Flow Child ID 6
  present(Attic_Fan_ID, S_BINARY);               // Attic Fan Child ID 13
  present(Attic_Louver_ID, S_BINARY);             // Attic Louver Child ID 14


// ************* End Of Presentation ********************
}

void loop()     
{ 
  delay(1000);

  Serial.println();
  Serial.print("Number of Devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());   
  Serial.print("Getting temperatures... ");  
  Serial.println();   
    
  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

  Serial.print("Solar Panel Temperature is:   ");
  printTemperature(sensors, Solar_Panel_Temp);
  Serial.println();

  Serial.print("Storage Tank Temperature is:   ");
  printTemperature(sensors, Tank_Temp);
  Serial.println();
 
  Serial.print("Shop Temperature is:   ");
  printTemperature(sensors, Shop_Temp);
  Serial.println();
//  //Attic_Temp
//  Serial.print("Attic Temperature is:   ");
//  printTemperature(sensors, Attic_Temp);
//  Serial.println();
   
  // query conversion time and sleep until conversion completed
 // int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  // sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
 // wait(conversionTime);

  // Read temperatures and send them to controller 
  //for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {
//  for (int 1=0;  
    // Fetch and round temperature to one decimal
    //float temperature = getTempByIndex(i);
    // Only send data if temperature has changed and no error
    #if COMPARE_TEMP == 1
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
    #else
    if (temperature != -127.00 && temperature != 85.00) {
    #endif


  // Go see if the panel pump needs to get turned on and if it is is there any water pressure and flow?
  float tankPumpPressureReading = tankPumpPressure(); // Get Tank Pump Pressure
  float tankPumpFlowReading = tankPumpFlow(); // Get Tank Pump Flow



      // Send in the new Solar_Panel_Temp
      send(msg.setSensor(Solar_Panel_Temp_ID).set(Solar_Panel_Temp,1));
    #if COMPARE_TEMP == 1
    if (currentSolarPanel_Temperature != Solar_Panel_Temp && Solar_Panel_Temp != -127.00 && Solar_Panel_Temp != 85.00) {
    #else
    if (Solar_Panel_Temp != -127.00 && Solar_Panel_Temp != 85.00) {
    #endif
      // Save new temperatures for next compare
      currentSolarPanelTemperature=set(Solar_Panel_temp);

      // Send in the new Tank_Temp
      send(msg.setSensor(Tank_Temp_ID).set(Tank_Temp,1));
    #if COMPARE_TEMP == 1
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
    #else
    if (temperature != -127.00 && temperature != 85.00) {
    #endif
      // Save new temperatures for next compare
      currentTankTemo=TankTemp;

      // Send in the new Shop_Temp
    send(msg.setSensor(Shop_Temp_ID).set(Shop_Temp,1));
    #if COMPARE_TEMP == 1
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
    #else
    if (temperature != -127.00 && temperature != 85.00) {
    #endif
    // Save new temperatures for next compare
    currentShopTemp=ShopTemp;

//      // Send in the new Attic_Temp
//      send(msg.setSensor(AtticTempID).set(Attic_Temp,1));
//    #if COMPARE_TEMP == 1
//    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
//    #else
//    if (temperature != -127.00 && temperature != 85.00) {
//    #endif
//      // Save new temperatures for next compare
//      currentAtticTemp=AtticTemp;

  processTankPump(tankPumpPressureReading, tankPumpFlowReading);
  writeLCD(currentPanelTemp, currentTankTemp, currentShopTemp, tankPumpPressureReading);  // Send results to LCD Dispaly 

}
******************** End Of Void Loop ************************

// All Relays and LED's are Active LOW
// Need to add tests for pressure and flow??????
void processTankPump(float tankPumpPressure, float tankPumpFlow){
 /*float systemDifference = currentPanelTemp - currentTankTemp;

  if (currentTankTemp > systemOverheat){  
    Serial.println("System Over Temp and PanelPump: Off!");
    digitalWrite(panelPumpPin, HIGH);
    tankPumpStatus = false;  
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
  }*/
}



// ******* End of viod loop ********

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
//*********( THE END )***********


// Panel Pump Pressure Sensor Check to see if it's changed
float tankPumpPressure(){
    return 1.0;
  // Put a startup timer here
//  tankPumpFlow ();  // get tank pump flow
// if after flow startup timer has expired and the flow is still to low do something 
  int sensorValue = analogRead(tank_Pump_Pressure_Pin);    // read the input pin Value range 0-1023
  float voltage = sensorValue * (5.0/1023.0);
  Serial.print("Voltage= ");
  Serial.println(int(voltage));
  float pumpPressure = ((voltage-111)/7); // Should be in PSI
  Serial.print("Pump_Pressure= ");
  Serial.print(float(pumpPressure));
  Serial.println("PSI"); //  

  return pumpPressure;
  
  // If Pressure has changed send in the the new Pump Pressure
  //    send(msg.setSensor(numSensors+2).set(pumpPressure,1));
  
  // if pressure is <20 PSI shut pump off and set an alarm 
}  

float tankPumpFlow() {
    return 1.0;
//void tankPumpFlow(oldFlow) {
  // Put tank flow software here
   // if flow is to low even if there is pressure
   // shut pump off and set an alarm also must setup a startup timer each time we turn on the pump
  // oldTankPumpFlow = tankPumpFlow
}

float logTempSensor(DallasTemperature sensors, const DeviceAddress address, const char* name){    
    Serial.print("Temperature for the device ");
    Serial.print(name);
    Serial.print(" is: ");
    float result = sensors.getTempF(address);
    Serial.println(result);  
    return result;
}




void setupLCD(){
// LCD Setup
lcd.begin (16,2); // <<-- our LCD is a 20x4, change for your LCD if needed
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);  // LCD Backlight ON
lcd.setBacklight(HIGH);
lcd.home (); // go home on LCD
}

void writeLCD(float currentPanelTemp, float currentTankTemp, float currentShopTemp, float currentTankPumpPressure){
    // Convert sensor flots to int and display
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
  lcd.print(int(currentTankPumpPressure));  
}


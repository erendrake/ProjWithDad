// Solar.h

#ifndef Solar_h
#define Solar_h
#define LIBRARY_VERSION 1.0.0

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
const int setPoint = Set_Temp_Pot_Pin;

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
int errorTankUnderTemp = 11;
int errorTestButton = 12;

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
float tankPumpPressure = 0.0;
float currentTankPumpPressure = 0.0;
float tankPumpFlowLPM = 0.0;
float tankPumpFlowGPM = 0.0;
float currentTankPumpFlowLPM = 0.0;
float currentTankPumpFlowGPM = 0.0;
float hvacSetPoint = 0.0;           // Float Value of Thermostate Setting

// Zero out variables used with LCD
int intPanelTemp = 0;
int intTankTemp = 0;
int intShopTemp = 0;
int intTankPumpPressure = 0;
int intAtticTemp = 0;
int intHVACSetPoint = 0;  // Thermostat Setting Value
int potValue = 0;         // Thermostat Pot

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
//************** Start of void before *****************


// **** 2 line LCD Stuff ****
// Initialize display. Google the correct settings for your display.
// The follwoing setting should work for the recommended display in the MySensors "shop".
//   LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal lcd(10, 11, 12, 13, A0, A1, A2);


}
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

// **** Start of Process HVAC Pump and Fan ****

void processHVACPumpFan()
{

}


void thermostatSetting()
{
  potValue = analogRead(set_Temp_Pot_Pin);
  Serial.print("Pot Value=");
  Serial.println(potValue);     // Can be Anything From 0-1023
  hvacSetPoint = (float(potValue) * 0.039) + 40.0;
  intHVACSetPoint = int(hvacSetPoint);
  Serial.println();
  Serial.print("Temp Set Point=");
  Serial.println(intHVACSetPoint);
  Serial.println();
}
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
#endif

//*********( THE END )***********

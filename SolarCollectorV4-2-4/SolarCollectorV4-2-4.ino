/**
  "SolarHeaterControllerV4-2-4", "RW/CW 1/15/17"
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
#include "alarm.h"
#include "my_lcd.h"
#include "my_mysensors.h"
#include "tank_pump.h"
#include "HVAC_Pump.h"

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

// **** Setup Pins ****
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
  pinMode(buttonPin, INPUT);
  pinMode(flowSensorPin, INPUT);              //initializes digital pin 21 ON Mega as an input
  // Analog
  pinMode(tank_Pump_Pressure_Pin, INPUT);
  pinMode(set_Temp_Pot_Pin, INPUT);
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

//**** Get Each Temp Sensor Reading by Their Hex Address ****
float printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
    tempF = sensors.getTempF(deviceAddress);
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

// **** Get Thermostat Setting ****
void thermostatSetting()
{
  potValue = analogRead(set_Temp_Pot_Pin);
  Serial.print("Pot Value=");
  Serial.println(potValue);                // Can be Anything From 0-1023
  hvacSetPoint = (float(potValue) * 0.039) + 40.0;
  intHVACSetPoint = int(hvacSetPoint);
  Serial.println();
  Serial.print("Temp Set Point=");
  Serial.println(intHVACSetPoint);
  Serial.println();
}

// **** ( THE END ) ****

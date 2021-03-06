// Config.h for Solar Project
// start serial port
//  Serial.begin(112500);
//
// Serial.println("Config.h.");
// Define Sensors and Controllers
// #define TEMP_POLLING_DELAY_SECONDS 3                  // Temp Polling Interval (Currently Not Used)
#define SOLAR_PANEL_TEMP_NAME "Solar_Panel_Temp"      //
#define TANK_TEMP_NAME "Tank_Temp"
#define SHOP_TEMP_NAME "Shop_Temp"
#define ATTIC_TEMP_NAME "Attic_Temp"
#define TANK_PUMP_NAME "Tank_Pump"
#define TANK_PUMP_PRESSURE_NAME "Tank_Pump_Pressure"
#define TANK_PUMP_FLOW_NAME "Tank_Pump_Flow"
#define ATTIC_FAN_NANE "Attic_Fan";
#define ATTIC_LOUVER_NAME "Attic_Louver";

#define attic_Louver_Open LOW
#define attic_Louver_Closed HIGH
#define attic_Fan_On LOW
#define attic_Fan_Off HIGH
#define attic_Gable_Louver_Open LOW
#define attic_Gable_Louver_Closed HIGH
#define cupolaFanOn LOW
#define cupolaFanOff HIGH
#define hvacFanPumpOn LOW
#define hvacFanPumpOff HIGH
#define hvacPumpOn LOW
#define hvacPumpOff HIGH
#define hvacFanOn LOW
#define hvacFanOff HIGH
#define Summer HIGH
#define Winter LOW
#define switchState Winter
#define lastSwitchState Winter
#define summerWinterMode Winter                        // Summer = 0, Winter = 1 Default to Winter
#define lcdA0 A0
#define lcdA1 A1
#define lcdA2 A2
#define tempPotPin A4
#define tankPumpPressurePin A11


// Test/Reset Button
bool buttonState = false;                                       // the current reading from the input pin
bool lastButtonState = false;                            // the previous reading from the input pin

// Other Nodes (remote) Parent and Child ID's
// Supplamental Shop Electic Heater Controller MySensors Info
const uint8_t Shop_Sup_Heater_Parent_ID = 7;                 // Parent Id of Heater Node
const uint8_t Shop_Sup_Heater_Child_ID = 1;                  // This is the Heater (Child) ID
const uint8_t Out_Side_Temp_Hum_Parent_ID = 6;
const uint8_t Out_Side_Temp_Child_ID = 1;
const uint8_t Out_Side_Hum_Child_ID = 0;

// **** Timers ****
unsigned long minPumpOnTime = 120000;
unsigned long currentPumpOnTime = 0;
unsigned long atticFanDelayMs = 10000;                // Fan Time Delay for Louvers to Open or Close
unsigned long loopSleepTimerMs = 3000;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long pumpOnTime = 0;               // Used for Minute Min Pump On Time
unsigned long hvacFanPumpTurnOnTime = 0;
unsigned long loopTimer = 0;

// **** Pin Assingments for Arduino Mega ****
// Radio Pin Assingments for Arduino Mega Defined in Main Sketch
// Pin Name - Pin Number - Wire Color
// CE PIN =     D40         Orange
// Miso Pin =   D50         Violet
// MOSI Pin =   D51         Blue
// SCK Pint =   D52         Green
// CSN/CS PIN = D53         Yellow
// VCC      = SupHeader +5V Red                   // * Extra Header Mostly for Power.
// GND      = SupHeader GND Black

// Digital
const uint8_t motion_Sensor_Pin = 8;                 // Motion Sensor Pin
const uint8_t general_Alarm_Pin = 9;                 // Alarm Buzzer
//const uint8_t atticGableLouverPin = 16;              // Attic Gable Louver Pin
//const uint8_t summerWinterModePin = 17;              // * Summer/Winter Mode Switch
//const uint8_t tankPumpPin = 20;                      // * Tank Pump Pin
//const uint8_t flowSensorPin = 21;                    // * Tank Pump Flow Sensor Pin Interrupt # 2 on Mega
//const uint8_t HVAC_Fan_Pump_Pin = 23;                // * HVAC Blower Fan and Pump Pin
// ONE_WIRE_BUS = 3;                             // * Dallas Temp Sensors Pin Defined in Main Sketch
const uint8_t atticLouverPin = 8;                 // * Attic Louver Pin
// const uint8_t tank_Heater_Pin = 26;                  // * Storage Tank Heater Pin
const uint8_t atticFanPin = 7;                    // * Attic Fan Pin
// const uint8_t cupolaFanPin = 28;                     // Cupola Fan Pin

// Analog
const uint8_t set_Temp_Pot_Pin = A4;                 // Analog Pin for Thermostat Pot
const uint8_t buttonPin = 11;                        // the number of the pushbutton pin
// const uint8_t tank_Pump_Pressure_Pin = A11;          // Tank Pressure Sensor Pin


// **** Flow Sensor Stuff ****
volatile int flowCounter;                         //measuring the rising edges of the signal
float calc_lpm;                                   // Liters Per Minute
float calc_gpm;

float heartBeatNo = 0.0;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;                    // the last time the output pin was toggled
long debounceDelay = 50;                      // the debounce time; increase if the output flickers

//  General Alarm State
uint8_t errorState = 0;
uint8_t errorOverTemp = 1;
uint8_t errorTempGetFail = 2;
uint8_t errorLowTankPumpPressure = 3;
uint8_t errorHighTankPumpPressure = 4;
uint8_t errorLowTankPumpFlow = 5;
uint8_t errorHighTankPumpFlow = 6;
uint8_t errorLowHVACPumpPressure = 7;
uint8_t errorHighHVACPumpPressure = 8;
uint8_t errorLowHVACPumpFlow = 9;
uint8_t errorHighHVACPumpFlow = 10;
uint8_t errorTankUnderTemp = 11;
uint8_t errorTestButton = 12;

// Set Current Sensor Readings to Zero or Not Found
float tankTemp = -127.0;
float currentTankTemp = -127.0;
float panelTemp = -127.0;
float currentPanelTemp = -127.0;
float lowerInletTemp = -127.0;
float currentLowerInletTemp = -127.0;
float shopTemp = -127.0;
float currentShopTemp = -127.0;
float atticTemp = -127.0;
float currentAtticTemp = -127.0;
float tankPumpFlow = 0.0;
float tankPumpPressure = 0.0;
float currentTankPumpPressure = 0.0;
float tankPumpFlowLPM = 0.0;                  // Tank Pump Flow in LPM
float tankPumpFlowGPM = 0.0;                  // Tank Pump Flow in GPM
float currentTankPumpFlowLPM = 0.0;           // Tank Pump Flow in LPM
float currentTankPumpFlowGPM = 0.0;           // Tank Pump Flow in GPM
float hvacSetPoint = 0.0;                     // Float Value of Thermostate Setting
float tempF = 0.0;

//Temperature Thresholds
float systemDiffOn = 5.0;                      // 5°
float systemDiffOff = 0.0;                     // 0°
float systemOverheat = 190.0;                  // OverTemp Tank Temp >= 190°
float systemDifference = - 20.0;
float tankTempUnderTemp = 40.0;               // Tank Under Temp or When to Turn the Tank Hearter On!
 
// Zero out variables used with LCD
int intPanelTemp = 0;
int intTankTemp = 0;
int intShopTemp = 0;
int intTankPumpPressure = 0;
int intAtticTemp = 0;
int intHVACSetPoint = 0;                      // Thermostat Setting Value
int potValue = 0;                             // Thermostat Pot

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
bool heating = false;
bool supHeating = false;
bool cupolaFanStatus = false;
bool ack = true;

// **** Hex Addresses of Dallas Temp Sensors ****
DeviceAddress Solar_Panel_Temp_Addr = {0x28, 0x9B, 0x44, 0x1D, 0x07, 0x00, 0x00, 0x7E};
DeviceAddress Tank_Temp_Addr =        {0x28, 0x18, 0xD3, 0x28, 0x07, 0x00, 0x00, 0xE2};
DeviceAddress Shop_Temp_Addr =        {0x28, 0x3A, 0xAC, 0x2A, 0x07, 0x00, 0x00, 0x3F};
DeviceAddress Attic_Temp_Addr =       {0x28, 0xB8, 0x3D, 0x29, 0x07, 0x00, 0x00, 0x49};
DeviceAddress Lower_Inlet_Temp_Addr = {0x28, 0x37, 0x49, 0x29, 0x07, 0x00, 0x00, 0xF5};
// 28 B8 3D 29 07 00 00 49
// 28 18 D3 28 07 00 00 E2


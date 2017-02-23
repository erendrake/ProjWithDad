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

// Test/Reset Button
int buttonState;                                       // the current reading from the input pin
int lastButtonState = HIGH;                            // the previous reading from the input pin


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

//Temperature Thresholds
#define systemDiffOn 1.0                              // 1°
#define systemDiffOff 0.0                             // 0°
#define systemOverheat 190.0                          // OverTemp Tank Temp >= 190°
float systemDifference = - 20.0;

// Other Nodes Parent and Child ID's
// Supplamental Shop Electic Heater Controller MySensors Info
const uint8_t shopSupHeaterParentID = 7;                 // Parent Id of Heater Node
const uint8_t shopSupHeaterChildID = 1;                  // This is the Heater (Child) ID
const uint8_t outSideTempHumParentID = 6;
const uint8_t outSideTempChildID = 1;
const uint8_t outSideHumChildID = 0;

// **** Timers ****
unsigned long minPumpOnTime = 120000;
unsigned long currentPumpOnTime = 0;
// unsigned long tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;
// unsigned long minimumFanOnMs = 60000;
unsigned long atticFanDelayMs = 10000;                // Fan Time Delay for Louvers to Open or Close
unsigned long loopSleepTimerMs = 3000;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long pumpOnTime = 0;               // Used for Minute Min Pump On Time
unsigned long hvacFanPumpOnTime = 0;
unsigned long currentHVACFanPumpOnTime = 0;
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
const uint8_t atticGableLouverPin = 16;              // Attic Gable Louver Pin
const uint8_t summerWinterModePin = 17;              // * Summer/Winter Mode Switch
const uint8_t tankPumpPin = 20;                      // * Tank Pump Pin
const uint8_t flowSensorPin = 21;                    // * Tank Pump Flow Sensor Pin Interrupt # 2 on Mega
const uint8_t HVAC_Fan_Pump_Pin = 23;                // * HVAC Blower Fan and Pump Pin
// ONE_WIRE_BUS = 24;                             // * Dallas Temp Sensors Pin Defined in Main Sketch
const uint8_t attic_Louver_Pin = 25;                 // * Attic Louver Pin
const uint8_t tank_Heater_Pin = 26;                  // * Storage Tank Heater Pin
const uint8_t attic_Fan_Pin = 27;                    // * Attic Fan Pin
const uint8_t cupolaFanPin = 28;                     // Cupola Fan Pin

// Analog
const uint8_t set_Temp_Pot_Pin = A4;                 // Analog Pin for Thermostat Pot
const uint8_t buttonPin = 28;                        // the number of the pushbutton pin
const uint8_t tank_Pump_Pressure_Pin = A11;          // Tank Pressure Sensor Pin


// **** Flow Sensor Stuff ****
volatile int flowCounter;                         //measuring the rising edges of the signal
float calc_lpm;                                   // Liters Per Minute
float calc_gpm;

float heartBeatNo = 0.0;

// Suplamental Heater Stuff




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
float tankTempUnderTemp = 40.0;               // Tank Under Temp


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
DeviceAddress Tank_Temp_Addr =        {0x28, 0x45, 0xA3, 0x1C, 0x07, 0x00, 0x00, 0xAD};
DeviceAddress Shop_Temp_Addr =        {0x28, 0xB8, 0x3D, 0x29, 0x07, 0x00, 0x00, 0x49};
DeviceAddress Attic_Temp_Addr =       {0x28, 0x30, 0x12, 0x29, 0x07, 0x00, 0x00, 0x95};
DeviceAddress Lower_Inlet_Temp_Addr = {0x28, 0x9B, 0x44, 0x1D, 0x07, 0x00, 0x00, 0x7E};
// DeviceAddress Lower_Inlet_Temp_Addr = {0x28, 0x37, 0x49, 0x29, 0x07, 0x00, 0x00, 0xF5}; // real snsor

// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega
/* 4 extra Temp Sensors
  Device Address: 2830122907000095 Temp C: 7.50 Temp F: 45.50
  Device Address: 28B83D2907000049 Temp C: 19.50 Temp F: 67.10
  Device Address: 2845A31C070000AD Temp C: 13.00 Temp F: 55.40
  Device Address: 2845A31C070000AD Temp C: 13.00 Temp F: 55.40
  Device Address: 2845A31C070000AD Temp C: 13.00 Temp F: 55.40
  Device Address: 2845A31C070000AD Temp C: 13.00 Temp F: 55.40

    Device A Address: 2880EA290700000A
    Device B Address: 283AAC2A0700003F
    Device C Address: 28D15E2907000043
    Device D Address: 28374929070000F5

*/

/*
  //Temperature Thresholds
  const int systemDiffOn = SYSTEM_DIFF_ON;
  const int systemDiffOff = SYSTEM_DIFF_OFF;
  const int systemOverheat = SYSTEM_OVERHEAT;
  float systemDifference = - 20.0;
*/

/*
  // **** Timers ****
  const int minPumpOnMilliSec = (MINIMUM_PUMP_ON_SECONDS * 1000);
  const int tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;
  const int minimumFanOnMs = 60000;
  const int fanDelayMs = 10000;
  const int loopSleepTimerMs = 10000;
  unsigned long previousMillis = 0;
  unsigned long currentMillis = 0;
*/
/*
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
*/

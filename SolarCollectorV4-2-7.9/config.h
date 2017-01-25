//Communications
//#define SERIAL_BPS 115200

//Temperature Thresholds
#define SYSTEM_DIFF_ON 1  
#define SYSTEM_DIFF_OFF 0
float SYSTEM_OVERHEAT = 190.0;

// **** Tank Pressure Sensor Pin ****
const byte tank_Pump_Pressure_Pin = A15;

// **** Relay Pins ****
const byte tankPumpPin = 20;

// **** Flow Sensor Stuff ****
volatile int flowCounter;                     //measuring the rising edges of the signal
const byte flowSensorPin = 21;                 //The pin location of the sensor
float calc_lpm;                                 // Liters Per Minute
float calc_gpm; 

// Pin For Test Alarm, Turn Off Alarm etc.
const byte buttonPin = A5;         // the number of the pushbutton pin
//Panel Pump
// MINIMUM_PUMP_ON_SECONDS = 120;     // 2 Minutes or 120000 Milli Seconds

// Thermostat
const byte set_Temp_Pot_Pin = A4;  // Analog Pin for Thermostat Pot

// Blower Fan and Pump Pin
const byte HVAC_Fan_Pump_Pin = 23;
const byte Set_Temp_Pot_Pin = A14;

// Attic Fan Pin
const byte attic_Fan_Pin = 27;
// Attic Louver Pin
const byte attic_Louver_Pin = 25;
// Attic Fan Delay
const int fanDelayMs = 10000;
#define attic_Louver_Open LOW
#define attic_Louver_Closed HIGH
#define attic_Fan_On LOW
#define attic_Fan_Off HIGH

// Storage Tank Heater Pin 
const byte tank_Heater_Pin = 26;
float tankTempUnderTemp = 40.0;
 
// Solar Panel Pump Pressure Sensor
// #define tank_Pump_Pressure_Pin A15

// Motion Sensor Pin 
const byte motion_Sensor_Pin = 8;

// Solar Panel Pump Flow Sensor
const byte tank_Pump_Flow_Pin = 21;

//Diagnostic LED
const byte run_Time_Pin= 27;
const byte general_Alarm_Pin= 9;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;                    // the last time the output pin was toggled
long debounceDelay = 50;                      // the debounce time; increase if the output flickers


//  General Alarm State
byte errorState = 0;
byte errorOverTemp = 1;
byte errorTempGetFail = 2;
byte errorLowTankPumpPressure = 3;
byte errorHighTankPumpPressure = 4;
byte errorLowTankPumpFlow = 5;
byte errorHighTankPumpFlow = 6;
byte errorLowHVACPumpPressure = 7;
byte errorHighHVACPumpPressure = 8;
byte errorLowHVACPumpFlow = 9;
byte errorHighHVACPumpFlow = 10;
byte errorTankUnderTemp = 11;
byte errorTestButton = 12;

// Define Sensors and Controllers
const byte TEMP_BUS_PIN = 22;
#define TEMP_POLLING_DELAY_SECONDS 3
#define SOLAR_PANEL_TEMP_NAME "Solar_Panel_Temp"
#define TANK_TEMP_NAME "Tank_Temp"
#define SHOP_TEMP_NAME "Shop_Temp"
#define ATTIC_TEMP_NAME "Attic_Temp"
#define TANK_PUMP_NAME "Tank_Pump"
#define TANK_PUMP_PRESSURE_NAME "Tank_Pump_Pressure"
#define TANK_PUMP_FLOW_NAME "Tank_Pump_Flow"
#define ATTIC_FAN_NANE "Attic_Fan";
#define ATTIC_LOUVER_NAME "Attic_Louver";


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
float hvacSetPoint = 0.0;                     // Float Value of Thermostate Setting
float tempF = 0.0;

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



// **** Hex Addresses of Dallas Temp Sensors ****
DeviceAddress Solar_Panel_Temp_Addr = {0x28, 0x30, 0x12, 0x29, 0x07, 0x00, 0x00, 0x95};
DeviceAddress Tank_Temp_Addr =        {0x28, 0x45, 0xA3, 0x1C, 0x07, 0x00, 0x00, 0xAD};
DeviceAddress Shop_Temp_Addr =        {0x28, 0xB8, 0x3D, 0x29, 0x07, 0x00, 0x00, 0x49};
DeviceAddress Attic_Temp_Addr =       {0x28, 0x9B, 0x44, 0x1D, 0x07, 0x00, 0x00, 0x7E};

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
const int minimumPumpOnMilliSec = (MINIMUM_PUMP_ON_SECONDS * 1000);
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

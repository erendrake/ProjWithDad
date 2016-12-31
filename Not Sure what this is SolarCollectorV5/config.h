//Communications
#define SERIAL_BPS 115200

//Temperature Thresholds
#define SYSTEM_DIFF_ON 5  
#define SYSTEM_DIFF_OFF 0
#define SYSTEM_OVERHEAT 160

//Temp Sensors
#define TEMP_BUS_PIN 3

//Panel Pump
const int TANK_PUMP_PIN = 4;
#define MINIMUM_PUMP_ON_SECONDS 120

// Blower Fan and Pump Pin
const int HVAC_Blower_Fan_and_Pump = 6;

// Attic Fan Pin
const int attic_Fan_Pin = 8;

// Attic Louver Pin
const int attic_Louver_Pin = 7;

// Storage Tank Heater Pin 
const int tank_Heater_Pin = 5;

// Solar Panel Pump Pressure Sensor
#define tank_Pump_Pressure_Pin A1

// Solar Panel Pump Flow Sensor
const int tank_Pump_Flow_Pin = 2;

//Diagnostic LED
const int run_Time_Pin= 6;
const int general_Alarm_Pin= 9;

//Timers
#define LOOP_SLEEP_TIMER_SECONDS 10
#define MINIMUM_FAN_ON_SECONDS 60
#define FAN_DELAY_SECONDS 5
#define TEMP_POLLING_DELAY_mlSECONDS 1000 
/*// Define Sensors and Controllers
const int TEMP_BUS_PIN = 3;
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

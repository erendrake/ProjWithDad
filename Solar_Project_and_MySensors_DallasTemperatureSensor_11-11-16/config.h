//Communications
#define SERIAL_BPS 115200

//Temperature Thresholds
#define SYSTEM_DIFF_ON 5  
#define SYSTEM_DIFF_OFF 0
#define SYSTEM_OVERHEAT 160

//Panel Pump
#define PANEL_PUMP_PIN 4
#define MINIMUM_PUMP_ON_SECONDS 120

// Blower Fan and Pump Pin
#define Blower_Fan_and_Pump 6

// Attic Fan Pin
#define Attic_Fan_Pin 7

// Attic Louver Pin
#define Attic_Louver_Pin 8

//Heater Pump
#define HEATER_PUMP_PIN 5

// Pump Pressure Sensor
#define Pressure_Sensor_Pin A1

// Pump Flow Sensor
#define Pump_Flow_Sensor_Pin 0


//Diagnostic LED
#define RUNTIME_LED_PIN 6
#define GENERAL_ALARM_LED_PIN 9

//Temp Sensors
#define TEMP_BUS_PIN 3
#define TEMP_POLLING_DELAY_SECONDS 3
#define TEMP_PANEL_SENSOR_NAME "Panel Sensor"
#define TEMP_TANK_SENSOR_NAME "Tank Sensor"
getTempByIndex(0) = 0x28, 0x9B, 0x44, 0x1D, 0x07, 0x00, 0x00, 0x7E);
getTempByIndex(1) = 0x28, 0x45, 0xA3, 0x1C, 0x07, 0x00, 0x00, 0xAD);
/*
Solar Tank Sensor Device Address: 2845A31C070000AD
Solar Panel Sensor Device Address: 289B441D0700007E
Attic Temp Sensor Device Address: 28D95E29070000E2
Shop Temp Sensor Device Address: 28D15E2907000043
Format: insideThermometer = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };
*/

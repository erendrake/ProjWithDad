//Communications
#define SERIAL_BPS 19200

//Temperature Thresholds
#define SYSTEM_DIFF_ON 10
#define SYSTEM_DIFF_OFF -5
#define SYSTEM_OVERHEAT 160

//Panel Pump
#define PANEL_PUMP_LED_PIN 7
#define PANEL_PUMP_PIN 4
#define MINIMUM_PUMP_ON_SECONDS 120

//Heater Pump
#define HEATER_PUMP_LED_PIN 8
#define HEATER_PUMP_PIN 5

//Diagnostic LED
#define GENERAL_ALARM_LED_PIN 9
#define RUNTIME_LED_PIN 6

//Temp Sensors
#define TEMP_BUS_PIN 3
#define TEMP_POLLING_DELAY_SECONDS 10
#define TEMP_PANEL_SENSOR_ADDRESS { 0x28, 0xB6, 0xE6, 0x1D, 0x07, 0x00, 0x00, 0x32 }
#define TEMP_TANK_SENSOR_ADDRESS { 0x28, 0xAE, 0x9F, 0x2A, 0x07, 0x00, 0x00, 0x98 }



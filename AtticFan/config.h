//Communications
#define MAX_ATTACHED_DS18B20 16
#define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No

//Temperature Thresholds
#define SYSTEM_DIFF_ON 10
#define SYSTEM_DIFF_OFF 1

//Attic Fan
#define ATTIC_FAN_PIN 4
#define INTAKE_LOUVER_PIN 5

//Diagnostic LED
#define RUNTIME_LED_PIN 6
#define GENERAL_ALARM_LED_PIN 9

//Temp Sensors
#define TEMP_BUS_PIN 3

//Timers
#define LOOP_SLEEP_TIMER_SECONDS 30
#define MINIMUM_FAN_ON_SECONDS 60
#define FAN_DELAY_SECONDS 15

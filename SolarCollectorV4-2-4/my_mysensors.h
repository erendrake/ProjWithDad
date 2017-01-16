// My MySensors Stuff

#inndef my_mysensors_h
// #ifndef my_mysensors_h
#define my_mysensors_h
#define LIBRARY_VERSION 1.0.0
#include <MySensors.h>

//void define_child_ids()
//{
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
//}
// **** Initialize sensor message to MySensors Gateway ****
//void messages()
//{
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
//}

//void present_sensors()
//{
// Send the sketch version information to the gateway and Controller
  sendSketchInfo("SolarHeaterControllerV4-2-4", "RW/CW 1/16/17");
  // Fetch the number of attached temperature sensors
  numSensors = Number_Temp_Sensors;
  // Present all sensors to controller
  // Array for storing temps
  present(Solar_Panel_Temp_ID, S_TEMP, "PanelTemp");            // Panel Temp Child ID 0
  delay(100);
  present(Tank_Temp_ID, S_TEMP, "TankTemp");                    // Tank Temp Child ID 1
  delay(100);
  present(Shop_Temp_ID, S_TEMP, "ShopTemp");                    // Shop Temp Child ID 2
  delay(100);
  present(Attic_Temp_ID, S_TEMP, "AtticTemp");                  // Attic Temp Child ID 3
  delay(100);
  present(Tank_Heater_ID, S_BINARY, "TankHeater");              // Tank Heater Child ID 15
  delay(100);
  present(Tank_Pump_ID, S_BINARY, "TankPump");                  // Tank Pump Child ID 10
  delay(100);
  present(Tank_Pump_Pressure_ID, S_CUSTOM, "PumpPress");        // Panel Pump Pressure Child ID 4
  delay(100);
  present(Tank_Pump_Flow_ID, S_WATER, "PumpFlow");              // Panel Pump Flow Child ID 6
  delay(100);
  present(Attic_Fan_ID, S_BINARY, "AtticFan");                  // Attic Fan Child ID 13
  delay(100);
  present(Attic_Louver_ID, S_BINARY, "AtticLouver");            // Attic Louver Child ID 14
  delay(100);
  present(General_Alarm_ID, S_BINARY, "GeneralAlarm");         // General Alarm Over Heat and Test Button
  delay(100);
  present(Test_Button_ID, S_BINARY, "TestButton");             // Test Button Sets off the General Alarm
  delay(100);
  present(HVAC_Set_Point_ID, S_HEATER, "TempSetPoint");
  delay(100);
  present(Motion_Sensor_ID, S_MOTION, "Motion Sensor");
//}
#endif;

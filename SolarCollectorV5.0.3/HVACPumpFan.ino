// Solar Project
// HVAC Pump and Fan
// Pump Between the Storage Tank and the Heat Exchanger on Top of the Furnace Blower(Fan)

void processHVACPumpFan()
{
  Serial.println("processHVACPumpFan.");
  // See if We Need to and Can Turn on the HVAC Pump and Fan

  if ((heating == true) && ((currentTankTemp + 2.0) > currentShopTemp) && (currentShopTemp <= hvacSetPoint))
  {
    Serial.println("HVAC Pump and Fan are Already On");
    return;
  }
  if ((heating != true) && ((currentTankTemp + 5.0) > currentShopTemp) && ((currentShopTemp - 2.0) <= hvacSetPoint))
  {
    hvacPumpFanOn();
    return;
  }
  // unsigned long hvacFanPumpTurnOnTime = 0;
  unsigned long currentHVACFanPumpOnTime = (millis() - hvacFanPumpTurnOnTime);  // Check How Long the Fan and Pump Have Been Running
  if (shopTemp >= (hvacSetPoint + 2.0) && (currentHVACFanPumpOnTime >= minPumpOnTime) && heating == true)
  {
    hvacPumpFanOff;
  }

}

void hvacPumpFanOn()
{
  digitalWrite(HVAC_Fan_Pump_Pin, hvacFanPumpOn);             // Turn on HVAC Pump and Fan if shopTemp <= setPoint - 2
  hvacFanPumpTurnOnTime = millis();                           // Start the 2 Minute Timer
  Serial.println("HVAC Pump and FAN are ON!");
  heating = true;
  send(msg_HVAC_Fan_Pump.setSensor(HVAC_Fan_Pump_ID).set(1));
//    send(msg_solar_panel_temp.setSensor(Solar_Panel_Temp_ID).set(panelTemp, 1));
}

void hvacPumpFanOff()
{
  digitalWrite(HVAC_Fan_Pump_Pin, hvacFanPumpOff);          // Turn HVAC Pump and Fan if off if shopTemp >= setPoint + 2
  Serial.println("HVAC Pump and FAN are Off");
  heating = false;
  send(msg_HVAC_Fan_Pump.setSensor(HVAC_Fan_Pump_ID).set(0));
}


/*
 * // Define Child ID's for MySensors GW
const uint8_t Number_Child_IDs           =   18; // Number of Child ID's
const uint8_t Number_Temp_Sensors        =    5; // Panel Temp Child ID
const uint8_t Solar_Panel_Temp_ID        =    0; // Panel Temp Child ID
const uint8_t Tank_Temp_ID               =    1; // Tank Temp Child ID
const uint8_t Shop_Temp_ID               =    2; // Shop Temp Child ID
const uint8_t Attic_Temp_ID              =    3; // Panel Pump Flow Child ID
const uint8_t Lower_Panel_Inlet_Temp_ID  =    4; // Solar Panel Lower Inlet Temp Sensor
const uint8_t Tank_Pump_ID               =   10; // Panel Pump Child ID
const uint8_t Tank_Pump_Pressure_ID      =   11; // Panel Pump Pressure Child ID
const uint8_t Tank_Pump_Flow_ID          =   12; // Panel Pump Flow Child ID
const uint8_t Attic_Fan_ID               =   13; // Attic Fan on/off
const uint8_t Attic_Louver_ID            =   14; // Attic Louver open/closed
const uint8_t Attic_Gable_Louver_ID      =   15; // Attic Gable Louver South End (for summer cooling)
const uint8_t Tank_Heater_ID             =   16; // Storage Tank Heater on/off
const uint8_t HVAC_Fan_Pump_ID           =   20; // Pump and Fan for HVAC
const uint8_t General_Alarm_ID           =   30; // Alarms to Vera and Local Buzzer
const uint8_t Test_Button_ID             =   31; // Test Button
const uint8_t HVAC_Set_Point_ID          =   32; // Pot to Control Shop Temp (controls HVAV Pump and Fan)
const uint8_t Motion_Sensor_ID           =   33; // Moton Sensor for Alarm
const uint8_t Summer_Winter_Mode_ID      =   41; // Summer Winter Mode Switch


// **** Initialize sensor message to MySensors Gateway ****
MyMessage msg_solar_panel_temp(Solar_Panel_Temp_ID, V_TEMP);
MyMessage msg_tank_temp(Tank_Temp_ID, V_TEMP);
MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
MyMessage msg_attic_temp(Attic_Temp_ID , V_TEMP);
MyMessage msg_lower_inlet_temp(Lower_Panel_Inlet_Temp_ID, V_TEMP);
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
//  MyMessage msg_shop_sup_heater(Shop_Sup_Heater_PID, V_HVAC_FLOW_STATE);
MyMessage msg_summer_wintermode(Summer_Winter_Mode_ID, V_STATUS);
//  MyMessage msg_Sup_Heater_Status(Shop_Sup_Heater_CID, V_STATUS);   //
//  heaterStatus.setDestination(Shop_Sup_Heater_PID);
//  MyMessage msg_Sup_Heater_Control(HEATER_CHILD_CONTROL_ID, V_STATUS);   //
//  heaterControl.setDestination(Shop_Sup_Heater_CID);
  #define hvacFanPumpOn LOW
  #define hvacFanPumpOff HIGH
  #define hvacPumpOn LOW
  #define hvacPumpOff HIGH
  #define hvacFanOn LOW

*/
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

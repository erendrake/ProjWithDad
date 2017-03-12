// Solar Project
// Get Thermostat Setting

void thermostatSetting()
{
//  Serial.println("Thermostat Setting.");
  potValue = analogRead(set_Temp_Pot_Pin);
  Serial.print("Pot Value= ");
  Serial.println(potValue);                // Can be Anything From 0-1023
  hvacSetPoint = (float(potValue) * 0.039) + 40.0;
  intHVACSetPoint = int(hvacSetPoint);
  Serial.print("Thermostat Setting= ");
  Serial.println(intHVACSetPoint);
    send(msg_set_temp_pot_heat.setSensor(HVAC_Set_Point_ID).set(hvacSetPoint, 1));
    send(msg_set_temp_pot_heat.setSensor(HVAC_Set_Point_ID).set(hvacSetPoint, 1));
    // MyMessage msg_set_temp_pot_heat(HVAC_Set_Point_ID, V_HVAC_SETPOINT_HEAT);
// MyMessage msg_set_temp_pot_cool(HVAC_Set_Point_ID, V_HVAC_SETPOINT_COOL);
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

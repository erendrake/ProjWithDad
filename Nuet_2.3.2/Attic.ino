

void processAtticFan()
{
  Serial.println("Process Attic Fan");
  Serial.print("Attic Temp= ");
  Serial.println(atticTemp, 1);
  Serial.print("Tank Temp= ");
  Serial.println(tankTemp, 1);
  float atticTempDiff = (currentAtticTemp - currentTankTemp);
  Serial.print("CurrentAtticTemp - CurrentTankTemp= ");
  Serial.println(atticTempDiff, 1);

  if (atticTempDiff <= systemDiffOff);
  {
    atticFanLouverOff();
    return;
  }
  if ((atticTempDiff > systemDiffOff) && (atticFanStatus == true))
  {
    Serial.println("Attic FanLouver is All Ready On");
    return;
  }
  else if (atticTempDiff >= systemDiffOn);
  {
    atticFanLouverOn();
  }
}


// **** Turn On the Stuff Upstairs ****
void atticFanLouverOn()
{
  digitalWrite(atticLouverPin, LOW);                    //  Open Attic Louvers
  Serial.println("Attic Louvers are Open");
  atticLouverStatus = true;
  //wait for Louver to open
  //  delay(atticFanDelayMs);
  //    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(1));             //  Send GW New Status
  //  delay(500);
  digitalWrite(atticFanPin, LOW);                            // Turn On Attic Fan
  Serial.println("Attic Fan is On");
  atticFanStatus = true;
  //    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(1));                   //  Send GW New Status
}

// **** Turn Off the Stuff Upstairs ****
void atticFanLouverOff()
{
  digitalWrite(atticFanPin, HIGH);                           // Turn Off Fan
  Serial.println("Attic Fan is Off");
  atticFanStatus = false;
  //    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(0));                   //  Send GW New Status
  //wait for fan to stop
  //  delay(atticFanDelayMs);
  digitalWrite(atticLouverPin, HIGH);                  //  Close Attic Louvers
  Serial.println("Attic Louvers are Closed");
  atticLouverStatus = false;
  //    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(0));             //  Send GW New Status
}
/*
  #define systemDiffOn 5.0                              // 5°
  #define systemDiffOff 0.0                             // 0°
    // Digital
  const uint8_t motion_Sensor_Pin = 8;                 // Motion Sensor Pin
  const uint8_t general_Alarm_Pin = 9;                 // Alarm Buzzer
  const uint8_t atticGableLouverPin = 16;              // Attic Gable Louver Pin
  const uint8_t summerWinterModePin = 17;              // * Summer/Winter Mode Switch
  const uint8_t tankPumpPin = 20;                      // * Tank Pump Pin
  const uint8_t flowSensorPin = 21;                    // * Tank Pump Flow Sensor Pin Interrupt # 2 on Mega
  const uint8_t HVAC_Fan_Pump_Pin = 23;                // * HVAC Blower Fan and Pump Pin
  // ONE_WIRE_BUS = 24;                                // * Dallas Temp Sensors Pin Defined in Main Sketch
  const uint8_t atticLouverPin = 25;                   // * Attic Louver Pin
  const uint8_t tank_Heater_Pin = 26;                  // * Storage Tank Heater Pin
  const uint8_t atticFanPin = 27;                      // * Attic Fan Pin
  const uint8_t cupolaFanPin = 28;
*/
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

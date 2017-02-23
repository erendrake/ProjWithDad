// Sola Project
// HVAC Pump and Fan
// Pump Between the Storage Tank and the Heat Exchanger on Top of the Furnace Blower(Fan)

void processHVACPumpFan()
{
  Serial.println("processHVACPumpFan.");
  // See if We Need to and Can Turn on the HVAC Pump and Fan
  if ((currentTankTemp > currentShopTemp) && (currentShopTemp <= hvacSetPoint))
  {
    digitalWrite(HVAC_Fan_Pump_Pin, hvacFanPumpOn);           //  Turn on HVAC Pump and Fan if shopTemp <= setPoint - 2
    hvacFanPumpOnTime = millis();                           // Start the 2 Minute Timer
    Serial.println("HVAC Pump and FAN are ON!");
    heating = true;
    return;
  }
  currentHVACFanPumpOnTime = millis() - hvacFanPumpOnTime;  // Check How Long the Fan and Pump Have Been Running
  if (shopTemp >= (hvacSetPoint + 2.0) && (currentHVACFanPumpOnTime >= minPumpOnTime) && heating == true);
  {
    digitalWrite(HVAC_Fan_Pump_Pin, hvacFanPumpOff);          // Turn HVAC Pump and Fan if off if shopTemp >= setPoint + 2
    Serial.println("HVAC Pump and FAN are Off");
    heating = false;
  }
}
/*
  #define hvacFanPumpOn LOW
  #define hvacFanPumpOff HIGH
  #define hvacPumpOn LOW
  #define hvacPumpOff HIGH
  #define hvacFanOn LOW
  #define hvacFanOff HIGH
  if ((systemDifference > systemDiffOn) && (currentLowerInletTemp >= 40.0) && tankPump == false)
  {
    digitalWrite(tankPumpPin, LOW);
    pumpOnTime = millis();                                // Start the 2 Min Clock
    Serial.println("Tank Pump: On!");
    tankPump = true;
  }
  // Check to See if the 2 min. Clock has Run Out
  currentPumpOnTime = millis() - pumpOnTime;              // Compute How Long the Pump Has Been Running
  // If the 2 Min Clock Has Run Out and the Solar Panel Temp Has Dropped Below or = to the Tank Temp Turn Off the Pump
  if (tankPump == true && ((currentPumpOnTime >= minPumpOnTime) && (systemDifference <= systemDiffOff)));
  {
      digitalWrite(tankPumpPin, HIGH);
      Serial.println("tankPump: Off!");
      tankPump = false;
  }
*/
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

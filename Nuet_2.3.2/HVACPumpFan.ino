// Sola Project
// HVAC Pump and Fan
// Pump Between the Storage Tank and the Heat Exchanger on Top of the Furnace Blower(Fan)

void processHVACPumpFan()
{
  Serial.println("processHVACPumpFan.");
  // See if We Need to and Can Turn on the HVAC Pump and Fan
  
  if ((heating == true) && ((currentTankTemp) > currentShopTemp) && (currentShopTemp <= hvacSetPoint))
    {
      Serial.println("HVAC Pump and Fan are Already On");
      return;
    }
  if ((heating != true) && (currentTankTemp > currentShopTemp) && ((currentShopTemp-2.0) <= hvacSetPoint))
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
  digitalWrite(HVAC_Fan_Pump_Pin, hvacFanPumpOn);           //  Turn on HVAC Pump and Fan if shopTemp <= setPoint - 2
  hvacFanPumpTurnOnTime = millis();                           // Start the 2 Minute Timer
  Serial.println("HVAC Pump and FAN are ON!");
  heating = true;
}

void hvacPumpFanOff()
  {
    digitalWrite(HVAC_Fan_Pump_Pin, hvacFanPumpOff);          // Turn HVAC Pump and Fan if off if shopTemp >= setPoint + 2
    Serial.println("HVAC Pump and FAN are Off");
    heating = false;
  }


/*
  #define hvacFanPumpOn LOW
  #define hvacFanPumpOff HIGH
  #define hvacPumpOn LOW
  #define hvacPumpOff HIGH
  #define hvacFanOn LOW

*/
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

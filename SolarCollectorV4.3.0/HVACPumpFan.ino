// Sola Project
// HVAC Pump and Fan
// Pump Between the Storage Tank and the Heat Exchanger on Top of the Furnace Blower(Fan)

void processHVACPumpFan()
{
  // See if We Need to and Can Turn on the HVAC Pump and Fan
  if (((currentTankTemp + 3.0) >= hvacSetPoint) && (shopTemp <= (hvacSetPoint - 2.0)))
  {
  digitalWrite(HVAC_Fan_Pump_Pin, LOW);           //  Turn on HVAC Pump and Fan if shopTemp <= setPoint - 2
    heating = true;
    Serial.println("Heater On");
  }
  else if (shopTemp >= (hvacSetPoint + 2.0))
  {
  digitalWrite(HVAC_Fan_Pump_Pin, HIGH);          // Turn HVAC Pump and Fan if off if shopTemp >= setPoint + 2
    heating = false;
    Serial.println("Heater Off");
  }
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega   

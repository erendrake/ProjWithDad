
// HVACPumpFan

// **** Start of Process HVAC Pump and Fan ****
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

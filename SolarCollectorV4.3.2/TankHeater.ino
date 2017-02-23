// Solar Project
// Sketch to Turn On Tank Heater if Shop Gets To Cold

void  processTankHeater()
{
   Serial.println("We made it to processTankHeater.");

  if (currentTankTemp <= tankTempUnderTemp)            // Turn On Tank Heater If the W
  {
    digitalWrite(tank_Heater_Pin, LOW);
    Serial.println("Tank Heater Turned On");
    // set alarm state
  }
  else
  {
    Serial.println("Tank Heater Is Off");
    digitalWrite(tank_Heater_Pin, HIGH);
    // reset alarm state
  }
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega 

// **** Get Thermostat Setting ****
void thermostatSetting()
{
  potValue = analogRead(set_Temp_Pot_Pin);
  Serial.print("Pot Value=");
  Serial.println(potValue);                // Can be Anything From 0-1023
  hvacSetPoint = (float(potValue) * 0.039) + 40.0;
  intHVACSetPoint = int(hvacSetPoint);
  Serial.println();
  Serial.print("Temp Set Point=");
  Serial.println(intHVACSetPoint);
  Serial.println();
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega 

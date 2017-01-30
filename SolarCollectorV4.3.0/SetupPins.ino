// **** Setup Pins ****
void setupPins()
{
  // Digital
  pinMode(tankPumpPin, OUTPUT);
  digitalWrite(tankPumpPin, HIGH);
  pinMode(tank_Heater_Pin, OUTPUT);
  digitalWrite(tank_Heater_Pin, HIGH);
  pinMode(HVAC_Fan_Pump_Pin, OUTPUT);
  digitalWrite(HVAC_Fan_Pump_Pin, HIGH);
  pinMode(attic_Fan_Pin, OUTPUT);
  digitalWrite(attic_Fan_Pin, HIGH);
  pinMode(attic_Louver_Pin, OUTPUT);
  digitalWrite(attic_Louver_Pin, HIGH);
  pinMode(general_Alarm_Pin, OUTPUT);
  digitalWrite(general_Alarm_Pin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(buttonPin, HIGH);
  pinMode(motion_Sensor_Pin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(flowSensorPin, INPUT_PULLUP);              //initializes digital pin 21 ON Mega as an input
  pinMode(summerWinterModePin, INPUT_PULLUP);
    // Analog
  pinMode(tank_Pump_Pressure_Pin, INPUT);
  pinMode(set_Temp_Pot_Pin, INPUT);
  pinMode(interruptLED, OUTPUT);
  digitalWrite(interruptLED, HIGH);
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega 

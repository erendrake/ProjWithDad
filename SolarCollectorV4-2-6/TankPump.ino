// **** Start of processTankPump ****
void processTankPump()
{
  unsigned long currentMillis = millis();    // minimumPumpOnMilliSec 120000
  // #define MINIMUM_PUMP_ON_SECONDS 120
  // unsigned long interval = minimumPumpOnMilliSec; // the time we need to wait
  // unsigned long previousMillis=0; // millis() returns an unsigned long.
  systemDifference = currentPanelTemp - currentTankTemp;
  // minimumPumpOnSeconds = 120

  if (currentTankTemp > systemOverheat)
  {
    Serial.println("System Over Temp and Panel Pump: Off!");
    digitalWrite(tankPumpPin, HIGH);
    tankPump = false;
    errorState = errorOverTemp;
    return;
  }
  if (systemDifference > systemDiffOn && tankPump == false)
  {
    digitalWrite(tankPumpPin, LOW);
    Serial.println("Tank Pump: On!");
    tankPump = true;
    // Start the 2 min. Clock
    unsigned long currentMillis = millis();
  }
  // Check to See if the 2 min. Clock has Run Out

  else if (tankPump == true && ((unsigned long)(currentMillis - previousMillis) >= minimumPumpOnMilliSec))
  {
    if (systemDifference < systemDiffOff && tankPump)
    {
      digitalWrite(tankPumpPin, HIGH);
      Serial.println("tankPump: Off!");
      tankPump = false;
    }
  }
}
// **** Read Tank Pump Pressure ****
// Read Pump Pressure Sensor, Check to see if it's changed
void readTankPumpPressure()
{
  // if after flow startup timer has expired and the flow is still to low add something Here!
  float rawSensorValue = analogRead(tank_Pump_Pressure_Pin);    // Read Pin A15 Value range 0-1023
  float voltage = rawSensorValue * (5.0 / 1023.0);
  Serial.print("Raw Pressure Value= ");
  Serial.println(rawSensorValue, 1);
  Serial.print("Voltage= ");
  Serial.println(voltage, 1);
  tankPumpPressure = ((voltage - 111) / 7); // Should be in PSI
  currentTankPumpPressure = tankPumpPressure;
  Serial.print("Pump_Pressure= ");
  Serial.print(tankPumpPressure, 1);
  Serial.println(" PSI"); //
  Serial.println ();
  send(msg_tank_pump_pressure.setSensor(Tank_Pump_Pressure_ID).set(currentTankPumpPressure, 1));
  // Add Later
  // If Pressure has changed send in the the new Pump Pressure
  //    send(msg.setSensor(numSensors+2).set(pumpPressure,1));
  // if pressure is <?? PSI shut pump off and set an alarm
}

// **** Tank Pump Flow ****
void readTankPumpFlow ()
{
  Serial.print("Flow Counter=");
  Serial.println(flowCounter);
  Serial.println ();
  tankPumpFlowLPM = (float(flowCounter) * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour
  tankPumpFlowGPM = (tankPumpFlowLPM * .26);
  currentTankPumpFlowGPM = (tankPumpFlowGPM, 2);
  Serial.print ("Tank Pump Flow LPM= ");
  Serial.println (tankPumpFlowLPM, DEC);      // prints the number calculated above
  Serial.print ("Gallons Per Min= ");
  Serial.println (currentTankPumpFlowGPM, DEC);
  Serial.println ();
  send(msg_tank_pump_flow.setSensor(Tank_Pump_Flow_ID).set(currentTankPumpFlowGPM, 1));  
}

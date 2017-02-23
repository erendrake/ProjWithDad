// Solar Project
// Sketch to Control Tank Pump, Read Pump Pressure and Flow

void processTankPump()
{
   Serial.println("We made it to processTankPump.");
  // minTankPumpOnSeconds = 120
  // const int systemDiffOn = SYSTEM_DIFF_ON;             // 1°
  // const int systemDiffOff = SYSTEM_DIFF_OFF;           // 0°
  // const int systemOverheat = SYSTEM_OVERHEAT;          // Tank Temp >= 190°
  systemDifference = currentPanelTemp - currentTankTemp;
  Serial.print("CurrentTankTemp");
  Serial.println(currentTankTemp, 1);
  Serial.print("System Over Temp=");
  Serial.println(systemOverheat, 1);
  if (currentTankTemp > systemOverheat)
  {
    Serial.println("System Over Temp and Panel Pump: Off!");
    digitalWrite(tankPumpPin, HIGH);
    tankPump = false;
    errorState = errorOverTemp;
    return;
  }
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
}

// **** Read Tank Pump Pressure ****
// Read Pump Pressure Sensor, Check to see if it's changed
void readTankPumpPressure()
{
   Serial.println("We made it to readTankPumpPressure.");
  // if after flow startup timer has expired and the flow is still to low add something Here!
  float rawSensorValue = analogRead(tank_Pump_Pressure_Pin);    // Read Pin A15 Value range 0-1023
  float voltage = rawSensorValue * (5.0 / 1023.0);
  Serial.print("Raw Pressure Value= ");
  Serial.println(rawSensorValue, 1);
  Serial.print("Voltage= ");
  Serial.println(voltage, 1);
  tankPumpPressure = ((rawSensorValue - 111) / 7); // Should be in PSI
  currentTankPumpPressure = tankPumpPressure;
  Serial.print("Pump_Pressure= ");
  Serial.print(tankPumpPressure, 1);
  Serial.println(" PSI"); //
  Serial.println ("");
  send(msg_tank_pump_pressure.setSensor(Tank_Pump_Pressure_ID).set(currentTankPumpPressure, 1));
  wait(500);
  // Add Later
  // If Pressure has changed send in the the new Pump Pressure
  //    send(msg.setSensor(numSensors+2).set(pumpPressure,1));
  // if pressure is <?? PSI shut pump off and set an alarm
}

// **** Tank Pump Flow ****
void readTankPumpFlow ()
{
  Serial.println("We made it to readTankPumpFlow.");
  Serial.print("Flow Counter=");
  Serial.println(flowCounter);
  Serial.println ("");
  tankPumpFlowLPM = (float(flowCounter) * 60 / 5.5); //(Pulse frequency x 60) / 5.5Q, = flow rate in L/hour
  tankPumpFlowGPM = (tankPumpFlowLPM * .26);
  currentTankPumpFlowGPM = (tankPumpFlowGPM, 2);
  Serial.print ("Tank Pump Flow LPM= ");
  Serial.println (tankPumpFlowLPM, DEC);      // prints the number calculated above
  Serial.print ("Gallons Per Min= ");
  Serial.println (currentTankPumpFlowGPM, DEC);
  Serial.println ("");
  send(msg_tank_pump_flow.setSensor(Tank_Pump_Flow_ID).set(currentTankPumpFlowGPM, 1));  
  wait(500);
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega 

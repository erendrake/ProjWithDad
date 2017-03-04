// Solar Project
// Sketch for Process Alarms

void processAlarm ()
{
  Serial.println("We made it to processAlarm.");
  String errorString = String("");
  Serial.println("Error State= " + errorState);
  if (errorState = 0)
  {
    Serial.println("No Problems Here");
  }
  //
  else if (errorState = 1)
  {
    //    errorOverTemp
    Serial.println("Storage Tank Over Heat");
    errorString = ("Over Temp");
  }
  else if (errorState = 2)
  {
    //    errorTempGetFail
    Serial.println("Failed to Get Temp Reading");
    errorString = ("Temp Read Error");
  }
  else if (errorState = 3)
  {
    //    errorLowTankPumpPressure
    Serial.println("Low Tank Pump Pressure");
    errorString = ("Low Tank Pump Press");
  }
  else if (errorState = 4)
  {
    //    errorHighTankPumpPressure
    Serial.println("High Tank Pump Pressure");
    errorString = ("Hi Tank Pump Press");
  }
  else if (errorState = 5)
  {
    //    errorLowTankPumpFlow
    Serial.println("Low Tank Pump Flow");
    errorString = ("Low Tank Pump Press");
  }
  else if (errorState = 6)
  {
    //    errorHighTankPumpFlow
    Serial.println("High Tank Pump Flow");
    errorString = ("High Tank Pump Press");
  }
  else if (errorState = 7)
  {
    //    errorLowHVACPumpPressure
    Serial.println("Low HVAC Pump Pressure");
    errorString = ("Low Tank Pump Press");
  }
  else if (errorState = 8)
  {
    //    errorHighHVACPumpPressure
    Serial.println("High HVAC Pump Pressure");
    errorString = ("high Tank Pump Press");
  }
  else if (errorState = 9)
  {
    //    errorLowHVACPumpFlow
    Serial.println("Low HVAC Pump Flow");
    errorString = ("Low HVAC Pump Press");
  }
  else if (errorState = 10)
  {
    //    errorHighHVACPumpFlow
    Serial.println("High HVAC Pump Flow");
    errorString = ("High HVAC Pump Flow");
  }
  else if (errorState = 11)
  {
    //    errorTestButton
    Serial.println("Test Button Pressed");
    errorString = ("Test Button Pressed");
  }
  else
  {
    Serial.println("No Problems Here Boss");
    errorState = 0;
  }
  //  alarm (errorString);
}

// **** If Alarm turn on Buzzer and Tell Vera About It ****
void alarm(String errorString)
{
  if (errorState == 0)  // Turn off buzzer and return
  {
    digitalWrite(general_Alarm_Pin, LOW);
    generalAlarmStatus = false;
  }
  else
  {
    generalAlarmStatus = true;
    digitalWrite(general_Alarm_Pin, HIGH);
    Serial.println("There is an Error Somewhere Fix It!");
    //   send(msg_general_alarm.setSensor(General_Alarm_ID).set(errorString, 1));
  }
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

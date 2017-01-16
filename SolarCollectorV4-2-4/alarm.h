// Alarm.h

#ifndef alarm_h
#define alarm_h
#define LIBRARY_VERSION 1.0.0

//  General Alarm State
int errorState = 0;
int errorOverTemp = 1;
int errorTempGetFail = 2;
int errorLowTankPumpPressure = 3;
int errorHighTankPumpPressure = 4;
int errorLowTankPumpFlow = 5;
int errorHighTankPumpFlow = 6;
int errorLowHVACPumpPressure = 7;
int errorHighHVACPumpPressure = 8;
int errorLowHVACPumpFlow = 9;
int errorHighHVACPumpFlow = 10;
int errorTankUnderTemp = 11;
int errorTestButton = 12;

void processAlarm ()
{
  String errorString = String("");
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

// **** Test Button ****
void Test_Button ()             // Install a Switch To Turn Off Buzzer
{
  // unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  // unsigned long debounceDelay = 50;
  // unsigned long interval=1000; // the time we need to wait
  // unsigned long previousMillis=0; // millis() returns an unsigned long.
  // Test Button Will Set off Alarm
  Serial.println("Start of Test Button");
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:
  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == HIGH)
      {
        errorState = 12;
      }
      else
      {
        errorState = 0;
      }
    }
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastButtonState = reading;
  }
}

#endif

//*********( THE END )***********

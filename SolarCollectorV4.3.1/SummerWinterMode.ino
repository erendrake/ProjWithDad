// Check Summer Winter Mode Switch

void processSummerWinterMode()
{
     Serial.println("We made it to processSummerWinterMode.");
   Serial.println("But no one is home yet!");
  /*
  // unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  // unsigned long debounceDelay = 50;
  // unsigned long interval=1000; // the time we need to wait
  // unsigned long previousMillis=0; // millis() returns an unsigned long.
  // Test Button Will Set off Alarm
  Serial.println("SummerWinterMode Sketch");
  // read the state of the switch into a local variable:
  int reading = digitalRead(summerWinterModePin);
  // check to see if you just switch has changed
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:
  // If the switch changed, due to noise or pressing:
  if (reading != lastSwitchState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    // #define lastSwitchState winter
    // if the switch state has changed:

    if (reading != switchState)
    {
      switchState = reading;
    }
    if (reading == Summer)
    {
    Serial.println ("System is Set to Summer Mode");
    }
    else 
    {
    Serial.println ("System is Set for Winter Mode");
    }
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastSwitchState = reading;
  }
  */
  
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega











// Solar Project
// Sketch for Test Button

void Test_Button ()             // Install a Switch To Turn Off Buzzer
{
  Serial.println("We made it to Test_Button.");
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
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega


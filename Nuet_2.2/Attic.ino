

void processAtticFan()
{
  Serial.println("Process Attic Fan");
  Serial.print("CurrentAtticTemp - CurrentShopTemp= ");
  Serial.println((currentAtticTemp - currentShopTemp), 1);
  if (currentAtticTemp <= currentShopTemp)
  {
    atticFanLouverOff();
  }
  /*  if (atticLouverStatus == true)
    {
      Serial.println("Attic Suff is All Ready On");
      Serial.println("Attic Fan is On");
      Serial.println("Attic Louvers are Open");
    }
    else
  */
  if ((currentAtticTemp + 1) >= currentShopTemp);
  {
    Serial.println("Turn on the Stuff Upstairs");
    atticFanLouverOn();
  }
}
/* // Digital
const uint8_t motion_Sensor_Pin = 8;                 // Motion Sensor Pin
const uint8_t general_Alarm_Pin = 9;                 // Alarm Buzzer
const uint8_t atticGableLouverPin = 16;              // Attic Gable Louver Pin
const uint8_t summerWinterModePin = 17;              // * Summer/Winter Mode Switch
const uint8_t tankPumpPin = 20;                      // * Tank Pump Pin
const uint8_t flowSensorPin = 21;                    // * Tank Pump Flow Sensor Pin Interrupt # 2 on Mega
const uint8_t HVAC_Fan_Pump_Pin = 23;                // * HVAC Blower Fan and Pump Pin
// ONE_WIRE_BUS = 24;                             // * Dallas Temp Sensors Pin Defined in Main Sketch
const uint8_t atticLouverPin = 25;                 // * Attic Louver Pin
const uint8_t tank_Heater_Pin = 26;                  // * Storage Tank Heater Pin
const uint8_t atticFanPin = 27;                    // * Attic Fan Pin
const uint8_t cupolaFanPin = 28;    
*/

// **** Turn On the Stuff Upstairs ****
void atticFanLouverOn()
{
  digitalWrite(atticLouverPin, LOW);                    //  Open Attic Louvers
  Serial.println("Attic Louvers are Open");
  atticLouverStatus = true;
  //wait for Louver to open
  delay(atticFanDelayMs);
  //    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(1));             //  Send GW New Status
//  delay(500);
  digitalWrite(atticFanPin, LOW);                            // Turn On Attic Fan
  Serial.println("Attic Fan is On");
  atticFanStatus = true;
  //    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(1));                   //  Send GW New Status
}


// **** Turn Off the Stuff Upstairs ****
void atticFanLouverOff()
{
  digitalWrite(atticFanPin, HIGH);                           // Turn Off Fan
  Serial.println("Attic Fan is Off");
  atticFanStatus = false;
  //    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(0));                   //  Send GW New Status
  //wait for fan to stop
  delay(atticFanDelayMs);
  digitalWrite(atticLouverPin, HIGH);                  //  Close Attic Louvers
  Serial.println("Attic Louvers are Closed");
  atticLouverStatus = false;
  //    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(0));             //  Send GW New Status
  delay(500);
}

// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

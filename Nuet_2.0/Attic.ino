

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
    atticFanLouverOn;
  }
}


void atticFanLouverOn()
{
  digitalWrite(attic_Louver_Pin, LOW);                    //  Open Attic Louvers
  Serial.println("Attic Louvers are Open");
  atticLouverStatus = true;
  //wait for Louver to open
  delay(atticFanDelayMs);
  //    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(1));             //  Send GW New Status
//  delay(500);
  digitalWrite(attic_Fan_Pin, LOW);                            // Turn On Attic Fan
  Serial.println("Attic Fan is On");
  atticFanStatus = true;
  //    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(1));                   //  Send GW New Status
}

void atticFanLouverOff()
{
  digitalWrite(attic_Fan_Pin, HIGH);                           // Turn Off Fan
  Serial.println("Attic Fan is Off");
  atticFanStatus = false;
  //    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(0));                   //  Send GW New Status
  //wait for fan to stop
  delay(atticFanDelayMs);
  digitalWrite(attic_Louver_Pin, HIGH);                  //  Close Attic Louvers
  Serial.println("Attic Louvers are Closed");
  atticLouverStatus = false;
  //    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(0));             //  Send GW New Status
  delay(500);
}

// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

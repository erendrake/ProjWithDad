

void processAtticFan()
{
  Serial.println("processAtticFan");
  systemDifference = currentAtticTemp - currentShopTemp;
  while (systemDifference > systemDiffOn)
  {
  atticFanLouverOn();
  }
  if ((atticLouverStatus == true) && (currentAtticTemp > currentShopTemp))
  {
  atticFanLouverOn();
  }
  else 
  atticFanLouverOff();
}


void atticFanLouverOn()  
{
  digitalWrite(attic_Louver_Pin, attic_Louver_Open);                    //  Open Attic Louvers
  Serial.println("Attic Louvers are Open");
  atticLouverStatus = true;
  //wait for Louver to open
  delay(atticFanDelayMs);
  //    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(1));             //  Send GW New Status
  delay(500);
  digitalWrite(attic_Fan_Pin, attic_Fan_On);                            // Turn On Attic Fan
  Serial.println("Attic Fan is On");
  atticFanStatus = true;
  //    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(1));                   //  Send GW New Status
}

void atticFanLouverOff()
{
  digitalWrite(attic_Fan_Pin, attic_Fan_Off);                           // Turn Off Fan
  Serial.println("Attic Fan is Off");
  atticFanStatus = false;
  //    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(0));                   //  Send GW New Status
  //wait for fan to stop
  delay(atticFanDelayMs);
  digitalWrite(attic_Louver_Pin, attic_Louver_Closed);                  //  Close Attic Louvers
  Serial.println("Attic Louvers are Closed");
  atticLouverStatus = false;
  //    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(0));             //  Send GW New Status
  delay(500);
}
/*  Serial.print("Attic Fan is ");
  Serial.println(atticFanStatus);
  Serial.print("Attic Louvers are ");
  Serial.println(atticLouverStatus);
*/
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

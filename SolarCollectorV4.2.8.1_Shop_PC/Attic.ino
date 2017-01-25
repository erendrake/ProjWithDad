void processAtticFan(){
  Serial.println("We made it to the processAtticFan");
  float systemDifference = atticTemp - shopTemp;
  if (systemDifference > systemDiffOn && atticFanStatus == false && atticLouverStatus == false){
    digitalWrite(attic_Louver_Pin, attic_Louver_Open);                    //  Open Attic Louvers   
    atticLouverStatus = true;
    //wait for Louver to open
    wait(atticFanDelayMs);
    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(1));             //  Send GW New Status
    wait(500);
    digitalWrite(attic_Fan_Pin, attic_Fan_On);                            // Turn On Attic Fan  
    atticFanStatus = true;
    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(1));                   //  Send GW New Status
    return;
  }
  else if(systemDifference < systemDiffOff && atticFanStatus == true && atticLouverStatus == true){
    digitalWrite(attic_Fan_Pin, attic_Fan_Off);                           // Turn Off Fan  
    atticFanStatus = false;
    send(msg_attic_fan.setSensor(Attic_Fan_ID).set(0));                   //  Send GW New Status
    //wait for fan to stop 
    wait(atticFanDelayMs);    
    digitalWrite(attic_Louver_Pin, attic_Louver_Closed);                  //  Close Attic Louvers  
    atticLouverStatus = false;
    send(msg_attic_louver.setSensor(Attic_Louver_ID).set(0));             //  Send GW New Status 
    wait(500);
    return;
  }
}

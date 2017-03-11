// Solar Project
// 2 Line x 16 LCD
// Line 1 P=Solar Panel Temp, T=Storage Tank Temp, S=Shop Temp
// Line 2 A=Attic Temp, PP=Tank Pump Pressure (PSI), F=Tank Pump Flow (?LPM)
// Line 1 Postion 15 Heartbeat

void writeLCD()
{
  Serial.println("Write LCD");
  lcd.clear();
  lcd.setCursor (0, 0); // go to start of 1st line
  lcd.print("P");
  lcd.setCursor (1, 0); //
  lcd.print(currentPanelTemp, 1);
//  Serial.print("CurrentPanelTemp");
//  Serial.println(currentPanelTemp, 1);
//  Serial.println("");

  lcd.setCursor (6, 0); //
  lcd.print("T");
  lcd.setCursor (7, 0); //
  lcd.print(currentTankTemp, 1);
//  Serial.print("CurrentTankTemp");
//  Serial.println(currentTankTemp, 1);

  lcd.setCursor (12, 0); //
  lcd.print(heartBeatNo, 1);
//  Serial.print("HeartBeat");
//  Serial.println(heartBeatNo, 1);

  lcd.setCursor (0, 1); //
  lcd.print("A");
  lcd.setCursor (1, 1); //
  lcd.print(currentAtticTemp, 1);
//  Serial.print("CurrentPanelTemp");
//  Serial.println(currentPanelTemp, 1);

  lcd.setCursor (6, 1); //
  lcd.print("S");
  lcd.setCursor (7, 1);
  lcd.print(currentShopTemp, 1);;
//  Serial.print("CurrentPanelTemp");
//  Serial.println(currentPanelTemp, 1);
  
  /*  lcd.setCursor (5, 1); //
    lcd.print("PP");
    lcd.setCursor (8, 1); //
    lcd.print(int(currentTankPumpPressure));

    lcd.setCursor (11, 1);
    lcd.print ("F");
    lcd.setCursor (13, 1);
    lcd.print(currentTankPumpFlowGPM, 1);
  */

}


void writeHeartBeatNoLCD()
{
//  lcd.setCursor (12, 0); //
//  lcd.print("H");
  lcd.setCursor (12, 0); //
  lcd.print(heartBeatNo, 1);
//  Serial.print("HeartBeat");
//  Serial.println(heartBeatNo, 1);
//  lcd.setCursor (11, 1);
//  lcd.print("     ");
//  lcd.setCursor (11, 1);
//  lcd.print(heartBeatNo, 1);
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega

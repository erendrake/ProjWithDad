// Solar Project
// 2 Line x 16 LCD
// Line 1 P=Solar Panel Temp, T=Storage Tank Temp, S=Shop Temp
// Line 2 A=Attic Temp, PP=Tank Pump Pressure (PSI), F=Tank Pump Flow (?LPM)

void writeLCD()
{
  lcd.clear();
  lcd.setCursor (0, 0); // go to start of 1st line
  lcd.print("P");
  lcd.setCursor (2, 0); //
  lcd.print(int(currentPanelTemp));

  lcd.setCursor (6, 0); //
  lcd.print("T");
  lcd.setCursor (8, 0); //
  lcd.print(int(currentTankTemp));

  lcd.setCursor (10, 0); //
  lcd.print("S");
  lcd.setCursor (12, 0); //
  lcd.print(int(currentShopTemp));

  lcd.setCursor (0, 1); //
  lcd.print("A");
  lcd.setCursor (2, 1); //
  lcd.print(int(currentAtticTemp));

  lcd.setCursor (5, 1); //
  lcd.print("PP");
  lcd.setCursor (8, 1); //
  lcd.print(int(currentTankPumpPressure));

  lcd.setCursor (11, 1);
  lcd.print ("F");
  lcd.setCursor (13, 1);
  lcd.print(currentTankPumpFlowGPM, 1);
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega 


// 2 Line x 16 LCD

#infndef my_lcd_h
#define my_lcd_h
#define LIBRARY_VERSION 1.0.0
#include <LiquidCrystal.h>

// **** 2 line LCD Stuff ****
// Initialize display. Google the correct settings for your display.
// The follwoing setting should work for the recommended display in the MySensors "shop".
//   LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal lcd(10, 11, 12, 13, A0, A1, A2);

// **** OUTPUT STUFF ****
void writeLCD()
{
  Serial.println ("We made it to writeLCD");
  // Convert sensor flots to int and display
  lcd.clear();
  lcd.setCursor (0, 0); // go to start of 1st line
  lcd.print("P");
  lcd.setCursor (1, 0); //
  lcd.print(int(currentPanelTemp));

  lcd.setCursor (4, 0); //
  lcd.print("T");
  lcd.setCursor (5, 0); //
  lcd.print(int(currentTankTemp));

  lcd.setCursor (8, 0); //
  lcd.print("S");
  lcd.setCursor (9, 0); //
  lcd.print(int(currentShopTemp));

  lcd.setCursor (12, 0); //
  lcd.print("A");
  lcd.setCursor (13, 0); //
  lcd.print(int(currentShopTemp));

  lcd.setCursor (0, 1); //
  lcd.print("PP");
  lcd.setCursor (2, 1); //
  lcd.print(int(currentTankPumpPressure));

  lcd.setCursor (4, 1);
  lcd.print ("F");
  lcd.setCursor (5, 1);
  lcd.print(currentTankPumpFlowGPM, 1);
}

#endif

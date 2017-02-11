// Solar Project
// Turn on Supplemental Shop Heater If Shop Temp is Below Thermostat Setting by 0°F in Winter Setting or -4° in Summer Setting
// The Heater End Will Always Turn Off After 10min. Just In Case

void processShopSupHeater()
{
   Serial.println("We made it to Supplemental Heater Sketch.");
/*  char mode;
if (summerWinterMode = 0)
  {
    char mode[] = "Summer";
  }
  else
  {
    char mode[] = "Winter";
  }
*/   
  Serial.println("Summer/Winter Mode Switch Set to " + summerWinterMode);
  if (summerWinterMode == Summer);                   // 0 = Summer, 1 = Winter

  {
    if (currentShopTemp <= 35.0)                     // Burr Really Cold for Summer
    {
      // Send Message to Shop Sup Heater Node to Turn On
      supHeating = true;
    }
    else

    if (currentShopTemp >= 38.0)
      {
        // Send Message to Shop Sup Heater Node to Turn Off
        supHeating = false;
      }
    return;
  }
  // It Must Be Winter
  // See if we need to turn on the heater
  if (currentShopTemp >= (hvacSetPoint - 2));
  {
  // Send Message to Shop Sup Heater Node to Turn On
  supHeating = true;
  }
if (currentShopTemp <= (hvacSetPoint + 2))
  {
  // Send Message to Shop Sup Heater to Turn On
  supHeating = false;
  }
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega 

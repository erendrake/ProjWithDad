// Solar Project
// Get Each Temp Sensor Reading by Their Hex Address

float printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
//   Serial.println("We made it to Get Temp by Address.");
    tempF = sensors.getTempF(deviceAddress);
    if (tempF <= -50.00)
    {
      Serial.println("Error getting temperature  ");
      Serial.print("F: ");
      Serial.println(tempF);      
//      Serial.println("");

      tempGetFail = true;
      tempF=-127.0;
    }
    else
    {
      Serial.print("F: ");
      Serial.println(tempF);
//      Serial.println("");
      return (tempF);
    }
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega 

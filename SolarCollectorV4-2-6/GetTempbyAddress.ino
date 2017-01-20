//**** Get Each Temp Sensor Reading by Their Hex Address ****
float printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
    tempF = sensors.getTempF(deviceAddress);
    if (tempF <= -50.00)
    {
      Serial.print("Error getting temperature  ");
      tempGetFail = true;
    }
    else
    {
      Serial.print("F: ");
      Serial.print(tempF);
      return (tempF);
    }
}

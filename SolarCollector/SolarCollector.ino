#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>

const int pumpLEDPin = PUMPLEDPIN;
const int tempBusPin = TEMPBUSPIN;
const int tempPollingDelay = TEMPPOLLINGDELAY;

OneWire  ds(tempBusPin);
DallasTemperature sensors(&ds);
  
void setup() {
  pinMode(pumpLEDPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Kelvinated Online Occular Tempcontrol");
  sensors.begin();
}

void loop() {
  digitalWrite(pumpLEDPin, HIGH);
  
  Serial.print("Requesting temperatures from pin: ");
  Serial.print(tempBusPin);
  Serial.print(" ");
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.print("DONE: Found ");
  Serial.print(sensors.getDeviceCount());
  Serial.println(" temp probes");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  for (int i = 0; i < sensors.getDeviceCount(); i++) {
    Serial.print("Temperature for the device ");
    Serial.print(i +1);
    Serial.print(" (index 0) is: ");
    Serial.print(i);
    Serial.print(") is: ");
    Serial.println(sensors.getTempCByIndex(i));  
  }
  
  
  digitalWrite(pumpLEDPin, LOW);

  
  delay(tempPollingDelay);
}

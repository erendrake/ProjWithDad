#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>

const int panelPumpLedPin = PANEL_PUMP_LED_PIN;
const int panelPumpPin = PANEL_PUMP_PIN;
const int tempBusPin = TEMP_BUS_PIN;
const int tempPollingDelay = TEMP_POLLING_DELAY;
const int serialBPS = SERIAL_BPS;

OneWire  ds(tempBusPin);
DallasTemperature sensors(&ds);
  
void setup() {
  pinMode(panelPumpLedPin, OUTPUT);  
  #if SERIAL
  Serial.begin(serialBPS);
  Serial.println("Kelvinated Online Occular Tempcontrol");
  #endif
  sensors.begin();  
}

void loop() {
  digitalWrite(panelPumpLedPin, LOW);
  requestTempSensors();
  logTempSensors();  
  digitalWrite(panelPumpLedPin, HIGH);  
  delay(tempPollingDelay);
}

















void requestTempSensors(){  
  #if SERIAL
  Serial.print("Requesting temperatures from pin: ");
  Serial.print(tempBusPin);
  Serial.print(" ");  
  #endif
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  
  #if SERIAL
  Serial.print("DONE: Found ");
  Serial.print(sensors.getDeviceCount());
  Serial.println(" temp probes");   
  #endif
}

void logTempSensors(){
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  #if SERIAL
  for (int i = 0; i < sensors.getDeviceCount(); i++) {
    Serial.print("Temperature for the device ");
    Serial.print(i +1);
    Serial.print(" (index 0) is: ");
    Serial.print(i);
    Serial.print(") is: ");
    Serial.println(sensors.getTempCByIndex(i));  
  }
  #endif
}


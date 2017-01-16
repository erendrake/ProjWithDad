// Tank Pump Stuff

#ifndef tank_pump_h
#define tank_pump_h
#define LIBRARY_VERSION 1.0.0


//Temperature Thresholds
const int systemDiffOn = SYSTEM_DIFF_ON;
const int systemDiffOff = SYSTEM_DIFF_OFF;
const int systemOverheat = SYSTEM_OVERHEAT;
float systemDifference = - 20.0;


// **** Timers ****
const int minimumPumpOnMilliSec = (MINIMUM_PUMP_ON_SECONDS * 1000);
const int tempPollingDelay = TEMP_POLLING_DELAY_SECONDS;
const int minimumFanOnMs = 60000;
const int fanDelayMs = 10000;
const int loopSleepTimerMs = 10000;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// **** Start of processTankPump ****
void processTankPump()
{
  unsigned long currentMillis = millis();    // minimumPumpOnMilliSec 120000
  // #define MINIMUM_PUMP_ON_SECONDS 120
  // unsigned long interval = minimumPumpOnMilliSec; // the time we need to wait
  // unsigned long previousMillis=0; // millis() returns an unsigned long.
  systemDifference = currentPanelTemp - currentTankTemp;
  // minimumPumpOnSeconds = 120

  if (currentTankTemp > systemOverheat)
  {
    Serial.println("System Over Temp and Panel Pump: Off!");
    digitalWrite(tankPumpPin, HIGH);
    tankPump = false;
    errorState = errorOverTemp;
    return;
  }
  if (systemDifference > systemDiffOn && tankPump == false)
  {
    digitalWrite(tankPumpPin, LOW);
    Serial.println("Tank Pump: On!");
    tankPump = true;
    // Start the 2 min. Clock
    unsigned long currentMillis = millis();
  }
  // Check to See if the 2 min. Clock has Run Out

  else if (tankPump == true && ((unsigned long)(currentMillis - previousMillis) >= minimumPumpOnMilliSec))
  {
    if (systemDifference < systemDiffOff && tankPump)
    {
      digitalWrite(tankPumpPin, HIGH);
      Serial.println("tankPump: Off!");
      tankPump = false;
    }
  }
}

void readTankPumpPressure()
{
  // if after flow startup timer has expired and the flow is still to low add something Here!
  int sensorValue = analogRead(tank_Pump_Pressure_Pin);    // read the input pin Value range 0-1023
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.print("Raw Pressure Value= ");
  Serial.println(sensorValue);
  Serial.print("Voltage= ");
  Serial.println(voltage, 1);
  tankPumpPressure = ((voltage - 111) / 7); // Should be in PSI
  currentTankPumpPressure = tankPumpPressure;
  Serial.print("Pump_Pressure= ");
  Serial.print(tankPumpPressure, 1);
  Serial.println(" PSI"); //

  //  return pumpPressure;

  // If Pressure has changed send in the the new Pump Pressure
  //    send(msg.setSensor(numSensors+2).set(pumpPressure,1));
  // if pressure is <?? PSI shut pump off and set an alarm
}

// **** Read Tank Pump Pressure ****
// Read Pump Pressure Sensor, Check to see if it's changed

// **** Turn On Tank Heater if Shop Gets To Cold ****
void  processTankHeater()
{
  if ((currentTankTemp && shopTemp) < tankTempUnderTemp)            // Turn On Tank Heater If the W
  {
    digitalWrite(tank_Heater_Pin, LOW);
    Serial.println("Tank Heater Turned On");
//    alarmState
//    else
    }
}

#endif


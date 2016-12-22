 /*
 * Wiring (radio wiring on www.mysensors.org)
 * ------------------------------------
 * Arduino   RTC-Module     I2C Display 
 * ------------------------------------
 * GND       GND            GND
 * +5V       VCC            VCC
 * A4        SDA            SDA
 * A5        SCL            SCL
 *
 * http://www.mysensors.org/build/display
 *
 */

  // Enable debug prints to serial monitor
  #define MY_DEBUG 

  // Enable and select radio type attached
  #define MY_RADIO_NRF24
  #define MY_NODE_ID 26
  #include <SPI.h>
  #include <MySensors.h>  
  #include <DallasTemperature.h>
  #include <TimeLib.h> 
  #include <DS3232RTC.h>  // A  DS3231/DS3232 library
  #include <OneWire.h> 
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
  #define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 
  #define MAX_ATTACHED_DS18B20 16

  
  bool timeReceived = false;
  unsigned long lastUpdate=0, lastRequest=0;
  
  int DS18S20_Pin = 3;                     // DS18S20 Signal pin on digital 3
  int Heater_Pin = 4;                      // Shop heater relay on pin 4

  //Temperature chip i/o
  OneWire ds(DS18S20_Pin);                  // OneWire Temp Sensor on digital pin 2
  // 2 line LCD Stuff
  // Initialize display. Google the correct settings for your display. 
  // The follwoing setting should work for the recommended display in the MySensors "shop".
  LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

  int temperature = 0;
  int setPoint = 0;
  // Define Child ID's for MySensors GW
  const int Number_Child_IDs           =    2; // Number of Child ID's
  const int Number_Temp_Sensors        =    1; // Panel Temp Child ID
  const int Shop_Temp_ID               =    2; // Shop Temp Child ID
  //************** Start of void before *****************
// Start OneWire and Dallas Temp 
  OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 
  float lastTemperature[MAX_ATTACHED_DS18B20]; //?????
  int numSensors=0;  //?????
  bool receivedConfig = false;
  bool metric = false;

void before() // ??????
{
//  TRANSPORT_DEBUG(PSTR("BLARG:B\n"));
  // Startup up the OneWire library
  sensors.begin();
DeviceAddress Shop_Temp =        {0x28, 0x37, 0x49, 0x29, 0x07, 0x00, 0x00, 0xF5};
// set the resolution to tempResolution (Can be 9 to 12 bits .. lower is faster)
  const int tempResolution = 9;
  sensors.setResolution(Shop_Temp, tempResolution);
}
  
void setup() {
  Serial.begin(115200);
  pinMode(Heater_Pin,OUTPUT);             // Relay 1           output was not declared in this scope          
  digitalWrite(Heater_Pin,LOW);          // Turn off proofing box heater
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);                  // Turn off Arduino LED
  // the function to get the time from the RTC
  setSyncProvider(RTC.get);  
  // Request latest time from controller at startup
  requestTime();  
  // initialize the lcd for 16 chars 2 lines and turn on backlight
  lcd.begin(16,2); 
  Serial.println("End of Void Setup");
}
void presentation()  {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Shop Heater & RTC", "1.0");
}
  // Initialize sensor message to MySensors Gateway
  //   MyMessage msg(0,V_TEMP);
  MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);

// This is called when a new time value was received
void receiveTime(unsigned long controllerTime) {
  // Ok, set incoming time 
  Serial.print("Time value received: ");
  Serial.println(controllerTime);
  RTC.set(controllerTime); // this sets the RTC to the time from controller - which we do want periodically
  timeReceived = true;
}

void loop() {
unsigned long now = millis();

  // If no time has been received yet, request it every 10 second from controller
  // When time has been received, request update every hour
  if ((!timeReceived && (now-lastRequest) > (10UL*1000UL))
    || (timeReceived && (now-lastRequest) > (60UL*1000UL*60UL))) {
    // Request time from controller. 
    Serial.println("requesting time");
    requestTime();  
    lastRequest = now;
  }
  
  float rawTemp = getTemp();
  Serial.print("We just got the temp= ");
  Serial.println(rawTemp);
  temperature = (int(rawTemp)); 
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  // float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  float rawSetPoint=(sensorValue*0.039)+40.0;
  setPoint = (int(rawSetPoint));
  Serial.println();
  Serial.print("Temp Set Point=");
  Serial.println(int(setPoint));
  Serial.println();
  Serial.println(sensorValue);

  // See if we need to turn on the heater
  if (temperature < setPoint) {   
    digitalWrite(Heater_Pin,HIGH);            //  Turn on if Temp < setPoint
    digitalWrite(13, HIGH);                  //  Turn on the LED 
  } else {
    digitalWrite(Heater_Pin,LOW);           // Turn off if Temp >= setPoint
    digitalWrite(13, LOW);                  //  Turn off the LED 
  }
  writeLCD(setPoint, temperature);
  delay(1000);                               // delay here to slow down the output so it is easier to read and better control of heater relay  
}

float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  TemperatureSum = (TemperatureSum * 9.0)/ 5.0 + 32.0; // Convert Celsius to Fahrenheit
  return TemperatureSum;
}
void writeLCD(int setPoint, int temperature){
    // Convert sensor flots to int and display
  Serial.println("We made it to writeLCD");
  Serial.print("Temp Set Point= ");
  Serial.println(int(setPoint));
  Serial.print("Shop Temp= ");
  Serial.println(int(temperature));  
  lcd.home();
  lcd.setCursor (0,0); // go to start of 1st line
  lcd.print("                ");
  lcd.setCursor (0,1); // go to start of 1st line
  lcd.print("                ");
  lcd.setCursor (0,0); // go to start of 1st line
  tmElements_t tm;
  RTC.read(tm);
  lcd.print("Time: ");
  lcd.setCursor (6,0);
  printDigits(tm.Hour);
  lcd.print(":");
  printDigits(tm.Minute);
  lcd.print(":");
  printDigits(tm.Second);
  lcd.setCursor (0,1);
  lcd.print("TSet:");     
  lcd.setCursor (5,1); // 
  lcd.print(int(setPoint));
  lcd.setCursor (9,1); // 
  lcd.print("ST:");
  lcd.setCursor (12,1); // 
  lcd.print(int(temperature));
}
void printDigits(int digits){
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

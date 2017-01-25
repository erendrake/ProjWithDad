 /*
 * Wiring (radio and RTC wiring on www.mysensors.org)
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
  
//  const int ONE_WIRE_BUS = 3;                     // DS18S20 Signal pin on digital 3
  const int Heater_Pin = 4;                      // Shop heater relay on pin 4
  const int hysteresis = 5;
  const int tempResolution = 9;
  int temperature = 0;
  int setPoint = 0;
  // Set Default Readings
  float ShopTemp = -127.0;
  float currentShopTemp = -127.0;
  
 // Start OneWire and Dallas Temp 
  OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 
  float lastTemperature[MAX_ATTACHED_DS18B20]; //?????
  int numSensors=0;  //?????
  bool receivedConfig = false;
  bool metric = false;
  bool heating = false;


  // 2 line LCD Stuff
  // Initialize display. Google the correct settings for your display. 
  // The follwoing setting should work for the recommended display in the MySensors "shop".
  LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);



  // Define Child ID's for MySensors GW
  const int Number_Child_IDs           =    2; // Number of Child ID's
  const int Number_Temp_Sensors        =    1; // Panel Temp Child ID
  const int Shop_Temp_ID               =    1; // Shop Temp Child ID
  const int Shop_Heater_ID             =    2; // Shop Heater Child ID ** Not on current system **
  
  // Initialize sensor message
  MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
  MyMessage msg_shop_heater(Shop_Heater_ID, V_STATUS);
  

//************** Start of void before *****************
void before(){
//  TRANSPORT_DEBUG(PSTR("BLARG:B\n"));
  // Startup up the OneWire library
    sensors.begin();
  // Dallas Temp by Address ** not currently being used **
    DeviceAddress Shop_Temp =        {0x28, 0x37, 0x49, 0x29, 0x07, 0x00, 0x00, 0xF5};
  // set the resolution to tempResolution (Can be 9 to 12 bits .. lower is faster)
    sensors.setResolution(Shop_Temp, tempResolution);
}
  
void setup(){
    pinMode(Heater_Pin,OUTPUT);             //         
    digitalWrite(Heater_Pin,LOW);           // 
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

// **** Presentation ****
void presentation(){
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Shop Electric Heater", "1.0");
  present(Shop_Temp_ID, S_TEMP, "ShopTemp");                 // Shop Temp Child ID 1   
  present(Shop_Heater_ID, S_BINARY, "Heater");               // Shop Electric Heater Child ID 2 
  

}

// Hex Addresses of Dallas Temp Sensors
DeviceAddress Shop_Temp_Addr         = {0x28, 0xB8, 0x3D, 0x29, 0x07, 0x00, 0x00, 0x49};  

// **** void loop ****
void loop()
{

  // Get Time From GW
  unsigned long now = millis();
  // If no time has been received yet, request it every 10 second from controller
  // When time has been received, request update every hour
  if ((!timeReceived && (now-lastRequest) > (10UL*1000UL))
    || (timeReceived && (now-lastRequest) > (60UL*1000UL*60UL))) 
    {
    // Request time from controller. 
    Serial.println("requesting time");
    requestTime();  
    lastRequest = now;
    }
  
  //  float rawTemp = getTemp();
  // Request temperatures from Dallas sensors
      sensors.requestTemperatures();
  // Wait for sensors to resolve
  delay(1000);
  Serial.println();
  Serial.print("Number of Devices found on bus = ");  
  Serial.println(sensors.getDeviceCount());   
  Serial.print("Getting temperatures... ");  
  Serial.println();   

// **** Get Shop Temp by Address ****
    Serial.print("Shop Temperature is:   ");
    ShopTemp = printTemperature(sensors, Shop_Temp_Addr);
    Serial.println();
    currentShopTemp=ShopTemp;
    // Send GW the new Shop_Temp
    Serial.println(); 
    send(msg_shop_temp.setSensor(Shop_Temp_ID).set(ShopTemp,1));
    send(msg_shop_heater.setSensor(Shop_Heater_ID).set(heating,1));
// **** End of Get Shop Temp by Address ****  
/*  // Initialize sensor message
  MyMessage msg_shop_temp(Shop_Temp_ID, V_TEMP);
  MyMessage msg_shop_heater(Shop_Heater_ID, V_STATUS);*/
  
  Serial.print("We just got the temp= ");
  Serial.println(ShopTemp);
  temperature = (int(ShopTemp)); 
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
  if (temperature >= setPoint); {
    digitalWrite(Heater_Pin,LOW);           // Turn off if Temp >= setPoint
    digitalWrite(13, LOW);                  //  Turn off the LED
    heating=0;
  }
  if (temperature <= (setPoint-hysteresis)) {
    digitalWrite(Heater_Pin,HIGH);            //  Turn on if Temp < setPoint
    digitalWrite(13, HIGH);                  //  Turn on the LED     
    heating=1;
  }   
  writeLCD(setPoint, temperature);
//  delay(1000);                               // delay here to slow down the output so it is easier to read and better control of heater relay  
}

//******** Start of printTemperature ***********
float printTemperature(DallasTemperature sensors, DeviceAddress deviceAddress)
{
   float tempF = sensors.getTempF(deviceAddress);
   if (tempF == -127.00) 
   {
   Serial.print("Error getting temperature  ");
   } 
   else
   {
   Serial.print("F: ");
   Serial.print(tempF);
   return(tempF);
   }
}
// ***** End printTemperature ********

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

// This is called when a new time value was received
void receiveTime(unsigned long controllerTime) {
  // Ok, set incoming time 
  Serial.print("Time value received: ");
  Serial.println(controllerTime);
  RTC.set(controllerTime); // this sets the RTC to the time from controller - which we do want periodically
  timeReceived = true;
}

/* Solar Project
 *  
 * Motion Sensor example using HC-SR501 
 * 
 * http://www.mysensors.org/build/motion
 *
 */

void motionSensor()
{
   Serial.println("We made it to Motion Sensor.");
   Serial.println("But no one is home yet!");
/*  // Read digital motion value
  bool tripped = digitalRead(motion_Sensor_Pin) == HIGH;

  Serial.println(tripped);
  send(msg.set(tripped?"1":"0"));  // Send tripped value to gw

  // Sleep until interrupt comes in on motion sensor. Send update every two minute.
  sleep(digitalPinToInterrupt(motion_Sensor_Pin), CHANGE, SLEEP_TIME);
*/
}
// "*" = Supplemental Sensor Board. Pluges Into Extra I/O Pins on Ardino Mega 

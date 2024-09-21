/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/

//5 volts / 1024 units or, 0.0049 volts (4.9 mV) per unit
// 928 * 4.9mV = 4.500mV


// These constants won't change. They're used to give names to the pins used:
const int sensorPin = A1;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot
int sensorTemp = 0;
int sensorCal = 0;
int calVal = 0;
int deltaVal = 50;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  for (calVal=0; calVal<=5; calVal++){
  sensorTemp += analogRead(sensorPin);
  //Serial.print("SensorTemp: ");
  //Serial.println(sensorTemp);
  delay(1000);
 }
 sensorCal =  sensorTemp/calVal;
 //Serial.print("Sensor calibratie waarde: ");
 //Serial.println(sensorCal);
 delay(2000);
  
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(sensorPin);
  //Serial.println(sensorValue);
  
  if (sensorValue > (sensorCal - deltaVal) && sensorValue < (sensorCal + deltaVal)) {
    Serial.print("Glucose: ");
    Serial.print(",");
    Serial.println(sensorValue);
    
  }
  
  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(1000);
}

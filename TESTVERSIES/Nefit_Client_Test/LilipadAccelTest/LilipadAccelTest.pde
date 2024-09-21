/*
 ADXL335
 
 Reads an Analog Devices ADXL3xx accelerometer and communicates the
 acceleration to the computer.  The pins used are designed to be easily
 compatible with the breakout boards from Sparkfun, available from:
 http://www.sparkfun.com/commerce/categories.php?c=80

 http://www.arduino.cc/en/Tutorial/ADXL3xx

 The circuit:
 analog 0: accelerometer self test
 analog 1: z-axis
 analog 2: y-axis
 analog 3: x-axis
 analog 4: ground
 analog 5: vcc
 
 created 2 Jul 2008
 by David A. Mellis
 modified 4 Sep 2010
 by Tom Igoe 
 
 This example code is in the public domain.

*/

// these constants describe the pins. They won't change:
const int power = 500;              // Powersupply = 5V
const int xpin = 3; //A3;           // x-axis of the accelerometer
const int ypin = 2; //A2;           // y-axis
const int zpin = 1; //A1;           // z-axis (only on 3-axis models)


void setup()
{
  // initialize the serial communications:
  Serial.begin(9600);
  
  // Provide ground and power by using the analog inputs as normal
  // digital pins.  This makes it possible to directly connect the
  // breakout board to the Arduino.  If you use the normal 5V and
  // GND pins on the Arduino, you can remove these lines.
}

void loop()
{
  Serial.println(analogRead(ypin));
  Serial.println(analogRead(xpin));
  if(analogRead(xpin)<(power-50)) Serial.println("Ligt op rechterzijkant");
  if(analogRead(xpin)>(power+50)) Serial.println("Ligt op linkerzijkant");
  if(analogRead(ypin)<(power-50)) Serial.println("Staart Omhoog");
  if(analogRead(ypin)>(power+50)) Serial.println("Staart Omlaag");
}

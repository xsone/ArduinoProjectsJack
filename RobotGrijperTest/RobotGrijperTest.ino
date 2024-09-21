/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int motorPin1 = 6;     // the number of the pushbutton pin
const int motorPin2 = 7;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  Serial.begin(9600);
  pinMode(motorPin1, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(motorPin2, OUTPUT); 
  digitalWrite(motorPin2, HIGH);  
  delay(5000);
}

void loop()
{
   if (Serial.available() > 0)
    {
     char inChar = Serial.read();
     //Serial.write("\n");
     Serial.println(inChar);
     switch(inChar)
      {
       case 's': digitalWrite(motorPin2, HIGH);
                 delay(5000);
                 digitalWrite(motorPin2, LOW);
                 break;
       case 'd': digitalWrite(motorPin1, HIGH);
                 delay(5000);
                 digitalWrite(motorPin1, LOW);
                 break;
      } 
    }      
}

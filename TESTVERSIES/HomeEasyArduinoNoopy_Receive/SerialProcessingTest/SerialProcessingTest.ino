/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */

int i = 0;
int j = 0;
int maxi = 255;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
 
}

void loop() {
  // print the string when a newline arrives:
 for (i = 0; i <= maxi; i++)
  {
   delay (50);
   Serial.print("{\"x\":");
   Serial.print(i);
   Serial.print(",\"y\":");
   Serial.print(i+10);
   Serial.print(",\"z\":");
   Serial.print(i+20);
   Serial.println("}");
 }
 if (i == maxi) i == 0;
}





//////////////////////////////////////////////////////////////////
//\u00a92011 bildr
//Released under the MIT License - Please reuse change and share
//Simple code for the ADXL335, prints calculated orientation via serial
//////////////////////////////////////////////////////////////////
 
//Analog read pins
#include "WProgram.h"
void setup();
void loop();
const int xPin = 3;
const int yPin = 2;
const int zPin = 1;
 
//The minimum and maximum values that came from
//the accelerometer while standing still
//You very well may need to change these
//int minVal = 265; oorspr.
//int maxVal = 402; oorspr.

unsigned const int minVal = 300;
unsigned const int maxVal = 455;

 
//to hold the caculated values
double x;
double y;
double z;
 
void setup(){
  Serial.begin(9600);
}
 
void loop(){
 
  //read the analog values from the accelerometer
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);
 
  //convert read values to degrees -90 to 90 - Needed for atan2
 // int xAng = map(xRead, minVal, maxVal, -90, 90);
 // int yAng = map(yRead, minVal, maxVal, -90, 90);
 // int zAng = map(zRead, minVal, maxVal, -90, 90);
  

  int xAng = map(xRead, minVal, maxVal, 0, 180);
   if (xAng < 0) xAng = 0;
  int yAng = map(yRead, minVal, maxVal, 0, 180);
   if (yAng < 0) yAng = 0;
  int zAng = map(zRead, minVal, maxVal, 0, 180);
   if (zAng < 0) zAng = 0;
 
 
  //Caculate 360deg values like so: atan2(-yAng, -zAng)
  //atan2 outputs the value of -\u03c0 to \u03c0 (radians)
  //We are then converting the radians to degrees
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);
 
  //Output the caculations
  /*
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" | y: ");
  Serial.print(y);
  Serial.print(" | z: ");
  Serial.println(z);
  */
  
  /*
  //Callibration
  Serial.print("x: ");
  Serial.print(xRead);
  Serial.print(" | y: ");
  Serial.print(yRead);
  Serial.print(" | z: ");
  Serial.println(zRead);
  
  Serial.println("----");
  */
    
  Serial.print("{\"x\":"); 
  Serial.print(xAng);
  Serial.print(",\"y\":"); 
  Serial.print(yAng);
  Serial.print(",\"z\":");
  Serial.print(zAng);
  Serial.println("}");
  
 
  delay(500);//just here to slow down the serial output - Easier to read
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


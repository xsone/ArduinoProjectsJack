/*
Adafruit Arduino - Lesson 14. Sweep
*/

#include <Servo.h> 

#include "WProgram.h"
void setup();
void loop();
int servoPin = 9;
 
Servo servo;  
 
int angle = 0;   // servo position in degrees 
 
void setup() 
{ 
  servo.attach(servoPin); 
} 
 
 
void loop() 
{ 
  // scan from 0 to 180 degrees
  for(angle = 0; angle < 180; angle++)  
  {                                  
    servo.write(angle);               
    delay(15);                   
  } 
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 0; angle--)    
  {                                
    servo.write(angle);           
    delay(15);       
  } 
} 

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


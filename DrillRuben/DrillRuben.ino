#include <AFMotor.h>

int i;
int motorValueInt;
String motorValueStr;

// DC motor on M1
AF_DCMotor motor(1);

// DC motor on M2
AF_DCMotor motor2(2);

  
void setup() 
 {  
  Serial.begin(9600);//set baud rate

  // Print startup text
  delay(100);
  Serial.println("Drill Conroller; (c) Ruben; Rev. 1.0 nov 2019");
  delay(200); // keep the welcome screen on for 3 seconds
   // Turn ON DC MOTOR #1
  motor.setSpeed(200);
  motor.run(RELEASE);
 }

// program starts here
void loop() 
{
 // wait button press
 for (;;)
  {  
   if (Serial.available() > 0)
    {
     motorValueStr = Serial.readString();
     Serial.print("String Value: ");
     Serial.println(motorValueStr);
     motorValueInt = motorValueStr.toInt();
     Serial.print("Int Value: ");
     Serial.println(motorValueInt);
     motor.run(FORWARD);
     motor.setSpeed(motorValueInt); 

     // inChar = Serial.read();
     //if (inChar == 'l') Lees();
     //if (inChar == 's') Stabiliteit();
    } 
  } 
}





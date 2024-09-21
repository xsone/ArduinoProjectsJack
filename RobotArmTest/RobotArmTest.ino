/*
Motor bedrading
M1 = geel
M2 = paars
M3 = rood
M4 = blauw
M5 = wit


Massa = zwart

0x07	Right Arrow
0x08	Backspace
0x09	Tab
0x0A	Enter
0x0B	Left Arrow
0x0C	Down Arrow
0x0D	Enter
0x0E	Up Arrow

BlueTooth HC04 Module aansluiting:
(BT) VCC = 5VDC (Arduino)
(BT) GND = 0VDC (Arduino)
(BT) RXD = TXD (Arduino gele draad)
(BT) TXD = RXD (Atduino groene draad);

ADSCI codes voor motorbesturing zie NumLock op toestenbord
Motor 1 (onderaan op draaivoet)
Rechtsom = 6 = > 
Linksom  = 4 = <

Motor 2 (onderaan op draaivoet omhog/omlaag)
Omhoog = 8 = ^
Omlaag = 2 = v

Motor 3 (midden)
Omhoog = 9 = PgUp
Omlaag = 3 = PgDn

Motor 4 (Draaien Kop)
Rechtsom = 7 = Home
Linksom  = 1 = End

Motor 5 (Grijper)
Open  = -
Dicht = +

Alle Motoren Stoppen = 0

*/


#include <AFMotor.h>

AF_DCMotor motor1(1); // create motor #2, 64KHz pwm
AF_DCMotor motor2(2); // create motor #2, 64KHz pwm
AF_DCMotor motor3(3); // create motor #2, 64KHz pwm
AF_DCMotor motor4(4); // create motor #2, 64KHz pwm

//Deze pinnen D2 en D13 worden niet gebruikt door het Funduino motorshield
int IN1 = 2; 
int IN2 = 13; 
int IN3 = A5;
int IN4 = A4;

char inChar;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("RobotArm v1.1..");
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
 
  
  motor1.setSpeed(255);     // set the speed to 200/255
  motor2.setSpeed(255);     // set the speed to 200/255
  motor3.setSpeed(255);     // set the speed to 200/255
  motor4.setSpeed(255);     // set the speed to 200/255
}

void loop() 
{
 Serial.println("...Start V1.9...");
 Serial.println();
 delay(100);
 // wait button press

/*s

 for (;;)
  {  
   if (Serial.available() > 0)
    {
     {
     switch (inChar = Serial.read()) {
     case 'u': motor4.run(FORWARD);
     case 'd': motor4.run(BACKWARD);
     case 'r': motor4.run(RELEASE);
    }
     Serial.println(inChar);
   } 
  } 
 }
 */
 
 for (;;)
  {  
   if (Serial.available() > 0)
    {
     inChar = Serial.read();
     //Serial.write("\n");
     Serial.println(inChar);
     switch(inChar)
      {
       case '6': motor1.run(BACKWARD);
                 break;
       case '4': motor1.run(FORWARD);
                 break;
       case '2': motor2.run(BACKWARD);
                 break;
       case '8': motor2.run(FORWARD);
                 break;
       case '3': motor3.run(BACKWARD);
                 break;
       case '9': motor3.run(FORWARD);
                 break;
       case '7': motor4.run(BACKWARD);
                 break;
       case '1': motor4.run(FORWARD);
                 break;  
       case 'f': GripperForward();
                 break;  
       case 'r': GripperReverse();
                 break;  
       case 'g': WheelsForward();
                 break;  
       case 's': WheelsReverse();
                 break;  
       case '5': motor1.run(RELEASE);
                 motor2.run(RELEASE);
                 motor3.run(RELEASE);
                 motor4.run(RELEASE);
                 GripperStop();
                 WheelsStop();
                 break;
       case 't': motor3.run(FORWARD);
                 delay(10000);  // run forward for 1 second
                 motor3.run(RELEASE);
                 delay(5000);  // 'coast' for 1/10 second
                 motor3.run(BACKWARD);  // run in reverse
                 delay(10000);  // run forward for 1 secondr
                 motor3.run(RELEASE);
                 break;
       default:  break;
      }
    }
  }
}  
 
 void GripperForward()
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    Serial.print("Gripper Forward");
    Serial.println();
 }

void GripperReverse()
{
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  Serial.print("Gripper Reverse");
  Serial.println();
}

void GripperStop()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    Serial.print("Gripper Stop");
    Serial.println();
}

void WheelsForward()
{
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);
  Serial.print("Wheels Forward");
  Serial.println();
}

void WheelsReverse()
{
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.print("Wheels Reverse");
    Serial.println();
}

void WheelsStop()
{
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.print("Wheels Stop");
    Serial.println();
}

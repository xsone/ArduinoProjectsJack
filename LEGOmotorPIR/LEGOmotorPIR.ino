// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 

int calibrationTime = 10;    //PIR sensor
long unsigned int lowIn;     //tijd waarbinen PIR sensor naar laag gaat
long unsigned int pause = 5000;  //verwachte delay waarbinnen beweging stopt
boolean lockLow = true;
boolean takeLowTime;  
int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;   // Test led
int servoPin = 5;   // attaches the servo on analog pin 5 to the servo object 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  
   //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
    
  myservo.attach(servoPin);  // attaches the servo on analog pin 5 to the servo object 
  //myservo.write(180);
  //delay(10000);
  //myservo.write(80);
  pinMode(ledPin, OUTPUT);
} 
 
 
void loop() 
{ 
  if(digitalRead(pirPin) == HIGH)
   {
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    staartMove();
    if(lockLow)
      {  
         //makes sure we wait for a transition to LOW before any further output is made:
       lockLow = false;            
       Serial.println("---");
       Serial.print("motion detected at ");
       Serial.print(millis()/1000);
       Serial.println(" sec"); 
       delay(50);
      }         
     takeLowTime = true;
    }

    if(digitalRead(pirPin) == LOW)
     {       
      digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

      if(takeLowTime)
       {
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
       }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }
} 


// Staart gaat van 0 nar 180 degrees en wacht 10 sec. en dan ter
void staartMove ()
{
 //myservo.write(170); 
  for(pos = 170; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
 myservo.write(0);
 digitalWrite(ledPin, LOW);
 delay(10000);
 for(pos = 0; pos < 170; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  }
} 

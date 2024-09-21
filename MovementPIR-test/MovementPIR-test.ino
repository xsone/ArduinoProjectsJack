/*
 Test movement sensor PIR
 
 created Jan. 2015, door Jack Cop
 
 */

#include <SPI.h>

int Verhoog = 0;
int Verlaag = 0;

int pirLedPin = 8;
int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int calibrationTime = 5;    //PIR sensor
int state = LOW;


void setup() 
{
 Serial.begin(9600);
 pinMode(pirPin, INPUT);
 pinMode(pirLedPin, OUTPUT);
 digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      digitalWrite(pirLedPin, HIGH);
      delay(500);
      digitalWrite(pirLedPin, LOW);
      delay(500);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  //PIRcal();
  attachInterrupt(1, MoveDetect, CHANGE);
}

void MoveDetect()
{
 Serial.println("Beweging...PIR AAN");
 //delay(1000);
 digitalWrite(pirLedPin, state);
 state = !state;
} 


/*
void PIRcal()
{
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("calibrating PIR sensor");
 lcd.setCursor(0,1); 
 for(int i = 0; i < calibrationTime; i++){
      lcd.print(".");
      delay(1000);
      }
 lcd.print("PIR active");
 delay(500);
}    
*/

void loop() {
  //noInterrupts();
  //digitalWrite(pirLedPin, LOW);
 } // end loop


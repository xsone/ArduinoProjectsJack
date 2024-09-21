#include <Time.h>


int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;
float sensorValue = 0;  // variable to store the value coming from the sensor
int count=9;
unsigned long starttime = 0;
int heartrate = 0;
boolean counted = false;


void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  Serial.println("...start hartslag meting...");
}


void loop() {
  // read the value from the sensor:

starttime = millis();
while (millis()<starttime+10000)                          // so i'm reading the pulse for 10 seconds
{
  sensorValue = analogRead(sensorPin);
  if (sensorValue >550 && counted == false)         // counting the pulse, and making sure I dont count
  {                                                                // each peak more than once
    count++;
    counted = true; 
    digitalWrite(ledPin, HIGH);
  }
  else if (sensorValue < 500)
  {
    counted = false;
    digitalWrite(ledPin, LOW);
  }
}

 heartrate = count*6;                                    // multiplying by six to get pulse per minute
 Serial.println(heartrate);                                   // printing to serial screen

  heartrate=0;
  count =0;
}

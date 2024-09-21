#include <Time.h>


int HeartSensorPin = A0;    // select the input pin for the Heartbeat sensor
int ForceSensorPin = A1;
int ledPin = 13;
float HeartSensorValue = 0;  // variable to store the value coming from the sensor
float ForceSensorValue = 0;
int count=9;
unsigned long starttime = 0;
int heartrate = 0;
boolean counted = false;


void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(HeartSensorPin, INPUT);
  pinMode(ForceSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  Serial.println("...start hartslag + kracht meting...");
}


void loop() {
  // read the value from the sensor:

Serial.println(HeartSensorValue);
starttime = millis();
while (millis()<starttime+1000)                          // so i'm reading the pulse for 10 seconds
{
  HeartSensorValue = analogRead(HeartSensorPin);
  ForceSensorValue = analogRead(ForceSensorPin);
  if (HeartSensorValue >550 && counted == false)         // counting the pulse, and making sure I dont count
  {                                                                // each peak more than once
    count++;
    counted = true; 
    digitalWrite(ledPin, HIGH);
  }
  else //if (sensorValue < 500)
  {
    counted = false;
    digitalWrite(ledPin, LOW);
  }
}

 heartrate = count*6;                                    // multiplying by six to get pulse per minute
 //Serial.print("♥ HeartBeat: ");
 //Serial.print(heartrate);                                   // printing to serial screen
 //Serial.println(" BPM");
 
 //Serial.println("Force: ");
 //Serial.print(ForceSensorValue);
 //Serial.println(" N");

  heartrate=0;
  count =0;
}

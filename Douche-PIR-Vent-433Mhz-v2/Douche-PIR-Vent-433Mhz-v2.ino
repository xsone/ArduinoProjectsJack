/* Badkamer ventilator + bewegingssensor + RF-transmitter 433MHz (KaKU)
* Name: Jack Cop
* Date: 21/2/2022
* Version: 1.2
*/

#include <NewRemoteTransmitter.h>

#define MAX_NALOOP 300 //Ventilator draait nooit langer dan 5min. na, bij geen beweging meer
#define MIN_AAN 5 //Minimaal altijd 5 sec. aan

//Settings Timer
unsigned long int timerAan = 0;
unsigned long int timerUit = 0;
unsigned int AanTeller = 0;
unsigned int UitTeller = 0;
unsigned int calibrationTime = 3;      

//Settings PIR-sensor
const int pirPin = 2;
const int stateLed = 13;
boolean transmitterState = LOW;
boolean pirState = LOW;

//Settings KlikAanKlikUit transmitter
const int receiverPin = 11;
unsigned long receivedCode_address = 1108871;
unsigned int receivedCode_period = 249;

NewRemoteTransmitter transmitter(receivedCode_address, receiverPin, receivedCode_period);

void setup() 
{
 pinMode(pirPin, INPUT); // Sensor pin as input
 pinMode(stateLed, OUTPUT); // Sensor pin as input
    
 // Need interrupts for delay()
 //interrupts();
  
 transmitter.sendGroup(LOW);
  
 Serial.begin(9600);
 Serial.println("...Start ventilator versie 2.0...");
 Serial.println();

 digitalWrite(stateLed, LOW);
 digitalWrite(pirPin, LOW);
}

//Bewegingssensor
void loop()
{
 if(digitalRead(pirPin) == HIGH)  //Wel Move, sensor input is 5V
  { 
   digitalWrite(stateLed, HIGH);
   Serial.println("Sensor input = HIGH");
   timerUit = 0;
   if (transmitterState == LOW && timerAan > 1) { //1e versie
    transmitter.sendGroup(HIGH); //Voorkomen dat onnodig steeds signaal naar transmitter wordt gestuurd
    //transmitterState = HIGH; 1e versie
    transmitterState = HIGH;
   } 
   timerAan++;
  } 
 
 if(digitalRead(pirPin) == LOW)  //Wel Move, sensor input is 5V
  { 
   digitalWrite(stateLed, LOW);
   Serial.println("Sensor input = LOW");
   timerUit++;
  } 
 
 if (timerUit >= (timerAan + MIN_AAN) || timerUit >= MAX_NALOOP) //Hoe langer je douched hoe langere naloop
  {
   digitalWrite(stateLed, LOW);
   Serial.println("Sensor input = LOW");
   //if (transmitterState == HIGH) //1e versie
   if (transmitterState == HIGH) 
    {
     transmitter.sendGroup(LOW);
     //transmitterState = LOW; //1e versie
     transmitterState = LOW;
    } 
   Serial.println("...Ventilator UIT...: ");
   timerUit = 0;
   timerAan = 0;
  }

 Serial.print("Timer AAN: ");
 Serial.println(timerAan, DEC);
 Serial.print("Timer UIT: ");
 Serial.println(timerUit, DEC);

 delay(1000); //timer van 1 sec.
}

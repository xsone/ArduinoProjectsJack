/* Badkamer ventilator met bewegingssensor
* Jack Cop
* september 2016
*/

// Meest simpele versie

#define MAX_NALOOP 300 //Ventilator draait nooit langer dan 5min. na, bij geen beweging meer

int TimerAan = 0;
int TimerUit = 0;
int AanTeller = 0;
int UitTeller = 0;
const int pirPin = 3;
const int stateLed = 5;
byte pirState = LOW;  // status van pir-bewegingssensor


void setup() 
{
  pinMode(pirPin, INPUT); // Sensor pin as input
  pinMode(stateLed, OUTPUT); // Sensor pin as input
    
  Serial.begin(9600);
  Serial.println("...Start ventilator versie 2.0...");
  Serial.println();
    
  digitalWrite(stateLed, HIGH);
  delay(1000);
  digitalWrite(stateLed, LOW);
  delay(1000);
  digitalWrite(stateLed, LOW);
  if (digitalRead(pirPin == LOW)) digitalWrite(pirPin, HIGH);
  delay(1000);
  digitalWrite(pirPin, HIGH);
  delay(1000);
 }

//Bewegingssensor
void loop()
{
 if(digitalRead(pirPin) == HIGH)  //No Move
  {
   //digitalWrite(stateLed, LOW);
   Serial.print("Vent UIT: ");
   Serial.println(TimerUit, DEC);
   TimerUit++;
  } 
 
 if(digitalRead(pirPin) == LOW)  //Wel Move
  {
   TimerUit = 0;
   digitalWrite(stateLed, HIGH);
   Serial.print("Vent AAN: ");
   Serial.println(TimerAan, DEC);
   TimerAan++;
  } 

 if (TimerUit > TimerAan || TimerUit >= MAX_NALOOP) //Hoe langer je douched hoe langere naloop
  {
   digitalWrite(stateLed, LOW);
   Serial.print("Vent UIT: ");
   Serial.println(TimerUit, DEC);
   TimerUit = 0;
   TimerAan = 0;
  }

 //Serial.print("Timer AAN: ");
 //Serial.println(TimerAan, DEC);

 delay(1000); //timer van 1 sec.
}


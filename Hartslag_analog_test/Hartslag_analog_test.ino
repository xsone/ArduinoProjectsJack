/*
 * Hartslag sensor o.b.v. IR
 * Daarnaast wordt er de analoge waarde van de sensor gelogd om daar eventuele andere patronen te zien
 * De gevoeligheid logt tussen 500 en 600
 * 
 * nov. 2022 (c) Jack Cop
 */

int sensorPin = 0;
int heartBeat = 0;
int heartBeatTest = 0;
unsigned long timer;
unsigned long interval = 10000;
unsigned long lastTime = 0;
boolean rising = false;
boolean falling = false;

void setup() {
   Serial.begin(9600);
   Serial.println("Start BPM sensor...");
}

void loop ()
{
 if(analogRead(sensorPin)> 600 && (rising == false)) {
   heartBeat++;
   heartBeatTest = 500;
   rising = true;
  }   
 if(analogRead(sensorPin) < 500 && (rising == true)) {
   rising = false;
   heartBeatTest = 0;
  }   
 if(millis() - lastTime > interval) {
   lastTime = millis();  
   //Serial.print("BPM: ");
   Serial.print(heartBeat*6);
   Serial.print(" ");
   //Serial.print("SensorValue: ");
   Serial.println(analogRead(sensorPin));
   heartBeat = 0;
 }
} 

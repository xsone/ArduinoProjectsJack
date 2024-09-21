/*

  Versie info:
  -------------
  File name: WaterSensorTest
  Werking: zend iedere 24 uur een waarde naar de database MySQL "Energielogger" o.b.v. Timeserver
  (c) Jack Cop 2012-2014
  versie 2.0
*/

#include <SPI.h>
#include <Time.h>

// Tijd instellingen
time_t t = 0UL; 
String tijdNu;
String tijdLog = "21:00:00";

// Sensor reading
volatile long int WaterIntWaarde=0;
volatile long int WaterLtrWaarde=0;
volatile long int WaterM3Waarde=0;

const int waterLedPin =  13;
int row = 0;

//Inlezen van meterdata en naar string omzetten
int incomingByte = 0;
//String inputString = "";
//String inputStringTest[10] = {"1-0:1.8.1(002074.842*kWh)","1-0:1.8.2(000881.383*kWh)","1-0:2.8.1(000010.981*kWh)","1-0:2.8.2(000028.031*kWh)"};
String inputString[10];
//String inputString = "1-0:1.8.1(002074.842*kWh)";

//char input; // incoming serial data (byte)
//bool readnextLine = false;
//#define BUFSIZE 75
//char buffer[BUFSIZE]; //Buffer for serial data to find \n .
//int bufpos = 0;
int InteruptTellerWater = 0;

// Initialisatie en starten
void setup()
{
  //Watersensor output LED indicator
  pinMode(waterLedPin, OUTPUT);  //LED indicator Water meter
  digitalWrite(waterLedPin, LOW); //Water meter
  
  //Watersensor input via interrupt 
  pinMode(2, INPUT);  //sensorinput Water INT0
  attachInterrupt(0, WaterRead, CHANGE); //interrupt digital pin 2
  
  //Initialiseer Seriele poort met baudreate slimme meter (115200) en Ethernetshield
  Serial.begin(115200);			// voor debugging
  Serial.println("Watersensor Interrup Test Programma gestart...");
  delay(1000);
}

//Programma
void loop() 
{

}  

// Uitlezen Watersensor
void WaterRead()
{
  digitalWrite(waterLedPin, LOW);
  if(InteruptTellerWater==1)
    {
     WaterIntWaarde++;
     InteruptTellerWater=0;
     digitalWrite(waterLedPin, HIGH);
     if(WaterLtrWaarde+WaterIntWaarde==1000) //100 spiegelpulsen = 100ltr = 1M3 water
      {
       WaterM3Waarde++;
       WaterLtrWaarde=0;
      } 
    }
   else InteruptTellerWater++;
  if(WaterIntWaarde==99999999) WaterIntWaarde=0; //Watermeter Max. waarde bereikt
  Serial.println(WaterIntWaarde, DEC);
}

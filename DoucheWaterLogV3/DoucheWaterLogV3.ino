//Temperatuurlogger Douchewater
//Versie 1.0 is inclusief saving op microSD shield Sparkfun Chip Select is daar pin nr 8, vandaar omgezet naar

#include <SD.h>
#include "DHT.h"

#define MIN    75
#define HALF  130 
#define FULL  255
#define NALOOP 5
#define SAMPLES 10

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define anout_pin 9 // toerental regeling ventilator 0-10V (van 5V omgezet naar 10V m.b.v transistor)
DHT dht(DHTPIN, DHTTYPE);


// variable declaration
//int fanSpeed = 0;
int ScanFreq = 2000; //x1000 is sec.
int TempValue = 0; // sample value array
int TempValueOld = 0;
int Stijgend = 0;
int Dalend = 0;
int Stabiel = 0;
int MotorStatus = MIN;

int potVal = 0;
int potPin = A0;


// initialize settings
void setup() 
{  
 pinMode(anout_pin, OUTPUT);
   
 Serial.begin(9600);//set baud rate
 Serial.println("DouchWaterLogger; (c) Jack; Rev. 1.0 november 2015");
 Serial.println();
 Serial.print("TemperatureStartWaarde: ");
 //TempValue = dht.readTemperature(); 
 TempValueOld = TempValue;
 //fanSpeed = MIN;
 Serial.print(TempValue);
 Serial.println(" *C ");
 Serial.println();
 delay(200); // keep the welcome screen on for 3 seconds
}

void loop()
{
 //TempValue = dht.readTemperature(); 
 
 //Potmeter simuleert temperatuur
  for (int i=0; i < SAMPLES ; i++) 
  {
   potVal += analogRead(potPin); 
   potVal /= SAMPLES ;
  } 
 TempValue = potVal/2; //ber
 // end potmeter
 
 Serial.print("Temperature: "); 
 Serial.print(TempValue);
 Serial.print(" *C ");
 Serial.print("\tTemperatureOld: "); 
 Serial.print(TempValueOld);
 Serial.println(" *C ");
 Serial.print("Stijgend: "); 
 Serial.print(Stijgend);
 Serial.print("\t\tDalend: "); 
 Serial.print(Dalend);
 Serial.print("\tStabiel: "); 
 Serial.print(Stabiel);
 Serial.println();
 Serial.print("Motor Speed: ");
 Serial.println(MotorStatus);
 Serial.println();

//ScanFreq = 5000; 
 
 //if (TempValue > 22.0 && TempValue < 25.0){ MotorSpeed (HALF); Serial.println("Motor HALF"); }
 //if (TempValue > 25.0){ MotorSpeed (FULL); Serial.println("Motor FULL"); }
 //if (TempValue < 20.0){ MotorSpeed (MIN); Serial.println("Motor MIN"); }
 
 
 
 
 
 
 if (TempValue > TempValueOld)
  {
   Stijgend++;
   Serial.println("Temperatuur STIJGT");
   if (Stijgend == 2) {MotorSpeed(HALF); MotorStatus = HALF;}
   if (Stijgend == 3) {MotorSpeed(FULL); MotorStatus = FULL;}
   Dalend = 0;
   Stabiel = 0;
  }
 if (TempValue < TempValueOld)
  {
   Dalend++;
   Serial.println("Temperatuur DAALT");
   if (Dalend == 2) {MotorSpeed(HALF); MotorStatus = HALF;}
   if (Dalend == 5) { MotorSpeed(MIN); Stijgend = 0; Dalend = 0; MotorStatus = MIN;}
   Stijgend = 0;
   Stabiel = 0;
  } 
 if (TempValue == TempValueOld)
  {
   Serial.println("Temperatuur STABIEL");
   Stabiel++;
   if (Stabiel == 10) { MotorSpeed(MIN); Stijgend = 0; Dalend = 0; MotorStatus = MIN;}
   //Dalend = 0;
  } 

 //Serial.print("MotorSpeed: ");
 //Serial.println(fanSpeed, DEC); 

  
 
 
 
/*  
 if (TempValue > (TempValueStart + 0.5))
  {
   TempValueStart = TempValue;
   Serial.println("Temperatuur stijgt..");
  }
  
 if (TempValue < (TempValueStart - 0.5))
  {
   TempValueStart = TempValue;
   Serial.println("Temperatuur daalt..");
  } 
*/  
 TempValueOld = TempValue; 
 delay(ScanFreq);
}

void MotorSpeed(int fanSpeed)
{
 analogWrite(anout_pin, 255-fanSpeed);
}


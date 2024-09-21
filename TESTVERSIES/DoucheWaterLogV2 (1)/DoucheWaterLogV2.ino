//Temperatuurlogger Douchewater
//Versie 1.0 is inclusief saving op microSD shield Sparkfun Chip Select is daar pin nr 8, vandaar omgezet naar

#include <SD.h>
#include "DHT.h"

#define MIN    75
#define HALF  130 
#define FULL  255

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define anout_pin 9 // toerental regeling ventilator 0-10V (van 5V omgezet naar 10V m.b.v transistor)
DHT dht(DHTPIN, DHTTYPE);


// variable declaration
int ScanFreq = 10000; //x1000 is sec.
float TempValue = 0.00; // sample value array
float TempValueStart = 0.00; // sample value array

// initialize settings
void setup() 
{  
 pinMode(anout_pin, OUTPUT);
   
 Serial.begin(9600);//set baud rate
 Serial.println("DouchWaterLogger; (c) Jack; Rev. 1.0 november 2015");
 Serial.println();
 Serial.print("TemperatureStartWaarde: ");
 TempValueStart = dht.readTemperature(); 
 Serial.print(TempValueStart,1);
 Serial.println(" *C ");
 Serial.println();
 delay(200); // keep the welcome screen on for 3 seconds
}

void loop()
{
 TempValue = dht.readTemperature(); 
 
 Serial.print("Temperature: "); 
 Serial.print(TempValue);
 Serial.println(" *C ");
 Serial.println();
//ScanFreq = 5000; 
 
 if (TempValue > 22.0 && TempValue < 25.0){ MotorSpeed (HALF); Serial.println("Motor HALF"); }
 if (TempValue > 25.0){ MotorSpeed (FULL); Serial.println("Motor FULL"); }
 if (TempValue < 20.0){ MotorSpeed (MIN); Serial.println("Motor MIN"); }
 
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
  
 delay(ScanFreq);
}

void MotorSpeed(int fanSpeed)
{
 analogWrite(anout_pin, 255-fanSpeed);
}


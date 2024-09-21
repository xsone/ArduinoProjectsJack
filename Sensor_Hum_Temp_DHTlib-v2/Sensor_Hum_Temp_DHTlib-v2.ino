// Badkamer ventilator regeling met DHT library
// (c) Jack Cop 3/12/2014
// version 1.0

#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 (besteld bij dealextreme.com
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

#define rxPin 4
#define txPin 5
//#define dht_dpin A0 //was A5
//#define dht_dpin 2 //was A5
#define anout_pin 9 // toerental regeling ventilator 0-10V (van 5V omgezet naar 10V m.b.v transistor)


SoftwareSerial mySerial(rxPin, txPin); // RX, TX t.b.v. BlueTooth uitlezing op smartphone.


//byte dht_dat[5];

const int ledPin =  13;
int fanSpeed = 75;
int minFanSpeed = 75; //is 0V
int maxFanSpeed = 255; //is 10V
int Timer = 0;
int StopTime = 100; //stopt na 15 min.
int Hum = 0;
int MinTemp = 15;
int TempOld = 0;
int Temp = 0;

void setup()
{
 Serial.begin(9600);
 mySerial.begin(38400);
 dht.begin();
 
 pinMode(rxPin, INPUT);
 pinMode(txPin, OUTPUT);
 pinMode(ledPin, OUTPUT); 
 digitalWrite(ledPin, LOW);

 Serial.println("Humidity and temperature start...");
   mySerial.println("Humidity and temperature start...");
 delay(500);
 //dht_dat[0] = 60;
 fanSpeed = minFanSpeed;
 Temp = dht.readTemperature(); //lees temperatuur uit in C, was ook een float
 TempOld = Temp;
 //PhoneInit(); // initialiseer BT verbinding
 //delay(700);
}


void PhoneInit()
{
  Serial.println("AT+RESET"); delay(500);
    mySerial.println("AT+RESET"); delay(500);
  //Serial.println("AT+ORGL"); delay(500);
  //  mySerial.println("AT+ORGL"); delay(500);
  Serial.println("AT+INIT"); delay(500);
    mySerial.println("AT+INIT"); delay(500);
  Serial.println("AT+ENSNIFF=0012,06,071015"); delay(500);
    mySerial.println("AT+ENSNIFF=0012,06,071015"); delay(500);
  Serial.println("AT+CLASS=0040"); delay(500);
    mySerial.println("AT+CLASS=0040"); delay(500); //adres BTshield
  Serial.println("AT+INQ"); delay(500);
    mySerial.println("AT+INQ"); delay(500);
}  



void loop()
{
  Hum = dht.readHumidity(); //lees RH uit in %, was orgineel een float waarde
  Temp = dht.readTemperature(); //lees temperatuur uit in C, was ook een float
  
  /*
  for (count=0; count < 100; count++)
   {
     gemTemp = gemTemp + Temp;
   }
  Temp = gemTemp/count; 
  */
  
  //Temperatuur stijgt
  if (TempOld < (Temp+1) )
   {
    fanSpeed += 50;
    Timer=0;
   } 

  //Temperatuur daalt
  if (TempOld > (Temp-1) ) 
   {
    fanSpeed -= 50;
    Timer++;
   } 
  
  //Temperatuur gaat omlaag en niet meer gedouched
  if (Timer > StopTime)
   {
    fanSpeed = minFanSpeed;
    Timer = 0;
   }

  if(fanSpeed <= minFanSpeed) fanSpeed = minFanSpeed;
  if(fanSpeed >= maxFanSpeed) fanSpeed = maxFanSpeed;

  analogWrite(anout_pin, 255-fanSpeed);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  DisplayData();
  TempOld = Temp;
  delay(500);
}


void DisplayData()
{
  Serial.print("Humidity: ");
  Serial.print(Hum);  
   mySerial.println(">"); 
   mySerial.print(Hum);
  Serial.print(" %\t");
   mySerial.print("\t");
  Serial.print("Temperature: ");
  Serial.print(Temp);
  Serial.print(" *C\t");
   mySerial.print(Temp);
   mySerial.print("\t");
  Serial.print(Timer,DEC);
   mySerial.print(Timer,DEC);
  Serial.print("\t");
   mySerial.print("\t");
  Serial.print(fanSpeed,DEC);
   mySerial.print(fanSpeed,DEC);
  Serial.print("\t");
   mySerial.print("\t");
  Serial.println(TempOld,DEC);
   mySerial.println(TempOld,DEC);
  //Serial.println(dht_dat[0],DEC);
}  

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");  
}  


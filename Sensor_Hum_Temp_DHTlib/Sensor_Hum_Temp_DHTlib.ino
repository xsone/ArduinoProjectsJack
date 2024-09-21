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
char HumiChar[10] = "0";
char TempChar[10] = "0";
char CalcChar[10] = "0"; //omrekenwaarde naar analog output
int CalcInt = 0;
int CalcIntOld = 0;
int Teller = 0;
int Timer = 0;
int StopTime = 900; //stopt na 15 min.
int State = 0;
int MinHum = 50;
int Hum = 0;
int HumOld = 0;
float Temp = 0;


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
 CalcInt = 75;
 PhoneInit(); // initialiseer BT verbinding
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
     
  //CalcInt = (5.1*dht_dat[0])-153;  //30% = low (0); 80% = high (255); werkt goe
  //CalcInt = (25.5*dht_dat[0])-1785; //test in bereik van 70% tot 80% RH
  //CalcInt = (9*dht_dat[0])-465;  // <=60% = rust 3V (75); 95% = high 10V (255)
  //CalcInt = (6*Hum-285;  // <=60% = rust 3V (75); 90% = high 10V (255)
  //CalcInt = (18*Hum - 1455);  // <=85% = rust 3V (75); 95% = high 10V (255)
  //CalcInt = (9*Hum - 600);  // <=75% = rust 3V (75); 95% = high 10V (255)
  //CalcInt = (6*Hum - 225);  // <=50% = rust 3V (75); 80% = high 10V (255)
  //CalcInt = (3.6*Hum - 33);  // <=30% = rust 3V (75); 80% = high 10V (255)
  //CalcInt = (4*Hum - 65);  // <=35% = rust 3V (75); 80% = high 10V (255)
  
  if (Teller >= 20)  // eerst 20 metingen doen i.v.m. pendelen
   {
    CalcInt = (6*Hum - 225);  // <=50% = rust 3V (75); 80% = high 10V (255)
    //analogWrite(anout_pin, 255-CalcInt);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
    Teller = 0;
   } 
   Teller++;  
  
  if (HumOld < (Hum+1)) 
   {
    //ventHoger = true;
    MinHum = 60;
    //HumOld = Hum;
   }
  if (HumOld > (Hum-1)) 
   {
    //ventLager = true;
    MinHum = 75;
    //HumOld = Hum;
   }
    
    
    //if (dht_dat[0] <= 60) CalcInt = 75; //minimum draaistand van 3V is 75bits
  if (CalcInt >= 255) CalcInt = 255;
  if (CalcInt <= 0) CalcInt = 0;

  if (Hum <= MinHum) //ventilator slaat af.
   {     
    CalcInt = 75;
    State = 0;
    Timer = 0;
    digitalWrite(ledPin, LOW);
   } 
    
  if (Hum > MinHum) //ventilator slaat aan en timer gaat lopen
   {
    State = 1;
    Timer++;
    digitalWrite(ledPin, HIGH);
   }
     
  if (Timer >= StopTime && State == 1)  //na een bepaalde periode "StopTime" wordt altijd gestopt en ventilator in laagste stand gezet.
   {
     CalcInt = 75; 
     digitalWrite(ledPin, LOW);
     Timer = 0;
     State = 0;
   } 
  analogWrite(anout_pin, 255-CalcInt);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  DisplayData();
  HumOld = Hum;
  delay(5000);
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
  Serial.print(CalcInt,DEC);
   mySerial.print(CalcInt,DEC);
  Serial.print("\t");
   mySerial.print("\t");
  Serial.println(HumOld,DEC);
   mySerial.println(HumOld,DEC);
  //Serial.println(dht_dat[0],DEC);
  delay(1000);  //delay van 1 sec.
}  

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");  
}  


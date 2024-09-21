/*
 Webclient voor:
 - Besturing CV-radiatoren per ruimte
 - Meten van T en RH per ruimte
 - Registreren van aanwezigheid persoon in ruimte
 - Rookmelder per ruimte
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created Jan. 2015, door Jack Cop
 
 */

#include <SPI.h>
#include "DHT.h"

#include <LiquidCrystal.h>
#include <LCDKeypad.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 (besteld bij dealextreme.com
DHT dht(DHTPIN, DHTTYPE);

int Hum = 0;
float Temp = 15.0;
char Move = '-';
int MoveTeller = LOW;
int UitGeweest = 0;

int Verhoog = 0;
int Verlaag = 0;
int TimeCVuit = 5000; //aantal miliseconden geen beweging waarna en CV uitgaat.
int Timer = 0;
int State = LOW;

//volatile long int MaxStookTijd = 3600000; //max. 1 uur stoken daarna weer uit bij beweging opni
volatile long int MaxStookTijd = 1000; //test na 10 seconden uit, stoken daarna weer uit bij beweging opni

int CV = 13;
int pirLedPin = 12;
int potPIN = 0;  // select the input pin for the potentiometer
//int ledPIN = 13; // select the output pin for the LED
int pirPin = 3;    //the digital pin connected to the PIR sensor's output
float potVAL = 0;  // variable to store the value coming from the potentiometer
char potVALchar = 0;
int analogChannel = 0;
char c;
char TempChar[10] = "0";
char RHChar[10] = "0";
int calibrationTime = 5;    //PIR sensor
float SetPoint = 15.0;
float LastsetPoint = 11.0;

LCDKeypad lcd;


void setup() {
 // Open serial communications and wait for port to open:
  pinMode(CV, OUTPUT);
  digitalWrite(CV, LOW);
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  pinMode(pirLedPin, OUTPUT);
  digitalWrite(pirLedPin, HIGH);
  delay(1000);
  digitalWrite(pirLedPin, LOW);
  //digitalWrite(pirPin, LOW);
  //PIRcal();
  //attachInterrupt(1, MoveDetect, CHANGE);
  attachInterrupt(1, MoveDetect, CHANGE);
  dht.begin();  
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("...CV radiator...");
  lcd.setCursor(0,1);
  lcd.print("...Kamer 1...");
  delay(2000);
  lcd.clear();
  
  
  Serial.begin(9600);
  Serial.println("CV is UIT");
  digitalWrite(CV, LOW);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CV is UIT");
  lcd.setCursor(0,1);
  lcd.print("S= ");
  lcd.setCursor(2,1);
  lcd.print(SetPoint);
  lcd.setCursor(8,1);
  lcd.print("T= ");
  lcd.setCursor(10,1);
  lcd.print(Temp);
  //Test van UDP connectie
  //Udp.write("CVaan");
  //delay(5000);
  //Udp.write("CVuit");
  //Udp.endPacket();
  //ThermostaatUit();
}

void MoveDetect()
{
 digitalWrite(pirLedPin, HIGH);
 //digitalWrite(pirLedPin, State);
 //State = !State;
 MoveTeller = HIGH;
 Timer = 0;
 //SetPoint = 15.0;
 //ThermostaatAan();
 //if (MoveTeller >= 3) MoveTeller = 0; // 3x beweging is CV aan
 //MoveTeller++;
 //if (Move == '-') Move = '*';
 //if (Move == '*') Move = '-';
 //lcd.setCursor(14,0);
 //lcd.print(Move);
 //Serial.println("Beweging...PIR AAN");
} 


void PIRcal()
{
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("calibrating PIR sensor");
 lcd.setCursor(0,1); 
 for(int i = 0; i < calibrationTime; i++){
      lcd.print(".");
      delay(1000);
      }
 lcd.print("PIR active");
 delay(500);
}    


void loop()
{
 //digitalWrite(pirLedPin, HIGH);
 lcd.setCursor(2,1);
 lcd.print(SetPoint);
 lcd.setCursor(10,1);
 lcd.print(Temp);
  
 if (lcd.button() == KEYPAD_UP) Verhoog = 1;
 if (lcd.button() == KEYPAD_DOWN) Verlaag = 1;
 if ( (lcd.button() == KEYPAD_NONE) && Verhoog ) {SetPoint += 0.5; Verhoog = 0;}
 if ( (lcd.button() == KEYPAD_NONE) && Verlaag ) {SetPoint -= 0.5; Verlaag = 0;}

 if (Temp > SetPoint) 
  {
   digitalWrite(CV, LOW);
   lcd.setCursor(0,0);
   lcd.print("CV is UIT (Move)");
  } 
 if (Temp <= SetPoint) 
  {   
   digitalWrite(CV, HIGH);
   lcd.setCursor(0,0);
   lcd.print("CV is AAN");
  } 
 
 if (UitGeweest)
  {
    SetPoint = LastsetPoint;
    UitGeweest = 0;
  } 
     
 // Hum = dht.readHumidity(); //leest RH uit in %, was orgineel een float waarde
 Temp = dht.readTemperature(); //leest temperatuur uit in C, was ook een float       
 potVAL = analogRead(potPIN); // read the value from the potentiometer
  
 delay(1); //delay van 1 ms
 Timer++;
 Serial.println(Timer, DEC);
 digitalWrite(pirLedPin, LOW);
 
 if (Timer >= MaxStookTijd)
  {
   LastsetPoint = SetPoint;
   UitGeweest = 1;
   digitalWrite(CV, LOW);
   lcd.setCursor(0,0);
   lcd.print("CV UIT (No Move)");
   Serial.println("CV UIT (No Move)");
   lcd.setCursor(2,1);
   lcd.print("00.00");
   //Timer = 0;
  } 
} 

/*
void loop()
{
  /*  
  lcd.setCursor(2,1);
    lcd.print(SetPoint);
    lcd.setCursor(10,1);
    lcd.print(Temp);
  
    if (lcd.button() == KEYPAD_UP) Verhoog = 1;
    if (lcd.button() == KEYPAD_DOWN) Verlaag = 1;
    if ( (lcd.button() == KEYPAD_NONE) && Verhoog ) {SetPoint += 0.5; Verhoog = 0;}
    if ( (lcd.button() == KEYPAD_NONE) && Verlaag ) {SetPoint -= 0.5; Verlaag = 0;}

    if (Temp > SetPoint) 
     {
      digitalWrite(CV, LOW);
      lcd.setCursor(0,0);
      lcd.print("CV is UIT");
     } 
    if (Temp <= SetPoint) 
     {   
      digitalWrite(CV, HIGH);
      lcd.setCursor(0,0);
      lcd.print("CV is AAN");
     } 
   //} 
  */
  
  /*
  ThermostaatAan();
  if (Timer >= MaxStookTijd) 
   {
    ThermostaatUit();
    MoveTeller = LOW;
    Timer = 0;
    digitalWrite(CV, LOW);
    lcd.setCursor(0,0);
    lcd.print("CV is UIT (No Move)");
    //SetPoint = 10.0;
   } 
  //MoveTeller = LOW;
  //digitalWrite(pirLedPin, LOW);
  
  /*
  if( (digitalRead(pirPin) == LOW) )
   {
    digitalWrite(pirLedPin, HIGH);
    Serial.println("Beweging...PIR AAN");
    Timer = 0;
    
    //digitalWrite(pirPin, HIGH);
   } 
 
   if( (digitalRead(pirPin) == HIGH) )
    {
     digitalWrite(pirLedPin, LOW);
     //Serial.println("PIR UIT");
     //delay(50); 
    } 
 
  */ 
 
 /* 
  if( (digitalRead(pirPin) == LOW) && (Timer > TimeCVuit) )
   {
    digitalWrite(pirLedPin, LOW);   //the led visualizes t
    digitalWrite(CV, LOW);
    lcd.setCursor(0,0);
    lcd.print("CV is UIT");
   } 
  */
  /*    
  Hum = dht.readHumidity(); //leest RH uit in %, was orgineel een float waarde
  Temp = dht.readTemperature(); //leest temperatuur uit in C, was ook een float       
  potVAL = analogRead(potPIN); // read the value from the potentiometer
  
  delay(1); //delay van 1 ms
  Timer++;
  Serial.println(Timer, DEC);
  digitalWrite(pirLedPin, LOW);   
  */
 //} // end loop





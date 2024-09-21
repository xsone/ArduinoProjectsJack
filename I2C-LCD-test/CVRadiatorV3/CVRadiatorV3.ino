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
 Update Nov. 2015
 
 */

/* LET op!!! geen digitale pinnen te gebruiken door combinatie LCD + Ethernet
According DFRobot site, the Ethernet Shield use digital pins 2, 10, 11, 12 & 13; 
it is compatible with the Arduino Ethernet library. 
While the LCD shield use digital pins 5, 6, 7, 8 & 9 and works with the LCB4Bit_Mod library, a modified version of LCD4bits.

Let op!!
Vanwege een pinconfilten tussen de Ethernet en LCD shields kunnen alleen de volgende pinnen worden gebruikt:

Digital: 0, 1, 2, 3, 5, 6, 7
Analog: 2, 3


*/

#include <SPI.h>
#include <DHT.h>
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>
#include <EthernetUdp.h> 

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address for PCF8574

//SDA van LCD naar Arduino Analog pin 4
//SCL van LCD naar Arduino Analog pin 5

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 (besteld bij dealextreme.com
DHT dht(DHTPIN, DHTTYPE);

int Hum = 0;
float Temp = 15.0;
float SetPoint = 15.0;
float LastsetPoint = 11.0;

char Move = '-';
int MoveTeller = 0;
int UitGeweest = 0;

int Verhoog = 0;
int Verlaag = 0;
int TimeCVuit = 5000; //aantal miliseconden geen beweging waarna en CV uitgaat.
int Timer = 0;
int State = LOW;
int motorState = LOW;

//volatile long int MaxStookTijd = 3600000; //max. 1 uur stoken daarna weer uit bij beweging opni
volatile long int MaxStookTijd = 10000; //test na 10 seconden uit, stoken daarna weer uit bij beweging opni

const int CVled = 8; // andere pinnen geven problemen met LCD + ethershield 10,11,12,13 zijn bezet door Ethershield
//const int pirLedPin = 12;
const int potPin = 0;  // select the analog input pin for the potentiometer
//const int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int motorPin1 =  5;   // One motor wire connected to digital pin 5
int motorPin2 =  6;    // One motor wire connected to digital pin 6

//unsigned int potVal = 0;  // variable to store the value coming from the potentiometer
float potVal = 0.0;  // variable to store the value coming from the potentiometer
char potVALchar = 0;
int CVstate = LOW;
char TempChar[10] = "0";
char RHChar[10] = "0";
int calibrationTime = 5;    //PIR sensor


byte mac[] = { 0xDE, 0xAD, 0xBE, 0x00, 0x8E, 0x10 }; // Laatste cijfer MAC steeds 1 ophogen
IPAddress ip(192,168,178,82);
unsigned int localPort = 8888;      

IPAddress RemIP(192, 168, 178, 80); //IP van CV ketel
unsigned int RemPort = 8888;      // local port to listen on

EthernetServer server(82);
EthernetClient client;
EthernetUDP Udp;

const int chipSelect = 4;      // CS for SD card

void setup() {
 // Open serial communications and wait for port to open:
 //pinMode(pirPin, INPUT);
 //digitalWrite(pirPin, LOW);
 //PIRcal();
 //attachInterrupt(1, MoveDetect, CHANGE);

 pinMode(CVled, OUTPUT);
 pinMode(motorPin1, OUTPUT); 
 pinMode(motorPin2, OUTPUT); 
 
 Serial.begin(9600);
 Wire.begin();
 Ethernet.begin(mac, ip);
 Udp.begin(localPort);
 server.begin();
 dht.begin();  
 
 Temp = dht.readTemperature(); //leest temperatuur uit in C, was ook een float     
 rotateLeftFull(5000); //kraan altijd dicht

 // LCD int en startscherm
 lcd.begin(16,2);
 lcd.backlight();
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(Ethernet.localIP());
 delay(3000);
 lcd.clear();
 lcd.print("...Radiator 1...");
 lcd.setCursor(0,1);
 lcd.print("...Kamer Jack...");
 delay(3000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("CV=UIT");
 lcd.setCursor(0,1);
 lcd.print("S= ");
 lcd.setCursor(2,1);
 lcd.print(SetPoint,1);
 lcd.setCursor(8,1);
 lcd.print("T= ");
 lcd.setCursor(10,1);
 lcd.print(Temp,1);
 delay(3000);

 //Start test
 Serial.begin(9600);
 Serial.println(Ethernet.localIP());
 Serial.println("Start programmaloop");
 //Show();
}

void MoveDetect()
{
 //digitalWrite(pirLedPin, State);
 //State = !State;
 //MoveTeller = HIGH;
 Timer = 0;
 //SetPoint = 15.0;
 //ThermostaatAan();
 //if (MoveTeller >= 3) MoveTeller = 0; // 3x beweging is CV aan
 //MoveTeller++;
 //if (Move == '-') Move = '*';
 //if (Move == '*') Move = '-';
 //lcd.setCursor(14,0);
 //lcd.print(Move);
 Serial.print("Beweging... ");
 Serial.println(MoveTeller++, DEC);
} 


void PIRcal()
{
 //lcd.clear();
 //lcd.setCursor(0,0);
 //lcd.print("calibrating PIR sensor");
 //lcd.setCursor(0,1); 
 for(int i = 0; i < calibrationTime; i++){
      //lcd.print(".");
      delay(1000);
      }
 //lcd.print("PIR active");
 delay(500);
}    

//Display data
void Show()
{
 lcd.setCursor(0,0);
 lcd.write("CV=");
 lcd.setCursor(3,0);
 lcd.write(CVstate);
 Serial.print("CVState: ");
 Serial.println(CVstate, DEC);
 Serial.print("MotorState: ");
 Serial.println(motorState, DEC);
 Serial.print("PotVal: ");
 Serial.println(potVal, 1); 
 Serial.print("SetPoint: ");
 Serial.println(SetPoint,1); 
 Serial.print("Temp: ");
 Serial.println(Temp,1);
 Serial.println();
} 

// GEEN LCD berichten in opnemen gaat fout!
void CVaan()
{
 CVstate = HIGH;
 digitalWrite(CVled, HIGH);
 Udp.beginPacket(RemIP, RemPort);
 Udp.write("CV=AAN");
 Udp.endPacket();
 lcd.setCursor(0,0);
 lcd.write("CV=AAN");
 //Serial.println(CVstate, DEC);
 //Serial.print("SetPoint: ");
 //Serial.println(SetPoint,1); 
 //lcd.setCursor(0,0);
 //lcd.write("CV is AAN");
 //lcd.setCursor(2,1);
 //lcd.print(SetPoint,1);
 //if (motorState == LOW) rotateRightFull(5000); //Radiatorkraan Open
}  

// GEEN LCD berichten in opnemen gaat fout!
void CVuit()
{
 CVstate = LOW;
 digitalWrite(CVled, LOW);
 Udp.beginPacket(RemIP, RemPort);
 Udp.write("CV=UIT");
 Udp.endPacket();
 lcd.setCursor(0,0);
 lcd.write("CV=UIT");
 //Show();
 //Serial.println(CVstate, DEC);
 //Serial.print("SetPoint: ");
 //Serial.println(SetPoint,1); 
 //lcd.setCursor(0,0);
 //lcd.write("CV is UIT");
 //lcd.setCursor(2,1);
 //lcd.print(SetPoint,1);
 //if (motorState == HIGH) rotateLeftFull(5000); //Radiatorkraan Dicht
} 

void loop()
{
 // Hum = dht.readHumidity(); //leest RH uit in %, was orgineel een float waarde
 Temp = dht.readTemperature(); //leest temperatuur uit in C, was ook een float       
 potVal = analogRead(potPin); // read the value from the potentiometer
 SetPoint = potVal/4; //bereik Potmater 0-128, bereik Setpoint 0-30 C dus delen door 4
 
//Serial.print("SetPoint: ");
// Serial.println(SetPoint,1); 
/*  
 Serial.print("PotVal: ");
 Serial.println(potVal, 1); 
 Serial.print("SetPoint: ");
 Serial.println(SetPoint,1); 
 Serial.print("Temp: ");
 Serial.println(Temp,1); 
*/ 
  
 //digitalWrite(CVled, CVstate);
 //lcd.setCursor(2,1);
 //lcd.print(SetPoint,1); //float op 1 dec achter komma
 //lcd.setCursor(10,1);
 //lcd.print(Temp,1);
 //if (lcd.button() == KEYPAD_UP) Verhoog = 1;
 //if (lcd.button() == KEYPAD_DOWN) Verlaag = 1;
 //if ( (lcd.button() == KEYPAD_NONE) && Verhoog ) {SetPoint += 0.5; Verhoog = 0;}
 //if ( (lcd.button() == KEYPAD_NONE) && Verlaag ) {SetPoint -= 0.5; Verlaag = 0;}
 //if (Temp > SetPoint && CVstate == HIGH ) //hoort bij PIR sensor
 if (Temp >= SetPoint && CVstate == HIGH) CVuit();
 //if (Temp <= SetPoint && CVstate == LOW) //hoort bij PIR sensor
 if (Temp < SetPoint && CVstate == LOW) CVaan();
 
/* 
 if (UitGeweest)
  {
    SetPoint = LastsetPoint;
    UitGeweest = 0;
  } 
 */ 
 
 /* 
 delay(1); //delay van 1 ms
 Timer++;
 Serial.println(Timer, DEC);
 digitalWrite(pirLedPin, LOW);
 */
 /*
 if (Timer == MaxStookTijd)
  {
   LastsetPoint = SetPoint;
   UitGeweest = 1;
   Timer = 0;
   CVuit();
   Serial.println("StookTimer... ");
  }
 Timer++;
 */
 lcd.setCursor(2,1);
 lcd.print(SetPoint,1);
 lcd.setCursor(10,1);
 lcd.print(Temp,1);
 //Show();
 delay(500); //vertraag 10ms
}

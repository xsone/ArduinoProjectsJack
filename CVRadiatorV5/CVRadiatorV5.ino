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
 Update Dec. 2015
 Update Feb. 2016
 
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

#define DHTPIN 2          // DHT 11 aan pin 2
#define DHTTYPE DHT11     // DHT 11 (besteld bij dealextreme.com
#define SAMPLES 10        // nodig voor stabiele meting setpoint potmeter
DHT dht(DHTPIN, DHTTYPE); // Initialisatie DHT 11

//Speciaal charater voor Smiley
byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

//Algemene declaraties
int Hum = 0;
float Temp = 15.0;
int SetPoint = 15;
int SetPointOld = 15;
//float SetPoint = 15.0;
int Timer = 0;
int State = LOW;
int motorState = LOW;
volatile long int MaxStookTijd = 8; //test na 10 seconden uit, stoken daarna weer uit bij beweging opni

//Bewegingssensor
char Move = '-';
int MoveTeller = 0;
int UitGeweest = 0;

//IO definties
const int CVled = 7;          // Led voor CV aan/uit, andere pinnen geven problemen met LCD + ethershield 10,11,12,13 zijn bezet door Ethershield
const int pirPin = 3;         // pin voor input pir-bewegingssensor DS-SUN
const int potPin = A1;        // analog input pin for the potentiometer
const int motorPin1 =  5;     // 1e motor daard aan digital pin 5
const int motorPin2 =  6;     // 2e motor draad aan digital pin 6
volatile int pirState = LOW;  // status van pir-bewegingssensor

//unsigned int potVal = 0;  // variable to store the value coming from the potentiometer
int potVal = 0.0;  // variable to store the value coming from the potentiometerchar potVALchar = 0;
int CVstate = LOW;
int Aan = 0;
int Uit = 0;
int ipReset = 0;

//Ethernet instellingen kamerthermostaat
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x00, 0x8E, 0x11 }; // Laatste cijfer MAC steeds 1 ophogen
//IPAddress ip(192,168,178,81);
//IPAddress gateway(192,168,178,1);
byte ip[]  = { 192, 168, 178, 81 };                  // IP-Adres van ethershield CV-ketel; kamers 81, 82 etc.
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };               // standdaard subnet 
byte phpserver[] = { 192, 168, 178, 20 };            // IP van de NAS-server
unsigned int localPort = 8888;      
boolean alreadyConnected = false; // whether or not the client was connected previously

//Ethernet instellingen CVketel server
IPAddress RemIP(192, 168, 178, 80); //IP van CV ketel
unsigned int RemPort = 8888;      // local port to listen on

//Ethernet server
//EthernetServer server(81);
//EthernetClient client;
EthernetUDP Udp;

//Startup
void setup() 
{
 // Definitie van I/O
 pinMode(CVled, OUTPUT);
 pinMode(motorPin1, OUTPUT); 
 pinMode(motorPin2, OUTPUT);
 pinMode(pirPin, INPUT);

 Serial.begin(9600);

  // Initialisatie alle comm. bussen 
 Wire.begin();
 dht.begin();  
 
 // LCD initialisatie en startscherm
 lcd.createChar(2, smiley);
 lcd.begin(16,2);
 lcd.backlight();
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Connecting IP...");

 Ethernet.begin(mac,ip, gateway, subnet); //vast IP adres
 // arduinoCVserver.begin();
 Serial.println(Ethernet.localIP());
 
 
 //server.begin();
 Udp.begin(localPort);
 
 //Serial test start
 Serial.println("Start programmaloop");
 Serial.println(Ethernet.localIP());

 //Start basisfunctie kamerthermostaat
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(Ethernet.localIP());
 delay(1000);
 lcd.clear();
 lcd.print("...Radiator 1...");
 lcd.setCursor(0,1);
 lcd.print("...Kamer Jack...");
 delay(1000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("CVuit");
 lcd.setCursor(0,1);
 lcd.print("S= ");
 lcd.setCursor(2,1);
 lcd.print(SetPoint,1);
 lcd.setCursor(8,1);
 lcd.print("T= ");
 lcd.setCursor(10,1);
 lcd.print(Temp,1);
 delay(1000);


 //Start metingen
 Temp = dht.readTemperature(0); //leest temperatuur uit in C = 0, was ook een float     
 //rotateDichtFull(5000); //kraan altijd dicht
 SetPoint = SetPointOld;
 CVuit();
}

//Bewegingssensor
void MoveDetect()
{
 if(digitalRead(pirPin) == HIGH)
  {
  lcd.backlight();
  lcd.setCursor(15,0);
  lcd.write(2);
  //lcd.write("█");
  pirState = HIGH;
  Timer = 0;
  }
 if(digitalRead(pirPin) == LOW)
  {
   lcd.setCursor(15,0);
   lcd.write("_");
   pirState = LOW;
   Timer++;
  }

  lcd.setCursor(10,0);
  lcd.print(Timer);
} 

void softReset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");
}  

// Post CVaan bericht, display en regel motor
void CVaan()
{
 CVstate = HIGH;
 digitalWrite(CVled, HIGH);
 Udp.beginPacket(RemIP, RemPort);
 Udp.write("CVaan");
 Udp.endPacket();
 lcd.setCursor(0,0);
 lcd.write("CVaan");
 //rotateOpenFull(15000);
 //delay(3000);
 //if (motorState == LOW) rotateRightFull(5000); //Radiatorkraan Open
}  

// Post CVuit bericht, display en regel motor
void CVuit()
{
 CVstate = LOW;
 digitalWrite(CVled, LOW);
 Udp.beginPacket(RemIP, RemPort);
 Udp.write("CVuit");
 Udp.endPacket();
 lcd.setCursor(0,0);
 lcd.write("CVuit");
 //rotateDichtFull(15000);
 //delay(3000);
 //Show();
 //if (motorState == HIGH) rotateLeftFull(5000); //Radiatorkraan Dicht
} 

void loop()
{
 // Hum = dht.readHumidity(); //leest RH uit in %, was orgineel een float waarde
 Temp = dht.readTemperature(); //leest temperatuur uit in C, was ook een float       
 
 //for (int i=0; i < SAMPLES ; i++) 
 // {
   potVal = analogRead(potPin); 
 //  potVal /= SAMPLES ;
 // }
 
 //SetPoint = (potVal+0.5)/8; //bereik Potmater 0-128, bereik Setpoint 0-30 C dus delen door 4
 SetPoint = (potVal)/20; //bereik Potmater 0-128, bereik Setpoint 0-30 C dus delen door 4 

 if ( (pirState == LOW) && (Timer > MaxStookTijd) ) { CVuit(); lcd.noBacklight(); Timer = MaxStookTijd; }
 else
  {
   if (Temp > SetPoint && CVstate == HIGH) CVuit();
   //if (Temp <= SetPoint && CVstate == LOW) //hoort bij PIR sensor
   if (Temp < SetPoint && CVstate == LOW) CVaan();
  } 
 
 MoveDetect();
 ShowSetPoint();
 delay(500); //vertraag 10ms
}

// Display actuele status
void ShowSetPoint()
 {
 lcd.setCursor(2,1);
 lcd.print("    ");
 lcd.setCursor(2,1);
 lcd.print(SetPoint);
 lcd.setCursor(10,1);
 lcd.print("    ");
 lcd.setCursor(10,1);
 lcd.print(Temp,1);
 SetPointOld = SetPoint;
 }


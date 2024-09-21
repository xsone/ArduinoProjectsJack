/*
 Webserver voor:
 - Besturing CV ketel
 - Toerenregeling douche-ventilator o.b.v. RH en T
 - Inlezen energieopbrengst Zonnepanelen STECA-inventor met KWh-meter (So uitgang)
 - ATMEGA 1250 kiezen als board
 created mei 2015, door Jack Cop
 */
#include <Ethernet.h>
#include <EthernetUdp.h>     // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <Time.h>

#include <SPI.h>             // needed for Arduino versions later than 0018
#include "DHT.h"   // nodig voor Temp + RH sensor

#define DHTPIN 2       // what pin we're connected to op DUE
#define DHTTYPE DHT11  // DHT 11 (besteld bij dealextreme.com
#define anout_pin 9    // toerental regeling ventilator 0-10V (van 5V omgezet naar 10V m.b.v transistor)

// Enter a MAC address and IP address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x00, 0x8E, 0x10 }; // Laatste cijfer MAC steeds 1 ophogen, kamer 1 0x11, 0x12 etc.
byte ip[]  = { 192, 168, 178, 80 };                  // IP-Adres van ethershield CV-ketel; kamers 81, 82 etc.
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };               // standdaard subnet 
byte phpserver[] = { 192, 168, 178, 20 };            // IP van de NAS-server

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetClient client;
EthernetUDP Udp;
unsigned int localPort = 8888;                       // local port to listen on
EthernetServer CVserver(localPort); //was poort 80 i.v.m. exteren toegang nu 85
boolean alreadyConnected = false; // whether or not the client was connected previously
String httpData = "";
boolean SetpointAndroidChanged = false;
boolean SetpointArduinoChanged = false;

//Temp/Hum sensor
DHT dht(DHTPIN, DHTTYPE);

// CV systeem
int Relay = 22;  // relay output voor sturen CV-ketel
int S0int = 0;   //pin 3 met interrupt 0;
int TestLed = 8;

// Startwaarden diverse variabelen
String Kamer = "?";
String Temperature = "00.0";
String Setpoint = "00";
String SetpointArduino = "00"; //afkomstig van vast Arduino device
String SetpointArduinoOld = "00"; //afkomstig van vast Arduino device
String SetpointAndroid = "00"; //afkomstig van mobiel Android device
String Humidity = "00.0";
String CVstate = "UIT";
String Move = "UIT";
String Smoke = "UIT";
String Koolmonox = "UIT";
String ZonPuls = "0";
String ZonKWh = "0";


// Tijd instellingen
time_t t = 0UL; 
String tijdNu;
String tijdLog = "21:00:00";

// Zonenergiemeter via S0 uitgang Interupt
float ZonKWH; // totale KHW waarde zonnepanelen
float ZonSUM; // watt teller zonnepanelen in 0.5W per puls
volatile unsigned long rpk = 0;               // raw pulse KWH counter (private)
volatile unsigned long rpk_old = 0;           // value of last read (private)
volatile boolean CS_KWH = false;              // Critical Section for KWH meter (private)



void setup() 
{
 pinMode(Relay, OUTPUT);
 pinMode(TestLed, OUTPUT);
 pinMode(S0int, INPUT);
  
  // init Serial
 Serial.begin(9600);
 Serial.println("UDP RW Programma gestart v1.4...");
 Serial.println(CVstate); 

 // disable SD card if one in the slot
 pinMode(4,OUTPUT);
 digitalWrite(4,HIGH);
 pinMode(10,OUTPUT);
 digitalWrite(10,HIGH);
 pinMode(53,OUTPUT); //Mega
 digitalWrite(53,HIGH);
 
 // init Ethernet and UDP:
 Ethernet.begin(mac,ip,gateway,subnet);
 //Udp.begin(localPort);
 
 //if (Ethernet.localIP() > 0)
 // {
   //CVserver.begin();
   //Udp.begin(localPort);
   Serial.println(Ethernet.localIP());
   for (int n = 0; n < 5; n++)
    {
     digitalWrite(TestLed, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(500);              // wait for a second
     digitalWrite(TestLed, LOW);    // turn the LED off by making the voltage LOW
     delay(500);
    }
 // } 
 //else softwareReset();

 /*  
 // haal NTP tijd op 
 now(); // haal actuele tijd op
 //Serial.println(Ethernet.localIP());
 setSyncProvider(getNtpTime);
 int tel = 0;
 while(timeStatus()== timeNotSet){
   Serial.print(".");
   digitalWrite(8, HIGH); //check of de timesync werkt
   delay(1000);
   digitalWrite(8, LOW);
   delay(1000);
   tel++;
   if (tel > 5){
      break; // na 15 sec geen NTP-time dan afbreken, cursos staat dan op 15
    }  
 }; // wachten op timesync door sync provider
  
 setTime(hour(),minute(),second(),day(),month(),year()); // Tijd instellen op de Arduino
 tijdNu = getTimeString();
 Serial.println(tijdNu);
 */

 //Zonnepanelen opbrangst via S0
 pinMode(S0pin, INPUT_PULLUP);    //sensorinput Gas INT1
 sensorIsOn = false; 

 // init sensors
 //dht.begin();
 
 // KWH interrupt attached to IRQ 0  = pin2
 //attachInterrupt(S0int, Kirq, FALLING);

 //test relais werking
 digitalWrite(Relay, HIGH);
 delay(2000);
 digitalWrite(Relay, LOW);
}



void loop() 
{
 //CVreadUDP();
 URLwrite();
 //if (SetpointArduinoChanged) { Setpoint = SetpointArduino; SetpointArduinoChanged = false;}
 //if (SetpointAndroidChanged) { Setpoint = SetpointAndroid; SetpointAndroidChanged = false;}
 
  //else Setpoint = SetpointArduino;
 //if (CVstate = "AAN") digitalWrite(Relay, HIGH); 
 //if (CVstate = "UIT") digitalWrite(Relay, LOW); 
 //ZonReadInt();
 //RHTread();
 ZonRead();
 
 //Serial.println(Kamer + CVstate + Humidity + Temperature + Move + Smoke);
 //delay(1000);
}

void resetEthernetShield() //reset when disconnected from the server
{  
  delay(100);
  Serial.println("reset ethernet");
  client.stop();
  Ethernet.begin(mac,ip,gateway,subnet);
  delay(1000);
}



void softwareReset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");  
}  


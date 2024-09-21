/*
 Webserver voor:
 - Besturing CV ketel
 - Toerenregeling douche-ventilator o.b.v. RH en T
 - Inlezen energieopbrengst Zonnepanelen STECA-inventor met KWh-meter (So uitgang)
 
 created mei 2015, door Jack Cop
 */
#include <Ethernet.h>
#include <EthernetUdp.h>     // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <Time.h>

#include <SPI.h>             // needed for Arduino versions later than 0018
//#include <LiquidCrystal.h>
//#include <LCDKeypad.h>
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
EthernetServer arduinoserver(81); //was poort 80 i.v.m. exteren toegang nu 85
//EthernetServer server(80); //was poort 80 i.v.m. exteren toegang nu 85

//Temp/Hum sensor
DHT dht(DHTPIN, DHTTYPE);

// CV systeem
//int Relay = 22;  // relay output voor sturen CV-ketel
int Relay = 8;  // relay output voor sturen CV-ketel test bij duemilanoive
int S0int = 0;   //pin 3 met interrupt 0;

// Startwaarden diverse variabelen
String CVstate = "CV???";
String Humidity = "0.0";
String Temperatuur = "0.0";
String ZonPuls = "0";
String ZonKWh = "0";
String Beweging = "NEE";
String Rook = "NEE";
String Koolmonox = "NEE";

// Tijd instellingen
//time_t t = 0UL; 
String tijdNu;
String tijdLog = "21:00:00";

// Zonenergiemeter via S0 uitgang Interupt
float ZonKWH; // totale KHW waarde zonnepanelen
float ZonSUM; // watt teller zonnepanelen in 0.5W per puls
volatile unsigned long rpk = 0;               // raw pulse KWH counter (private)
volatile unsigned long rpk_old = 0;           // value of last read (private)
volatile boolean CS_KWH = false;              // Critical Section for KWH meter (private)

// LCD keypad
//LCDKeypad lcd;


void setup() 
{
  // init LCD
  //lcd.begin(16, 2);
  
  // init Serial
  Serial.begin(9600);
  Serial.println("UDP RW Programma gestart...");
  Serial.println(CVstate); 
  
  // init Ethernet and UDP:
  Ethernet.begin(mac,ip,subnet); //vast IP adres
  arduinoserver.begin();
  Udp.begin(localPort);
  Serial.println(Ethernet.localIP());
  delay(1000);
  Serial.println(Ethernet.localIP());
 
   
  // haal NTP tijd op 
  //now(); // haal actuele tijd op
  Serial.println(Ethernet.localIP());
  Serial.println("Starting to sync the time via NTP.");
  setSyncProvider(getNtpTime);
  int tel = 0;
  while(timeStatus()== timeNotSet){
    Serial.print("."); //lcd.setCursor(tel,0); lcd.print("."); //uitvoer
    digitalWrite(Relay, HIGH); //check of de timesync werkt
    delay(1000);
    digitalWrite(Relay, LOW);
    delay(1000);
    tel++;
    if (tel > 15){
      //lcd.clear(); 
      break; // na 15 sec geen NTP-time dan afbreken, cursos staat dan op 15
    }  
  }; // wachten op timesync door sync provider
  
  setTime(hour(),minute(),second(),day(),month(),year()); // Tijd instellen op de Arduino
  tijdNu = getTimeString();
  Serial.println(tijdNu); //lcd.clear(); lcd.setCursor(0,0); lcd.print(tijdNu); // uitvoer
 
 
  // init sensors
  dht.begin();
  pinMode(Relay, OUTPUT);
  pinMode(S0int, INPUT);

  // KWH interrupt attached to IRQ 0  = pin2
  //attachInterrupt(S0int, Kirq, FALLING);

  //test relais werking
  digitalWrite(Relay, HIGH);
  delay(2000);
  digitalWrite(Relay, LOW);
}



void loop() 
{
 //LCDbericht();
 CVreadUDP();
 //ZonReadInt();
 RHTread();
 //Serial.println(CVstate, Humidity, Temperatuur, ZonPuls, ZonKWh);
 //LcdWrite(CVstate, Humi, Tempe, ZonPuls, ZonKWh);
 URLwrite(CVstate, Humidity, Temperatuur, ZonPuls, ZonKWh);
 delay(1000);
}

/*
void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");  
}  
*/

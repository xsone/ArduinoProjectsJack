/*
 Webserver voor:
 - Besturing CV ketel
 - Toerenregeling douche-ventilator o.b.v. RH en T
 - Inlezen energieopbrengst Zonnepanelen STECA-inventor met KWh-meter (So uitgang)
 
 created jan 2015, door Jack Cop
 */


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <LiquidCrystal.h>
#include <LCDKeypad.h>
#include "DHT.h"

#define DHTPIN 24     // what pin we're connected to op MEGA
#define DHTTYPE DHT11   // DHT 11 (besteld bij dealextreme.com
#define anout_pin 9 // toerental regeling ventilator 0-10V (van 5V omgezet naar 10V m.b.v transistor)

// Enter a MAC address and IP address for your controller below.
byte mac[] = { 0xDF, 0xA2, 0xDA, 0x00, 0x8A, 0x13 };
IPAddress ip(192, 168, 178, 18);

unsigned int localPort = 8888;      // local port to listen on
//unsigned int localPort = 80;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
EthernetClient client;
EthernetServer server(81); //was poort 80 i.v.m. exteren toegang nu 85
//EthernetServer server(80); //was poort 80 i.v.m. exteren toegang nu 85
LCDKeypad lcd;
DHT dht(DHTPIN, DHTTYPE);

//int Relay = 22;
int Relay = 5;

String CVstate = "CV unknown state";
String Humidity = "0.0";
String Temperatuur = "0.0";
String ZonPuls = "0";
String ZonKWh = "0";
String Beweging = "NEE";
String Rook = "NEE";
String Koolmonox = "NEE";

void setup() 
{
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  server.begin();
  Udp.begin(localPort);
  //attachInterrupt(4, ZonRead, FALLING);   //interrupt 4 digital pin 19 MEGA-board

  Serial.begin(9600);
  Serial.println("UDP RW Programma gestart...");
  Serial.println(Ethernet.localIP());

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("...UDP...");
  lcd.setCursor(0,1);
  lcd.print("...Test...");
  delay(5000); //laat 5 sec. startscherm zien 
  lcd.clear();
  
  dht.begin();
  pinMode(Relay, OUTPUT); 
 
  //Serial.println(CVstate); 

  //digitalWrite(Relay, HIGH);
  //delay(1000); 
  //digitalWrite(Relay, LOW);
}

void loop() 
{
 CVreadUDP();
 //Serial.println(CVstate);
 delay(1000);
 ZonRead();
 delay(1000);
 RHTread();
 //SerialWrite(CVstate, Humi, Tempe, ZonPuls, ZonKWh);
 //LcdWrite(CVstate, Humi, Tempe, ZonPuls, ZonKWh);
 URLwrite(CVstate, Humidity, Temperatuur, ZonPuls, ZonKWh);
 delay(1000);
}

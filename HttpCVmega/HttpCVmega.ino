/*
 Webserver voor:
 - Besturing CV ketel
 - Toerenregeling douche-ventilator o.b.v. RH en T
 - Inlezen energieopbrengst Zonnepanelen STECA-inventor met KWh-meter (So uitgang)
 
 created mei 2015, door Jack Cop
 */
#include <Ethernet.h>
#include <EthernetUdp.h>     // UDP library from: bjoern@cs.stanford.edu 12/30/2008
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
//EthernetClient client;
boolean alreadyConnected = false; // whether or not the client was connected previously

EthernetServer arduinoCVserver(8888); //was poort 80 i.v.m. exteren toegang nu 85

//Temp/Hum sensor
DHT dht(DHTPIN, DHTTYPE);

// CV systeem
//int Relay = 22;  // relay output voor sturen CV-ketel
int Relay = 9;  // LED test relay output voor sturen CV-ketel
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

String httpData = "";


void setup() 
{
  // init Serial
  Serial.begin(9600);
  Serial.println("Http RW Programma gestart...");
  Serial.println(CVstate); 
  
  // init Ethernet and TCP:
  Ethernet.begin(mac,ip, gateway, subnet); //vast IP adres
  arduinoCVserver.begin();
  Serial.println(Ethernet.localIP());

  // init sensors
  dht.begin();
  pinMode(Relay, OUTPUT);
  pinMode(S0int, INPUT);

  //test relais werking
  digitalWrite(Relay, HIGH);
  delay(2000);
  digitalWrite(Relay, LOW);
}


/*
void loop() 
{
 //EthernetClient client = arduinoCVserver.available(); 
 //CVreadTCP();
 //ZonReadInt();
 //RHTread();
 //Serial.println(CVstate, Humidity, Temperatuur, ZonPuls, ZonKWh);
 //LcdWrite(CVstate, Humi, Tempe, ZonPuls, ZonKWh);
 URLwrite(CVstate, Humidity, Temperatuur, ZonPuls, ZonKWh);
 delay(1000);
}
*/


void loop()
{
  EthernetClient client = arduinoCVserver.available();
  if (client) 
   {
    //Serial.println("Got a client");
    boolean currentLineIsBlank = true;
    if (!alreadyConnected) 
       {
        // clear out the input buffer:
        client.flush();
        Serial.println("We have a new client");
        //client.println("Hello, client!");
        alreadyConnected = true;
       }
    if (client.available() > 0)    
     {  
      char c = client.read();
      httpData += c;
      if (c == '\n' && currentLineIsBlank) 
       {
          // send a standard http response header
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Arduino CV Control</title>");
          client.println("</head>");
          //client.println("<meta http-equiv=refresh content=2>");
          client.println("<body bgcolor=\"#E0FFFF\">");
          client.println("<center><p><h1><align=center>Energylogger & Heatingcontrol Webserver  v1.3  </h1></p> ");
          client.println("<center><p><h1><align=center>");
          //client.println(String(tijdNu));
          client.println("<form method=\"get\">");
          client.println("</form>");
          client.println("</body>");
          client.println("</html>");
          
          
        /*  
          client.println("</h1></p> ");
          client.println("<table border=5>");
          client.println("<COL CLASS=kolom1><COL CLASS=kolom2><COL CLASS=kolom3><COL CLASS=kolom4><COL CLASS=kolom5>");
          client.println("<TH CLASS=cel1 links>KAMER</TH><TH CLASS=cel2>CV-STATE</TH><TH CLASS=cel3>HUM</TH><TH CLASS=cel4>TEMP</TH><TH CLASS=cel5>AANWEZIG?</TH>");
          client.println("<TR><TD CLASS=links>JACK</TD><TD>");
          client.println("CV status: " + String(CVstate));
          client.println("</TD><TD>");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("</TD><TD>");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("</TD></TR>");
          client.println("<TR><TD CLASS=links>MAURITS</TD><TD>");
          client.println("CV status: " + CVstate);
          client.println("</TD><TD>");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("</TD><TD>");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("</TD></TR>");
          client.println("<TR><TD CLASS=links>WOONKAMER</TD><TD>");
          client.println("CV status: " + String(CVstate));
          client.println("</TD><TD>");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("</TD><TD>");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("</TD></TR>");
          //client.println("</TD><TD>");
          //client.println(WaterLtrWaarde);
          //client.println("</TD><TD>C</TD><TD>");
          client.println("<TR><TD CLASS=links>ZON PULS</TD><TD>");
          //client.println("Zonpuls: " + String(ZonSUM, 4) + " pls");
          //client.println(ElecIntWaarde);
          client.println("</TD><TD>");
          //client.println("ZonKWh: " + String(ZonKWH, 4) + " KWh");
          client.println("</TD><TD>KWh</TD><TR>");
          client.println("<TR><TD CLASS=links>SOLAR</TD><TD>0</TD><TD>0</TD><TD>KWh</TD></TR>");
          client.println("<TR><TD CLASS=links>WIND</TD><TD>0</TD><TD>0</TD><TD>KWh</TD></TR>");
          client.println("</TABLE><br>");
          client.println();
          // print the current readings, in HTML format:
          client.println("<br />");
          client.println("CV status: " + String(CVstate));
          client.println("<br />");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("<br />");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("<br />");
          client.println("Zonpuls: " + String(ZonPuls) + " p");
          client.println("<br />");
          client.println("ZonKWh: " + String(ZonKWh)+ " KWh");
          client.println("<br />");
          client.println();
          client.println();
          client.println("Kamer Jack");
          client.println("<br />");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("<br />");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("<br />");
          client.println("Beweging: " + String(Beweging) + " C");
          client.println("<br />");
          client.println("Rook: " + String(Rook) + " C");
          client.println("<br />");
          client.println("Koolmonox: " + String(Koolmonox) + " C");
          client.println("<br />"); 
          client.println("</body>");
          client.println("</html>");
       */   

        Serial.println(httpData);
        Serial.println(httpData.substring(5,15));
        if ( httpData.substring(5,15) == "?cmd=CVaan") { Serial.println("CV AAN"); CVstate = "CVaan"; digitalWrite(Relay, HIGH); }
        if ( httpData.substring(5,15) == "?cmd=CVuit") { Serial.println("CV UIT"); CVstate = "CVuit"; digitalWrite(Relay, LOW); }
        httpData = "";    // finished wi
        //break;
       }//end curr line blanc
      if (c == '\n') {currentLineIsBlank = true;} 
      else if (c != '\r') {currentLineIsBlank = false;}
     } // end client avail
    } // end if client  
}  




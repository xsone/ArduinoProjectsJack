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
#include <Ethernet.h>
#include <EthernetUdp.h> 

int CV = 13;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0x00, 0x8E, 0x13 };
IPAddress ip(192,168,178,15);
unsigned int localPort = 8888;      // local port to listen on

IPAddress RemIP(192, 168, 178, 18); //IP van CV ketel
unsigned int RemPort = 8888;      // local port to listen on

EthernetServer server(82);
EthernetClient client;
EthernetUDP Udp;


void setup() {
 // Open serial communications and wait for port to open:
  pinMode(CV, OUTPUT);
  digitalWrite(CV, LOW);
 
  //Ethernet.begin(mac, ip);
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  server.begin();
  
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());
  digitalWrite(CV, LOW);

  //Test van UDP connectie
  //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  //Udp.write("CVaan");
  //delay(5000);
  //Udp.write("CVuit");
  //Udp.endPacket();
  //delay(10);
  //ThermostaatUit();
}

void loop()
{
  Udp.beginPacket(RemIP, RemPort);
  Udp.write("CVaan");
  //Udp.write("CVuit");
  Udp.endPacket();
  delay(5000);
  Udp.beginPacket(RemIP, RemPort);
  Udp.write("CVuit");
  //Udp.write("CVuit");
  Udp.endPacket();
  
} 


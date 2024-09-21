/*

  Versie info:
  -------------
  File name: EnergieLoggerMegaPHPtime_pde
  Werking: zend iedere 24 uur een waarde naar de database MySQL "Energielogger" o.b.v. Timeserver
  (c) Jack Cop 2012-2014
  versie 2.0
    
*/

#include <Adafruit_CC3000.h>
#include <ccspi.h>
//#include "utility/NetTime.h"
#include <string.h>
#include "utility/debug.h"
#include "utility/sntp.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed but DI

#define WLAN_SSID       "TRAFFICBOX2"   // cannot be longer than 32 characters!
#define WLAN_PASS       "FFFFF99999"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA
//#define WLAN_SECURITY   WLAN_SEC_WEP



#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <Time.h>

//#define TZ_OFFSET -7200L // time zone offset - set deze offset in seconds to.v. je local time
#define TZ_OFFSET -3600L  // time zone offset - set deze offset in seconds to.v. je local time //Zomertijd dan -2hrs (-7200L sec); Wintertijd dan -1hrs (-3600L sec)

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 }; // MAC-ADRES Ethershield zie onderkant

byte ip[]  = { 192, 168, 178, 30 };                // IP-Adres van ethershield meterkast
//byte ip[]  = { 192, 168, 178, 38};                  // IP-Adres van ethershield test
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };
//byte server[] = { 192, 168, 178, 20 };                 // IP van de NAS-server

//Ethernetcc3000 cc3000;
EthernetUDP Udp;

//Server server(80);
unsigned int localPort = 8888;

IPAddress timeServer(132, 163, 4, 101); // time-c.timefreq.bldrdoc.gov NTP server werkt!
// IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov
//IPAddress timeServer(131, 211, 8, 244); // pool.ntp.org NTP server in Ziggo router werkt soms! Onbetrouwbaar

const long NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message


byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// Tijd instellingen
time_t t = 0UL; 
String tijdNu;
String tijdLog = "21:03:00";

// Sensor reading
volatile long int GasIntWaarde=0;
volatile long int GasLtrWaarde=0;
volatile long int GasM3Waarde=0;
volatile long int WaterIntWaarde=0;
volatile long int WaterLtrWaarde=0;
volatile long int WaterM3Waarde=0;
volatile long int ElecKwhWaarde = 0;

int ElecIntWaarde = 0;
int ElecSensorWaarde = 0;
int ZwartWaarde=0; //detectie zwarte vlakje KWh
int InteruptTellerGas = 0;
int InteruptTellerWater = 0;
int zwartwaarde = 0;

// Initialisatie en starten
void setup()
{
 
 
 
 
 
 
 
 
 
 
  pinMode(2, INPUT);    //sensorinput Water INT0
  pinMode(3, INPUT);    //sensorinput Gas INT1
  pinMode(6, OUTPUT);  //LED indicator KWh meter
  pinMode(5, OUTPUT);  //LED indicator Gas meter
  pinMode(4, OUTPUT);  //LED indicator Water meter
  
  digitalWrite(6, LOW); //KWh meter
  digitalWrite(5, LOW); //Gas meter
  digitalWrite(4, LOW); //Water meter
    
  attachInterrupt(0, WaterRead, CHANGE); //interrupt digital pin 2
  attachInterrupt(1, GasRead, CHANGE);   //interrupt digital pin 3
  
  Serial.begin(9600);			// voor debugging
  Serial.println(F("Hello, CC3000!\n")); 
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

 
  /* Initialise the module */
  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while(1);
  }

  /* Optional: Update the Mac Address to a known value */
/*
  uint8_t macAddress[6] = { 0x08, 0x00, 0x28, 0x01, 0x79, 0xB7 };
   if (!cc3000.setMacAddress(macAddress))
   {
     Serial.println(F("Failed trying to update the MAC address"));
     while(1);
   }
*/
  
  
  /* Delete any old connection data on the module */
  Serial.println(F("\nDeleting old connection profiles"));
  if (!cc3000.deleteProfiles()) {
    Serial.println(F("Failed!"));
    while(1);
  }

  /* Attempt to connect to an access point */
  char *ssid = WLAN_SSID;             /* Max 32 chars */
  Serial.print(F("\nAttempting to connect to ")); Serial.println(ssid);
  
  /* NOTE: Secure connections are not available in 'Tiny' mode! */
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  
 
 
 // cc3300.begin(mac,ip);
  Udp.begin(localPort);
  now();
  Serial.println("Starting to sync the time via NTP.");
  setSyncProvider(getNtpTime);
  while(timeStatus()== timeNotSet){
    Serial.print(".");
    digitalWrite(6, HIGH); //check of de timesync werkt
    delay(1000);
    digitalWrite(6, LOW);
    delay(1000);
  }; // wachten op timesync door sync provider
  setTime(hour(),minute(),second(),day(),month(),year()); // Tijd instellen op de Arduino
  
  digitalWrite(6, HIGH); //KWh meter
  digitalWrite(5, HIGH); //Gas meter
  digitalWrite(4, HIGH); //Water meter
  delay(3000);
  digitalWrite(6, LOW); //KWh meter
  digitalWrite(5, LOW); //Gas meter
  digitalWrite(4, LOW); //Water meter
  
  Serial.println("EnergyLogger Programma gestart...");
  Serial.println(Ethernet.localIP());
  tijdNu = getTimeString();
  Serial.println(tijdNu);
  delay(1000);
}

//Programma
void loop() 
{
 ElecRead();

 tijdNu = getTimeString();
 
 if ( tijdNu == tijdLog )  // werkt voor loggen
  {
   Serial.print("Loggen  >  ");
   Serial.println(tijdNu);

/*
   if(DataSend())
    {
     GasIntWaarde=0;
     WaterIntWaarde=0;
     ElecKwhWaarde=0;
     delay(1000);
    }
  */  
   setSyncProvider(getNtpTime);
  }
}  

// Uitlezen Watersensor
void WaterRead()
{
  digitalWrite(4, LOW);
  if(InteruptTellerWater==1)
    {
     WaterIntWaarde++;
     InteruptTellerWater=0;
     digitalWrite(4, HIGH);
     if(WaterLtrWaarde+WaterIntWaarde==1000) //100 spiegelpulsen = 100ltr = 1M3 water
      {
       WaterM3Waarde++;
       WaterLtrWaarde=0;
      } 
    }
   else InteruptTellerWater++;
  if(WaterIntWaarde==99999999) WaterIntWaarde=0; //Watermeter Max. waarde bereikt
}

// Uitlezen Gassensor  
void GasRead()
{
 digitalWrite(5, LOW);
 if(InteruptTellerGas==1)
  {
   GasIntWaarde++;
   InteruptTellerGas=0;
   digitalWrite(5, HIGH);
   if(GasLtrWaarde+GasIntWaarde==1000) //100 spiegelpulsen is 1M3 gas
    {
     GasM3Waarde++;
     GasLtrWaarde=0;
    } 
  }
 else InteruptTellerGas++;
 if(GasIntWaarde==9999999) GasIntWaarde=0; //Gasmeter Max. waarde bereikt
}

// Uitlezen KWh sensor
void ElecRead()
{
 ElecSensorWaarde=analogRead(0);
 if(ElecSensorWaarde>850&&ElecSensorWaarde<950)
  {
   digitalWrite(6, HIGH); 
   if(zwartwaarde==1)
    {
     ElecIntWaarde++;  //bij "zien" van zwarte streepje
     zwartwaarde=0;
     return;
    } 
  } 
 else
  {
   digitalWrite(6, LOW);
   zwartwaarde=1;
   return;
  } 
 if(ElecIntWaarde==600) //600 omwentelingen = 1kWh
  {
   ElecKwhWaarde++;
   ElecIntWaarde=0;
   return;
  }
 if(ElecKwhWaarde==99999) ElecKwhWaarde=0; //KWHmeter Max. waarde bereikt
 return;
}


/*
// Verstuur data naar MySQL database
int DataSend()
{
  //cc3000 cc3000 = server.available()
  if (cc3000.connect(server,80))  // Verbinding mer PHP-server opbouwen
  {
    Serial.print("Verbinden...zend sensordata...");
    //cc3000.println("GET http://192.168.2.10/phpMyAdmin/mysqlvul.php?GAS=444&WATER=555&ELEC=666");
    //Naar MySQL database op webserver          
    cc3000.print("GET http://192.168.178.20/phpMyAdmin/mysqlvul.php");
    cc3000.print("?GAS=");
    cc3000.print(GasIntWaarde/100);
    cc3000.print("&WATER=");
    cc3000.print(WaterIntWaarde/10);
    cc3000.print("&ELEC=");
    cc3000.println(ElecKwhWaarde);
    cc3000.println("HTTP/1.1\r\n");
    cc3000.print("Host: http://192.168.178.20");
    cc3000.println();
    Serial.println("gereed!");
        
    GasIntWaarde=0;
    WaterIntWaarde=0;
    ElecKwhWaarde=0;
    return(1);
  }
  else
  {
    Serial.println(" ***** VERBINDING NIET MOGELIJK *****");
  }    
 delay(1);
 cc3000.stop();
 cc3000.flush();
}
*/

//NTP externe code
unsigned long getNtpTime()
{
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() ) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    
    // now convert NTP time into Unix time
    // starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;  
    return epoch - TZ_OFFSET;
  }
  return 0; // return 0 if u`````  qqnable to get the time
}

// send an NTP request to the time server at the given address 
unsigned long sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp: 		   
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}

// Vertaal naar tijdwaarde
String getTimeString() {  
  // gives back hh:mm:ss  
  time_t t = now();  
  String s = "";  
  if (hour(t) <10) s = s + "0";  
  s = s + hour(t) + ":"; 
  if (minute(t) <10) s = s + "0";  
  s = s + minute(t) + ":";  
  if (second(t) <10) s = s + "0";  
  s = s + second(t);  
  return(s);  
}  

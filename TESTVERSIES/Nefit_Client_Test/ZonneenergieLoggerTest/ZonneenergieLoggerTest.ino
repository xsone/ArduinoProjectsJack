/*

  Versie info:
  -------------
  File name: ZonneEnergieLoggerTest
  Werking: leest de S0 uitgang van de Voltcraft KWh uit o.b.v. interrupt
  (c) Jack Cop 2012-2014
  versie 2.0
    
*/

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <Time.h>
#include <udp.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 }; // MAC-ADRES Ethershield zie onderkant

byte ip[]  = { 192, 168, 178, 30 };                // IP-Adres van ethershield meterkast
//byte ip[]  = { 192, 168, 178, 40 };                  // IP-Adres van ethershield
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };
byte server[] = { 192, 168, 178, 20 };                 // IP van de NAS-server
EthernetClient client;

//Server server(80);
unsigned int localPort = 8888;

//IPAddress timeServer(132, 163, 4, 101); // time-c.timefreq.bldrdoc.gov NTP server
//IPAddress timeServer(96, 44, 157, 90);
IPAddress timeServer(131, 211, 8, 244); // pool.ntp.org NTP server in Ziggo router

const long NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 
EthernetUDP Udp;

// Tijd instellingen
//time_t logInterval = 300UL;          // append to file every loginterval seconds  
//time_t lastLogTime = 0UL;           
//time_t syncInterval = 10UL;         // ntp sync every syncinterval seconds  
time_t lastSyncTime = 0UL;          
time_t t = 0UL; 
time_t t_old = 0UL; 

// NTP instellingen
const unsigned long UTCcorrection = 0; 
unsigned long secsSince1900 = 0UL;
const unsigned long seventyYears = 2208988800UL;   
unsigned long epoch = secsSince1900 - seventyYears;  


//char c;

// Sensor reading
volatile unsigned long int ZonKWhWaarde = 0;
volatile unsigned long int ZonIntWaarde = 0;
int InteruptTellerZon = 0;

String tijdNu;
String tijdLog = String("21:30:00");

// Initialisatie en starten
void setup()
{
  /*
  Ethernet.begin(mac);
  Udp.begin(localPort);
  ntpTimeSync();
  delay(1000);
  ntpTimeSync();
  */
  
  Serial.begin(9600);			// voor debugging
  Serial.println("ZonneEnergyLogger Programma gestart...");
  //Serial.println(Ethernet.localIP());
  
  pinMode(13, OUTPUT);  //LED indicator KWh meter
  digitalWrite(13, LOW); //KWh meter
  delay(2000);
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);

  pinMode(3, INPUT);    //sensorinput Gas INT1
  digitalWrite(3, LOW);    //sensorinput Gas INT1
  attachInterrupt(1, ZonRead, FALLING);   //interrupt digital pin 3
}


// Uitlezen Kwhsensor  
void ZonRead()
{
 digitalWrite(13, HIGH);
 Serial.print("ZonPulsTeller (1000 = 1KWh): ");
 Serial.println(ZonIntWaarde, DEC);
 delay(500);
 digitalWrite(13, LOW);
 ZonIntWaarde++;

 if(ZonIntWaarde==1000) //1000 S0-pulsen is 1Kwh Zonneenergie
  {
   ZonKWhWaarde++;
   Serial.print("ZonKWhTeller:  ");
   Serial.println(ZonKWhWaarde, DEC);
  } 
 if(ZonIntWaarde==999999) ZonIntWaarde=0; //KWhmeter Max. waarde bereikt
 digitalWrite(3, LOW);
}

//Programma
void loop() 
{
}
  
  /*
 ElecRead();
 t = now();
 tijdNu = getTimeString();
 //Serial.println(tijdNu);

 if ( tijdNu == tijdLog ) {    // werkt voor loggen
 //if (tijdNu[6] == '0') {    // werkt voor loggen
    ntpTimeSync();
    Serial.print("Loggen  >  ");
    Serial.println(tijdNu);

    if(DataSend())
    {
     GasIntWaarde=0;
     WaterIntWaarde=0;
     ElecKwhWaarde=0;
     delay(1000);
    } 
 }
*/


/*
// Verstuur data naar MySQL database
int DataSend()
{
  //Client client = server.available()
  if (client.connect(server,80))  // Verbinding mer PHP-server opbouwen
  {
    Serial.print("Verbinden...zend sensordata...");
    //client.println("GET http://192.168.2.10/phpMyAdmin/mysqlvul.php?GAS=444&WATER=555&ELEC=666");
    //Naar MySQL database op webserver          
    client.print("GET http://192.168.178.20/phpMyAdmin/mysqlvul.php");
    client.print("?GAS=");
    client.print(GasIntWaarde/100);
    client.print("&WATER=");
    client.print(WaterIntWaarde/10);
    client.print("&ELEC=");
    client.println(ElecKwhWaarde);
    client.println("HTTP/1.1\r\n");
    client.print("Host: http://192.168.178.20");
    client.println();
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
 client.stop();
 client.flush();
}
*/

//Sync met NTP server
void ntpTimeSync() {  
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  unsigned long epoch = secsSince1900 - seventyYears;  
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
    secsSince1900 = highWord << 16 | lowWord;  
    unsigned long hours = (epoch  % 86400L) / 3600;
   }
 time_t t = epoch + UTCcorrection;
 setTime(t);
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
  s = s + (hour(t)+1) + ":"; //Zomertijd dan +2; Wintertijd dan +1
  if (minute(t) <10) s = s + "0";  
  s = s + minute(t) + ":";  
  if (second(t) <10) s = s + "0";  
  s = s + second(t);  
  return(s);  
}  

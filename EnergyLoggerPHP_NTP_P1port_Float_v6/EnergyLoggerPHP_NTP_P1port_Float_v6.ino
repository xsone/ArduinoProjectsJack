
/*

  Versie info:
  -------------
  File name: EnergieLoggerMegaPHPtime_pde
  Werking: zend iedere 24 uur een waarde naar de database MySQL "Energielogger" o.b.v. Timeserver
  (c) Jack Cop 2012-2017
  versie 3.0
  
  10/2/2018 probleem met watersensor opgelost
  
  
  Codes DSMR voor de slimme meter Landys&Gyr E350 SMR5.0
  0-0:96.1.1.255   Identifier van meter
  1-0:1.8.1.255    Geleverde elektriciteit AAN klant Tarief T1 in 0,001 KWh
  1-0:2.8.1.255    Geleverde elektriciteit DOOR klant Tarief T1 in 0,001 KWh (Zonnepanelen)
  1-0:1.8.2.255    Geleverde elektriciteit AAN klant Tarief T2 in 0,001 KWh
  1-0:2.8.2.255    Geleverde elektriciteit DOOR klant Tarief T2 in 0,001 KWh (Zonnepanelen)
  1-0:1.7.0.255    Actuele elektriciteitsproductie (+P) AAN klant met 1 Watt nauwkeurig (resolutie)
  1-0:2.7.0.255    Actuele elektriciteitsproductie (-P) DOOR klant met 1 Watt nauwkeurig (resolutie)
  0-0:96.7.21.255  Aantal storingen in een van de fasen
  0-0:96.7.9.255   Aantal lange storingen in een van de fasen
  1-0:99:97.0.255  Event logger lange storingen
  1-0:32.32.0.255  Aantal Voltage pieken in L1
  1-0:32.36.0.255  Aantal Voltage dalen in L1
  0-0:96.13.0.255  Text bericht max. 1024 chars
  
  
  Voorbeeld bericht via P1 poort
/XMX5LGBBFG10
 
1-3:0.2.8(42)
0-0:1.0.0(170108161107W)
0-0:96.1.1(4530303331303033303031363939353135)
1-0:1.8.1(002074.842*kWh)
1-0:1.8.2(000881.383*kWh)
1-0:2.8.1(000010.981*kWh)
1-0:2.8.2(000028.031*kWh)
0-0:96.14.0(0001)
1-0:1.7.0(00.494*kW)
1-0:2.7.0(00.000*kW)
0-0:96.7.21(00004)
0-0:96.7.9(00003)
1-0:99.97.0(3)(0-0:96.7.19)(160315184219W)(0000000310*s)(160207164837W)(0000000981*s)(151118085623W)(0000502496*s)
1-0:32.32.0(00000)
1-0:32.36.0(00000)
0-0:96.13.1()
0-0:96.13.0()
1-0:31.7.0(003*A)
1-0:21.7.0(00.494*kW)
1-0:22.7.0(00.000*kW)
0-1:24.1.0(003)
0-1:96.1.0(4730303139333430323231313938343135)
0-1:24.2.1(170108160000W)(01234.000*m3)
!D3B0
*/

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <Time.h>

#define TZ_OFFSET -3600L  // time zone offset - set deze offset in seconds to.v. je local time //Zomertijd dan -2hrs (-7200L sec); Wintertijd dan -1hrs (-3600L sec)

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 }; // MAC-ADRES Ethershield zie onderkant
byte ip[]  = { 192, 168, 178, 30 };                // IP-Adres van ethershield meterkast
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };
byte server[] = { 192, 168, 178, 20 };                 // IP van de NAS-server

EthernetClient client;
EthernetUDP Udp;

//Server server(80);
unsigned int localPort = 8888;
char timeServer[] = "0.nl.pool.ntp.org";

// NTP server instellingen
const long NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// Tijd instellingen
time_t t = 0UL; 
String tijdNu;
String tijdLog = "21:00:00";

// Sensor Pins
const int waterLedPin =  4;
const int waterInputPin = 2; //interrupt 0

//Inlezen van Slimme Meterdata naar String
String inputString;

// Waarden Slimme Meter P1-poort
float ElecLTverbruikt = 0; //Meter reading Electrics - consumption low tariff verbruikt
float ElecLTgeleverd = 0; //Meter reading Electrics - consumption low tariff opgewekt
float GasVerbruikt = 0;   //Meter reading Gas

float ElecLTverbruiktOld = 0; //Meter reading Electrics - consumption low tariff verbruikt
float ElecLTgeleverdOld = 0; //Meter reading Electrics - consumption low tariff opgewekt
float GasVerbruiktOld = 0;   //Meter reading Gas

unsigned long int WaterIntWaarde=0;
unsigned long int WaterLtrWaarde=0;
unsigned long int WaterM3Waarde=0;
boolean InteruptTellerWater = false;

//Test o.b.v. tijdsinterval definieve versie met NTP-time via ethernet
long lastTime = 0;        // will store last time 
//long interval = 1800000;           // interval at which to blink (milliseconds), halfuur
long interval = 3600000;           // interval at which to blink (milliseconds), halfuur
//long interval = 600000;           // interval at which to blink (milliseconds), 10 minuten
//long interval = 3600 * 1 * 1000; // 1 uren

// Initialisatie en starten
void setup()
{
  //Watersensor output LED indicator
  pinMode(waterLedPin, OUTPUT);  //LED indicator Water meter
  digitalWrite(waterLedPin, LOW); //Water meter
  
  //Watersensor input via interrupt 
  pinMode(waterInputPin, INPUT);  //sensorinput Water INT0
  attachInterrupt(0, WaterRead, CHANGE); //interrupt digital pin 2
  
  //Initialiseer Seriele poort met baudreate slimme meter (115200) en Ethernetshield
  Serial.begin(115200);			// voor debugging
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

/* Begin verwijderen in def versie met NTP */
/*
  Serial.println("Starting to sync the time via NTP.");
  sendNTPpacket(timeServer);
  delay(1000);
  setSyncProvider(getNtpTime);
  
  //getNtpTime();
  delay(1000);
  now();
  while(timeStatus()== timeNotSet){
    getNtpTime();
    Serial.print(".");
    digitalWrite(waterLedPin, LOW); //check of de timesync werkt
    delay(1000);
    digitalWrite(waterLedPin, HIGH);
    delay(1000);
  }; // wachten op timesync door sync provider
  setTime(hour(),minute(),second(),day(),month(),year()); // Tijd instellen op de Arduino
  digitalWrite(waterLedPin, LOW); //Water meter Led gebruikt als testled
  delay(3000);
  digitalWrite(waterLedPin, HIGH); //Water meter
/*Einde verwijderen voor def versie met NTP*/  
  Serial.println("EnergyLogger Programma gestart [v6.2]...");
  Serial.println(Ethernet.localIP());
  //tijdNu = getTimeString();
  //Serial.println(tijdNu);
  delay(1000);
  DataSend(); //Bij initialisatie teste of er iets naar de server wordt verstuurd.
}

//Programma
void loop() 
{
 //tijdNu = getTimeString();
 decodeTelegram(); //continue uitlezen slimme meter
 
// if ( tijdNu == tijdLog )  // werkt voor loggen
//  {
 
 if(millis() - lastTime > interval) 
  {
   lastTime = millis();   
  
   Serial.print("Loggen  >  ");
   //Serial.println(tijdNu);
   if(DataSend())
    {
     ElecLTverbruiktOld = ElecLTverbruikt; //Meter reading Electrics - consumption low tariff verbruikt
     ElecLTgeleverdOld = ElecLTgeleverd; //Meter reading Electrics - consumption low tariff opgewekt
     GasVerbruiktOld = GasVerbruikt;   //Meter reading Gas
      
     ElecLTverbruikt = 0; //Meter reading Electrics - consumption low tariff verbruikt
     ElecLTgeleverd = 0; //Meter reading Electrics - consumption low tariff opgewekt
     GasVerbruikt = 0;   //Meter reading Gas
     
     WaterIntWaarde=0;
     WaterLtrWaarde=0;
     WaterM3Waarde=0;
     //setSyncProvider(getNtpTime); // verplaatst omdat logger maar 1x schrijft
     
     ElecLTverbruikt = 50.33; //Meter reading Electrics - consumption low tariff verbruikt
     ElecLTgeleverd = 60.23; //Meter reading Electrics - consumption low tariff opgewekt
     GasVerbruikt = 70.14;   //Meter reading Gas

     ElecLTverbruiktOld = 10.11; //Meter reading Electrics - consumption low tariff verbruikt
     ElecLTgeleverdOld = 11.12; //Meter reading Electrics - consumption low tariff opgewekt
     GasVerbruiktOld = 13.33;   //Meter reading Gas
    
     //Serial.println(GasVerbruikt - GasVerbruiktOld,2);
     //Serial.println(float(WaterIntWaarde/2),2); //1 spiegel rondgang levert 2 pulsen maar is 1 litr water
     //Serial.println(ElecLTverbruikt - ElecLTverbruiktOld, 2);
     //Serial.println(ElecLTgeleverd - ElecLTgeleverdOld, 2);
     
     
     delay(1000);
    }
   //setSyncProvider(getNtpTime); //werkt niet 
  }
  Ethernet.maintain();
}  

// Uitlezen Watersensor
void WaterRead()
{
  digitalWrite(waterLedPin, LOW);
  if(InteruptTellerWater)
    {
     WaterIntWaarde++;
     InteruptTellerWater = false;
     digitalWrite(waterLedPin, HIGH);
     if(WaterLtrWaarde+WaterIntWaarde==2000) //1000 spiegelpulsen = 1000ltr = 1M3 water 
      {
       WaterM3Waarde++;
       WaterLtrWaarde=0;
      } 
    }
   else InteruptTellerWater = true;
  if(WaterIntWaarde==99999999) WaterIntWaarde=0; //Watermeter Max. waarde bereikt
  //Serial.println(WaterIntWaarde, DEC);
}

// Uitlezen Slimme Meter via P1-poort en bericht decoderen
void decodeTelegram()
 {
  if(Serial.available()>0) inputString = Serial.readStringUntil('\n');
  //Serial.println("P1 telegram");
  //Serial.print(inputString);

  //1-0:1.8.1.255    Geleverde elektriciteit AAN klant Tarief T1 (LT) in 0,001 KWh
  //1-0:2.8.1.255    Geleverde elektriciteit DOOR klant Tarief T1 (LT) in 0,001 KWh (Zonnepanelen)
  //1-0:1.8.2.255    Geleverde elektriciteit AAN klant Tarief T2 (HT) in 0,001 KWh
  //1-0:2.8.2.255    Geleverde elektriciteit DOOR klant Tarief T2 (HT) in 0,001 KWh (Zonnepanelen)
  //1-0:1.7.0.255    Actuele elektriciteitsproductie (+P) AAN klant met 1 Watt nauwkeurig (resolutie)
  //1-0:2.7.0.255    Actuele elektriciteitsproductie (-P) DOOR klant met 1 Watt nauwkeurig (resolutie)
  //0-1:24.2.1.255   Gas verbruikt in M3

  //Elektriciteit Laag tarief
  if(inputString.substring(0,9) == "1-0:1.8.1")
   {
   // Serial.print("ElecLTverbruikt: ");
   // Serial.println(inputString.substring(10,16));
    ElecLTverbruikt = inputString.substring(10,16).toFloat(); //Meter reading Electrics - consumption low tariff verbruikt
   // Serial.println(ElecLTverbruikt,2);
   } 
  if(inputString.substring(0,9) == "1-0:2.8.1")
   {
    //Serial.print("ElecLTgeleverd: ");
    //Serial.println(inputString.substring(10,16));
    ElecLTgeleverd = inputString.substring(10,16).toFloat();
    //Serial.println(ElecLTgeleverd,2);
   }       

  //Gas in M3 
  if(inputString.substring(0,10) == "0-1:24.2.1")
   {
    //Serial.print("GasVerbruikt: ");
    //Serial.println(inputString.substring(26,31));
    GasVerbruikt = inputString.substring(26,31).toFloat();
    //Serial.println(GasVerbruikt, 2);  
   }
  inputString = ""; // String weer leegmaken voor volgende reading
  Serial.flush();
} //Einde 'decodeTelegram()' functie


// Verstuur data naar MySQL database
int DataSend()
{
  //Client client = server.available()
  if (client.connect(server,80))  // Verbinding mer PHP-server opbouwen
  {
    Serial.print("Verbinden...zend sensordata...");
    //client.println("GET http://192.168.178.20/phpMyAdmin/mysqlvul.php?GAS=444&WATER=555&ELEC=666&ElecLTverbruikt=13&ElecLTgeleverd=14&ElecHTverbruikt=15&ElecHTgeleverd=16");
    //Naar MySQL database op webserver          
    client.print("GET http://192.168.178.20/phpMyAdmin/energiemeter.php");
    client.print("?GAS=");
    client.print(GasVerbruikt - GasVerbruiktOld);
    client.print("&WATER=");
    client.print(float(WaterIntWaarde/2)); //1 spiegel rondgang levert 2 pulsen maar is 1 litr water
    client.print("&ElecLTverbruikt=");
    client.print(ElecLTverbruikt - ElecLTverbruiktOld);
    client.print("&ElecLTgeleverd=");
    client.print(ElecLTgeleverd - ElecLTgeleverdOld);
    client.println();
    client.println("HTTP/1.1\r\n");
    client.print("Host: http://192.168.178.20");
    client.println();
    while (client.available() )
    {
      char c = client.read();
    }
    client.println();
    //delay(1);
    //WaterIntWaarde = 0; 
    //WaterIntWaarde += 1; 
    //Serial.println("gereed!");
    client.stop();
    client.flush();
    return(1);
  }
  else
  {
    Serial.println(" ***** VERBINDING NIET MOGELIJK *****");
    client.stop();
  }    
 //delay(1);
 //client.stop();
 //client.flush();
}

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
//unsigned long sendNTPpacket(IPAddress& address)
unsigned long sendNTPpacket(char* address)
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

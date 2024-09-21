
/*

  Versie info:
  -------------
  File name: EnergieLoggerMegaPHPtime_pde
  Werking: zend iedere 24 uur een waarde naar de database MySQL "Energielogger" o.b.v. Timeserver
  (c) Jack Cop 2012-2021
  versie 3.0
  
  10/02/2018 probleem met watersensor opgelost
  09/01/2021 actueel enregieverbruik/geleverd naar Buffershield waarmee buffervat wordt aan of uitgezet
  
  
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
#include <SPI.h>

byte macEnergielogger[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 }; // MAC-ADRES Energielogger in meterkast zie onderkant
byte ipEnergielogger[]  = { 192, 168, 178, 30 }; // IP-Adres van arduino Energielogger in meterkast
byte gateway[] = { 192, 168, 178, 1 }; // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };
byte serverNAS[] = { 192, 168, 178, 20 };            // IP van de NAS-server

EthernetClient client; //maak een client van de Energielogger in de meterkast.
IPAddress serverAddress(192, 168, 178, 45); //Buffervatshield werkt als server met dit adres
EthernetClient TCPclient; //via deze cleint genaamd TCPcleint wordt data naar het Buffervatshield gestuurd en verwerkt
const int serverPort = 80;

// Sensor Pins
const int waterLedPin =  4;
const int waterInputPin = 2; //interrupt 0

//Inlezen van Slimme Meterdata naar String
String inputString;

//Startwaarden voor Buffervat
String ElecActueelVerbruiktStr = "00.111";
float ElecActueelVerbruikt = 11.1; //Meter laag tarief Electriciteit verbruikt
String ElecActueelGeleverdStr = "00.222";  //Meter laag tarief Electriciteit opgewekt
float ElecActueelGeleverd = 22.2;  //Meter laag tarief Electriciteit opgewekt

// Waarden Slimme Meter P1-poort
float ElecLTverbruikt = 0; //Meter laag tarief Electriciteit verbruikt
float ElecLTgeleverd = 0;  //Meter laag tarief Electriciteit opgewekt
float GasVerbruikt = 0;    //Meter Gas verbruikt

//referentiewaarden om verschil te maken.
float ElecLTverbruiktOld = 0; //Meter reading Electrics - consumption low tariff verbruikt
float ElecLTgeleverdOld = 0; //Meter reading Electrics - consumption low tariff opgewekt
float GasVerbruiktOld = 0;   //Meter reading Gas

//Referentiewaarden Water-sensor
unsigned long int WaterIntWaarde=0;
unsigned long int WaterLtrWaarde=0;
unsigned long int WaterM3Waarde=0;
boolean InteruptTellerWater = false;

//Test o.b.v. tijdsinterval definieve versie met NTP-time via ethernet
long lastTimeBuffervat = 0;            // laatste tijd waarde m.b.t. ververs-interval Buffervat
long lastTimeEnergielogger = 0;         // laatste tijd waarde m.b.t. ververs-interval Energielogger
//long intervalEnergielogger = 1800000;    // intervalEnergielogger waarop energieverbuik wordt gelogged (ms.), halfuur
//long intervalEnergielogger = 3600000;      // intervalEnergielogger waarop energieverbuik wordt gelogged (ms.), 1 uur
//long intervalEnergielogger = 600000;     // intervalEnergielogger, 10 minuten
long intervalEnergielogger = 60000;     // intervalEnergielogger, 1 minuut
long intervalBuffervat = 10000; // per 10 sec. uitlezen Energie status om buffervat aan te sturen 

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
 
  //Initialiseer Ethernet connectie 
  Ethernet.begin(macEnergielogger,ipEnergielogger);

  // connect to TCP server (Arduino #2) van Buffervat server
  if (TCPclient.connect(serverAddress, serverPort))
    Serial.println("Connected to TCP server Buffervat");
  else
    Serial.println("Failed to connect to TCP server");

  //serverEnergielogger.begin();
  Serial.println("EnergieLogger Programma Buffervat gestart [v3.0], Client Energielogger op: ");
  Serial.println(Ethernet.localIP());
  delay(1000);

  dataSend(); //Bij initialisatie testen of er iets naar de server wordt verstuurd via het PHP script energielogger.php
  dataSentBuffervat(); //test of meter wordt uitgelezen met actuele waarden
}

//Programma
void loop() 
{
 decodeTelegram(); //continue uitlezen slimme meter

 //uitlezen energie status per 10 sec. op buffervat tijdig aan te sturen.
 if(millis() - lastTimeBuffervat > intervalBuffervat) 
  {
   lastTimeBuffervat = millis();
   
   if (TCPclient.connect(serverAddress, serverPort))
    Serial.println("Connected to TCP server");
  else
    Serial.println("Failed to connect to TCP server");
  dataSentBuffervat();
 }
 
 if(millis() - lastTimeEnergielogger > intervalEnergielogger) 
  {
   lastTimeEnergielogger = millis();   
   Serial.print("Loggen  >  ");
   
   if(dataSend())
    {
     Serial.print("ElecLTverbruikt: ");
     Serial.println(ElecLTverbruikt, 2);
     Serial.print("ElecLTverbruiktOld: ");
     Serial.println(ElecLTverbruiktOld, 2);
     
     ElecLTverbruiktOld = ElecLTverbruikt; //Meter reading Electrics - consumption low tariff verbruikt
     ElecLTgeleverdOld = ElecLTgeleverd; //Meter reading Electrics - consumption low tariff opgewekt
     GasVerbruiktOld = GasVerbruikt;   //Meter reading Gas
     
     //Serial.println(ElecLTverbruikt - ElecLTverbruiktOld, 2);
     //Serial.println(ElecLTgeleverd - ElecLTgeleverdOld, 2);
     //Serial.println(GasVerbruikt - GasVerbruiktOld,2);
     //Serial.println(float(WaterIntWaarde/2),2); //1 spiegel rondgang levert 2 pulsen maar is 1 litr water
      
     //ElecLTverbruikt = 0; //Meter reading Electrics - consumption low tariff verbruikt
     //ElecLTgeleverd = 0; //Meter reading Electrics - consumption low tariff opgewekt
     //GasVerbruikt = 0;   //Meter reading Gas
     
     WaterIntWaarde=0;
     WaterLtrWaarde=0;
     WaterM3Waarde=0;

     //Testwaarden
     //ElecLTverbruikt = 50.33; //Meter reading Electrics - consumption low tariff verbruikt
     //ElecLTgeleverd = 60.23; //Meter reading Electrics - consumption low tariff opgewekt
     //GasVerbruikt = 70.14;   //Meter reading Gas

     //ElecLTverbruiktOld = 10.11; //Meter reading Electrics - consumption low tariff verbruikt
     //ElecLTgeleverdOld = 11.12; //Meter reading Electrics - consumption low tariff opgewekt
     //GasVerbruiktOld = 13.33;   //Meter reading Gas
     
     delay(1000);
    }
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
  if(Serial.available() > 0) inputString = Serial.readStringUntil('\n');
  //Serial.println("P1 telegram");
  //Serial.print(inputString);

/*
  //1-0:1.8.1.255    Geleverde elektriciteit AAN klant Tarief T1 (LT) in 0,001 KWh
  //1-0:2.8.1.255    Geleverde elektriciteit DOOR klant Tarief T1 (LT) in 0,001 KWh (Zonnepanelen)
  //1-0:1.8.2.255    Geleverde elektriciteit AAN klant Tarief T2 (HT) in 0,001 KWh
  //1-0:2.8.2.255    Geleverde elektriciteit DOOR klant Tarief T2 (HT) in 0,001 KWh (Zonnepanelen)
  //1-0:1.7.0.255    Actuele elektriciteitsproductie (+P) AAN klant met 1 Watt nauwkeurig (resolutie)
  //1-0:2.7.0.255    Actuele elektriciteitsproductie (-P) DOOR klant met 1 Watt nauwkeurig (resolutie)
  //0-1:24.2.1.255   Gas verbruikt in M3
*/

/*
  //Elektriciteit Laag tarief verbuikt
  // substring nummer 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
  // substring(0,9)   1 - 0 : 1 . 8 . 1   5  0  .  3  3
  // substring(0,9)   1 - 0 : 1 . 7 . 0   0  .  3  5
  // act.verbruikt    1 - 0 : 1 . 7 . 0   0  0  .  4  9  4                 
  // act. gelevert    1 - 0 : 2 . 7 . 0   0  0  .  0  0  0
*/
  
  if(inputString.substring(0,9) == "1-0:1.8.1")
   {
    //Serial.print("ElecLTverbruikt: ");
   // Serial.println(inputString.substring(10,16));
    ElecLTverbruikt = inputString.substring(10,16).toFloat(); //Meter reading Electrics - consumption low tariff verbruikt
    //Serial.println(ElecLTverbruikt,2);
   } 
  //Electriciteit Laag tarief geleverd
  if(inputString.substring(0,9) == "1-0:2.8.1")
   {
    //Serial.print("ElecLTgeleverd: ");
    //Serial.println(inputString.substring(10,16));
    ElecLTgeleverd = inputString.substring(10,16).toFloat();
    //Serial.println(ElecLTgeleverd,2);
   }       

  /*
    1-0:1.7.0.255    Actuele elektriciteitsproductie (+P) AAN klant met 1 Watt nauwkeurig (resolutie)
    1-0:2.7.0.255    Actuele elektriciteitsproductie (-P) DOOR klant met 1 Watt nauwkeurig (resolutie)
  */  

 
  //Gas in M3 
  if(inputString.substring(0,10) == "0-1:24.2.1")
   {
    //Serial.print("GasVerbruikt: ");
    //Serial.println(inputString.substring(26,31));
    GasVerbruikt = inputString.substring(26,31).toFloat();
    //Serial.println(GasVerbruikt, 2);  
   }

  //Elektriciteit Actueel verbuikt
  if(inputString.substring(0,9) == "1-0:1.7.0")
   {
   // Serial.print("ElecVerbruikt: ");
   // Serial.println(inputString.substring(10,16));
    ElecActueelVerbruiktStr = inputString.substring(10,16);
    ElecActueelVerbruikt = ElecActueelVerbruiktStr.toFloat(); //Meter reading Electrics - consumption low tariff verbruikt
   // Serial.println(ElecVerbruikt,2);
   } 
  //Electriciteit Actueel gelevert
  if(inputString.substring(0,9) == "1-0:2.7.0")
   {
    //Serial.print("ElecActueelGeleverd: ");
    //Serial.println(inputString.substring(10,16));
    ElecActueelGeleverdStr = inputString.substring(10,16);
    ElecActueelGeleverd = ElecActueelGeleverdStr.toFloat();
    //Serial.println(ElecActueelGeleverd,2);
   }       
   
  inputString = ""; // String weer leegmaken voor volgende reading
  Serial.flush();
} //Einde 'decodeTelegram()' functie


// Verstuur data naar MySQL database op de NAS
int dataSend()
{
  if (client.connect(serverNAS,80))  // Verbinding mer PHP-server opbouwen
   {
    Serial.print("Verbinding met server (NAS)oke...verzenden sensordata...");
    //client.println("GET http://192.168.178.20/phpMyAdmin/mysqlvul.php?GAS=444&WATER=555&ELEC=666&ElecLTverbruikt=13&ElecLTgeleverd=14"); //test voor PHP programma op NAS-server
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
    client.stop();
    client.flush();
    Serial.println("gereed data naar MySQL database verzonden!");
    return(1);
  }
  else
  {
    Serial.println(" ***** VERBINDING met server (NAS) NIET MOGELIJK *****");
    client.stop();
    client.flush();
    return(0);
  }    
}

// Verstuur data naar Buffervat-server
void dataSentBuffervat()
{
  if (TCPclient.connect(serverAddress,serverPort))  // Verbinding mer Buffervat-server opbouwen
   {
    Serial.println("new client van server Energielogger");
    Serial.print("Elec actueel verbruikt: ");
    Serial.println(ElecActueelVerbruiktStr);
    Serial.print("Elec actueel geleverd: ");
    Serial.println(ElecActueelGeleverdStr);
    TCPclient.print("#");
    TCPclient.print(ElecActueelVerbruiktStr);
    TCPclient.print(";");
    TCPclient.print(ElecActueelGeleverdStr);
    TCPclient.print(";");
    TCPclient.println();
    Serial.println("gereed!");
   }
  else
  {
    Serial.println(" ***** VERBINDING met Buffervat-server niet MOGELIJK *****");
   }  
 delay(1);
 TCPclient.stop();
 TCPclient.flush();
 Serial.println("client disconnected");
}

/*

  Versie info "Energylogger"
  -------------
  Zend iedere 12uur een sensorwaarde incl. timestamp naar de database MySQL "Energielogger"
  Sensor: GAS, ELEC, WATER
    
*/

#include <Ethernet.h>
//#include <SPI.h>

// Timer settings
const long oneSecond = 1000;
const long oneMinute = oneSecond * 60;
const long oneHour = oneMinute * 60;
const long oneKwart = oneMinute * 15;
const long oneDay = oneHour * 24;

unsigned long lastConnectionTime = 0;            // tijdstip waarop voor het laaste connectie is geweest met de server, in millisec.
boolean lastConnected = false;                   // de laatst bekende status van de connectie na de main loop
const unsigned long postingInterval = oneHour/2; // updateinterval schrijf actie naar MySQL database

// Network settings
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 }; // MAC-ADRES Ethershield zie onderkant
byte ip[]  = { 192, 168, 178, 40 };                  // statisch ingestelde IP-Adres van ethershield
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router IP (Ziggo router)
byte subnet[]  = { 255, 255, 255, 0 };
byte server[] = { 192, 168, 178, 20 };               // IP-adres van de MySQL  des Servers eingeben
Client client(server, 80);
char c;

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
volatile long int timer=0;

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

// Uitlezen KWh meter
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

// Initialisatie start programma
void setup()
{
  Serial.begin(9600);			// voor debugging
  Serial.println("Programma gestart...");
  
  pinMode(6, OUTPUT);  
  pinMode(5, OUTPUT);  
  pinMode(4, OUTPUT);  
  digitalWrite(6, LOW); //KWh meter
  digitalWrite(5, LOW); //Gas meter
  digitalWrite(4, LOW); //Water meter
    
  pinMode(2, INPUT);    //sensorinput Water INT0
  pinMode(3, INPUT);    //sensorinput Gas INT1
  Ethernet.begin(mac, ip, gateway, subnet);
  DataSend();
  
  attachInterrupt(0, WaterRead, CHANGE); //interrupt digital pin 2
  attachInterrupt(1, GasRead, CHANGE);   //interrupt digital pin 3
  //delay(5000);				// wacht tot LAN gestart is
}

// Loop t.b.v. uitlezen meters
void loop()
{
 ElecRead();

 // send it out the serial port voor debugging
 if (client.available())
  {
   char c = client.read();
   Serial.print(c);
  }
 
 // if there's no net connection, but there was one last time through the loop, then stop the client:
 if (!client.connected() && lastConnected)
  {
   Serial.println();
   Serial.println("disconnecting.");
   client.stop();
  }

  // if you're not connected, and ten seconds have passed since your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) 
   {
    DataSend();
   }
  // store the state of the connection for next time through the loop:
  lastConnected = client.connected();
// client.flush();
}

// Verzamelen meetgegevens sensors
void DataSend()
{
  if (client.connect())  // Verbinding mer PHP-server opbouwen
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
    //return(1);
  }
  else
  {
   Serial.println(" ***** VERBINDING NIET MOGELIJK *****");
  }    
 lastConnectionTime = millis();
}


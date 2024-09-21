/*

  Versie info: EnergyLogger_Timer_v3 dd. 15/03/2013
  -------------
  Zend iedere 24 uur een waarde naar de database MySQL "Energielogger" [mysqlvul.php]
   
*/

#include <Ethernet.h>
#include <SPI.h>
#include <Time.h>

#include "WProgram.h"
void WaterRead();
void GasRead();
void ElecRead();
void setup();
void loop();
int DataSend();
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x06, 0xF0 }; // MAC-ADRES Ethershield (in meterkast!!) zie onderkant
byte ip[]  = { 192, 168, 178, 30 };                  // IP-Adres van ethershield
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };
byte server[] = { 192, 168, 178, 20 };               // IP des Servers eingeben
Client client(server, 80);
int old_value=0;                                     // Detectie van verandering sec, min, uur, day

// Sensor reading variabelen
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

// Uitlezen Kwh meter
void ElecRead()
{
 ElecSensorWaarde=analogRead(0);
 Serial.println(ElecSensorWaarde);
 
 if(ElecSensorWaarde>960&&ElecSensorWaarde<980)
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

// Initialisatie
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
  weekday();
  
  attachInterrupt(0, WaterRead, CHANGE); //interrupt digital pin 2
  attachInterrupt(1, GasRead, CHANGE);   //interrupt digital pin 3
}

void loop()
{
 ElecRead();
 
 //if(!client.connected() && hour()!=old_value)
 if(weekday()!=old_value)
  {
   DataSend();
   Serial.println(weekday());
   old_value=weekday();
  } 
}

// Post in MySQL database
int DataSend()
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
  }
 else 
  {
   Serial.println(" ***** VERBINDING NIET MOGELIJK *****");
  } 
 client.stop();
}



int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


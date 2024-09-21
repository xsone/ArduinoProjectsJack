/*

  Versie info:
  -------------
  zend iedere 12uur een waarde naar de database MySQL "Energielogger"
    
*/

#include <Ethernet.h>
#include <EthernetDNS.h>
#include <SPI.h>
#include <Twitter.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 }; // MAC-ADRES Ethershield zie onderkant
byte ip[]  = { 192, 168, 2, 14 };                    // IP-Adres van ethershield
byte gateway[] = { 192, 168, 2, 254 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };
byte server[] = { 192, 168, 2, 10 };                 // IP des Servers eingeben
Client client(server, 80);
char c;

Twitter twitter("214214910-wgjnY7vjwZEnJ4cl8dqoPahX90bVLmY5MX9yiNCs");


// Sensor reading
volatile long int GasIntWaarde=0;
volatile long int GasLtrWaarde=0;
volatile long int GasM3Waarde=0;
volatile long int WaterIntWaarde=0;
volatile long int WaterLtrWaarde=0;
volatile long int WaterM3Waarde=0;
volatile long int ElecKwhWaarde = 0;

char GasCharWaarde[20];
char ElecCharWaarde[20];
char WaterCharWaarde[20];
char Tweet[100];


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
     if(WaterLtrWaarde+WaterIntWaarde==100) //100 spiegelpulsen = 100ltr = 1M3 water
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
   if(GasLtrWaarde+GasIntWaarde==100) //100 spiegelpulsen is 1M3 gas
    {
     GasM3Waarde++;
     GasLtrWaarde=0;
    } 
  }
 else InteruptTellerGas++;
 if(GasIntWaarde==9999999) GasIntWaarde=0; //Gasmeter Max. waarde bereikt
}

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
    } 
  } 
 else
  {
   digitalWrite(6, LOW);
   zwartwaarde=1;
  } 
 if(ElecIntWaarde==600) //600 omwentelingen = 1kWh
  {
   ElecKwhWaarde++;
   ElecIntWaarde=0;
  }
 if(ElecKwhWaarde==99999) ElecKwhWaarde=0; //KWHmeter Max. waarde bereikt
}


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
  
  attachInterrupt(0, WaterRead, CHANGE); //interrupt digital pin 2
  attachInterrupt(1, GasRead, CHANGE);   //interrupt digital pin 3
  delay(5000);				// wacht tot LAN gestart is
}

void loop()
{
 ElecRead();
 if(timer==30000) //900000 15 min. timer zetten op 1x per 12 uur (43200000)
  {
   Serial.println(timer);
   DataSend();
   timer=0;
  } 
 //delay(900);
 if (client.available()) 
  {
   char c = client.read();
   Serial.print(c);
  }
 delay(1); 
 timer++;
 client.stop();
 client.flush();
}

void DataSend()
{
  if (client.connect())  // Verbindung zum Server aufbauen
  {
    Serial.print("Verbonden...data verzenden...");
    //client.println("GET http://192.168.2.10/phpMyAdmin/mysqlvul.php?GAS=444&WATER=555&ELEC=666");
    //Naar MySQL database op webserver          
    client.print("GET http://192.168.2.10/phpMyAdmin/mysqlvul.php");
    client.print("&GAS=");
    client.print(GasIntWaarde++);
    client.print("&WATER=");
    client.print(WaterIntWaarde);
    client.print("&ELEC=");
    client.println(ElecKwhWaarde);
    client.println("HTTP/1.1\r\n");
    client.print("Host: http://192.168.2.10");
    client.println();
    
    
    strcpy(Tweet, "GAS= ");
    GasIntWaarde=GasIntWaarde+2; //test om tweetwaarde te veranderen
    itoa(GasIntWaarde, GasCharWaarde, 10);
    strcat(Tweet, GasCharWaarde);
    strcat(Tweet, "WATER= ");
    itoa(WaterIntWaarde, WaterCharWaarde, 10);
    strcat(Tweet, WaterCharWaarde);
    strcat(Tweet, "ELEC= ");
    itoa(ElecIntWaarde, ElecCharWaarde, 10);
    Serial.println(Tweet);
    twitter.post("Tweet");
    Serial.println("gereed!");
    GasIntWaarde=0;
    WaterIntWaarde=0;
    ElecKwhWaarde=0;
    //Tweet[0]=0;
  }
  else
  {
    Serial.println(" ***** VERBINDING NIET MOGELIJK (NAS wel aan??)*****");
  }    
}


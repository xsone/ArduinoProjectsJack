#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Time.h>

#include <SPI.h>
#include <LiquidCrystal.h>
#include <LCDKeypad.h>
LCDKeypad lcd;

//byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 }; // MAC-ADRES Ethershield zie onderkant
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[]  = { 192, 168, 178, 32 };                // IP-Adres van ethershield meterkast
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };
byte server[] = { 192, 168, 178, 20 };                 // IP van de NAS-server

EthernetClient client;
EthernetUDP Udp;

//Server server(80);
unsigned int localPort = 8888;

// Tijd instellingen
time_t t = 0UL; 
String tijdNu;
String tijdLog = "21:00:00";

volatile int state = 0;
volatile unsigned long int ZonKWhWaarde = 0;
volatile unsigned long int ZonIntWaarde = 0;
int InteruptTellerZon = LOW;
char ZonChar[10] = "0";
char ZonKWhChar[10] = "0";

int pulseLength = 0;
unsigned long lastMillis = 0;
int solarInput = false;
int sensorIsOn = 0;

int pin = 3;
int test = 0;


void setup()
{
  //LCD keyboard
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ZonPuls: ");
  lcd.setCursor(0,1);
  lcd.print("ZonKWh : ");
  
  //Serial
  Serial.begin(9600);			// voor debugging
  Serial.println("ZonneEnergyLogger Programma gestart...");
 
  
  //Ethernet
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  now();
  Serial.println(Ethernet.localIP());
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
  
  //Sensor
  pinMode(13, OUTPUT);  //LED indicator KWh meter
  digitalWrite(13, LOW); //KWh meter
  delay(2000);
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);

  pinMode(pin, INPUT_PULLUP);    //sensorinput Gas INT1
  sensorIsOn = false; 
}

void loop()
{
 lcd.setCursor(10,0);
 lcd.print(ZonIntWaarde);
 lcd.setCursor(10,1);
 lcd.print(ZonKWhWaarde);  
  
 tijdNu = getTimeString();
 
 Serial.println(tijdNu);
 delay(1000);
 
 if ( tijdNu == tijdLog )  // werkt voor loggen
  {
   Serial.print("Loggen  >  ");
   Serial.println(tijdNu);

   if(DataSend())
    {
     ZonIntWaarde=0;
     ZonKWhWaarde=0;
     delay(1000);
    }
   setSyncProvider(getNtpTime); //werkt niet 
  }
  
  
  if(ZonIntWaarde==1000) ZonKWhWaarde++; //1000 S0-pulsen is 1Kwh Zonneenergie
  if(ZonIntWaarde==999999) ZonIntWaarde=0; //KWhmeter Max. waarde bereikt
  
  // read the digital input
    int solarInput = digitalRead(pin);
    // rising edge?
    if(solarInput && sensorIsOn == false)
    {
        sensorIsOn=true;
    }
    // falling edge?
    if(!solarInput && sensorIsOn == true)
    {
     // update counters
     ZonIntWaarde++;
     sensorIsOn = false;
        
     Serial.print("ZonPuls: ");
     Serial.println(ZonIntWaarde, DEC);
     Serial.println("---");
     DataSend(); //test later op tijd
     //Serial.print("ZonKWh: ");
     //Serial.println(ZonKWhWaarde, DEC); 
        
     //pulseLength = 0;
    }
}



// Verstuur data naar MySQL database
int DataSend()
{
  //Client client = server.available()
  if (client.connect(server,80))  // Verbinding mer PHP-server opbouwen
  {
    Serial.print("Verbinden...zend zonsensordata...");
    //client.println("GET http://192.168.2.10/phpMyAdmin/mysqlvul.php?GAS=444&WATER=555&ELEC=666");
    //client.println("GET http://192.168.178.20/phpMyAdmin/mysqlzon.php?GAS=444&ZONWATT=555&ZONKWH=666"); //test PHP
    //Naar MySQL database op webserver          
    client.print("GET http://192.168.178.20/phpMyAdmin/mysqlzon.php");
    client.print("?GAS=000");
    client.print("&ZONWATT=");
    client.print(ZonIntWaarde);
    client.print("&ZONKWH=");
    client.println(ZonKWhWaarde);
    //client.println("HTTP/1.1\r\n");
    //client.print("Host: http://192.168.178.20");
    //client.println();
    Serial.println("gereed!");
    delay(1);
    client.stop();
    client.flush();    
    //ZonIntWaarde=0;
    //ZonKWhWaarde=0;
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


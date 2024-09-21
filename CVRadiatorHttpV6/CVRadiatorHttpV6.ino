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
 Update Dec. 2015
 Update Feb. 2016
 Update Apr. 2016 Ethernetshield vervangen, had moeite met IP
 
 */

/* LET op!!! geen digitale pinnen te gebruiken door combinatie LCD + Ethernet
According DFRobot site, the Ethernet Shield use digital pins 2, 10, 11, 12 & 13; 
it is compatible with the Arduino Ethernet library. 
While the LCD shield use digital pins 5, 6, 7, 8 & 9 and works with the LCB4Bit_Mod library, a modified version of LCD4bits.

Let op!!
Vanwege een pinconfilten tussen de Ethernet en LCD shields kunnen alleen de volgende pinnen worden gebruikt:

Digital: 0, 1, 2, 3, 5, 6, 7
Analog: 2, 3
*/

#include <SPI.h>
#include <DHT.h>
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address for PCF8574

//SDA van LCD naar Arduino Analog pin 4
//SCL van LCD naar Arduino Analog pin 5

#define DHTPIN 2          // DHT 11 aan pin 2
#define DHTTYPE DHT11     // DHT 11 (besteld bij dealextreme.com
#define SAMPLES 10        // nodig voor stabiele meting setpoint potmeter
//#define UDPSHOW 20
#define MAXSTOOKTIJD 8
DHT dht(DHTPIN, DHTTYPE); // Initialisatie DHT 11

//Algemene declaraties
float Hum = 20.0;
float Temp = 15.0;
int SetPoint = 15;
int SetPointOld = 15;
int Timer = 0;
int UdpTimer = 0; 
int State = LOW;
int motorState = LOW;
String Kamer = "JACK";
char CVstate = 'L';
char Smoke = 'L'; 
char Move = 'L';
char UdpMsg[20];

//Bewegingssensor
int MoveTeller = 0;
int UitGeweest = 0;

//UPD strings die worden verstuurd in bericht van 20 chars
String Buffer;
String SetPointString;
String TempString;
String HumString;

//IO definties
const int CVled = 7;         // Led voor CV aan/uit, andere pinnen geven problemen met LCD + ethershield 10,11,12,13 zijn bezet door Ethershield
const int pirPin = 3;        // pin voor input pir-bewegingssensor DS-SUN
const int potPin = 1;        // analog input pin for the potentiometer
const int motorPin1 =  5;    // 1e motor daard aan digital pin 5
const int motorPin2 =  6;    // 2e motor draad aan digital pin 6
byte pirState = LOW;  // status van pir-bewegingssensor

unsigned int potVal = 0;  // variable to store the value coming from the potentiometerchar potVALchar = 0;
byte UdpRemState = LOW;

//Ethernet instellingen kamerthermostaat
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x00, 0x8E, 0x11 }; // Laatste cijfer MAC steeds 1 ophogen
byte ip[]  = { 192, 168, 178, 81 };                  // IP-Adres van ethershield CV-ketel; kamers 81, 82 etc.
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };               // standdaard subnet 
byte phpserver[] = { 192, 168, 178, 20 };            // IP van de NAS-server
byte RemIP[] ={ 192, 168, 178, 80 };

//Ethernet instellingen CVketel server
//IPAddress RemIP(192, 168, 178, 80); //IP van CV ketel
unsigned int RemPort = 8888;      // local port to listen on
boolean alreadyConnected = false; // whether or not the client was connected previously

//byte server[] = { 209,85,148,147 }; 
//EthernetClient client(CVradiatorServer, 8888);//Ethernet server
EthernetClient test;
EthernetUDP Udp;
unsigned int localPort = 8888;           

//Startup
void setup()
{
 delay(1000); //i.v.m. reset board t.o.v. ethershield
 // Definitie van I/O
 pinMode(CVled, OUTPUT);
 pinMode(motorPin1, OUTPUT); 
 pinMode(motorPin2, OUTPUT);
 pinMode(pirPin, INPUT);
 pinMode(4, OUTPUT); 
 Serial.begin(9600);

 // Initialisatie alle comm. bussen 
 Wire.begin();
 dht.begin();  
 digitalWrite(4, HIGH);
 
 // LCD initialisatie en startscherm
 lcd.begin(16,2);
 lcd.backlight();
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Connecting IP...");

 // init Ethernet and TCP:
 Ethernet.begin(mac,ip, gateway, subnet); //vast IP adres
 delay(1000);
 
 //Serial test start
 Serial.println(Ethernet.localIP());
 Serial.println("Start programmaloop");


 //EthernetClient test;

 if (test.connect(gateway,80)) 
  {
   Serial.println("Connected");
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("IP connection...");
   delay(1000);
  } 
 else 
  {
   Serial.println("Not connected");
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("No IP connection...");
   delay(1000);
   Ethernet.begin(mac,ip, gateway, subnet); //vast IP adres
  } 
 
 Udp.begin(localPort);

 //Start basisfunctie kamerthermostaat
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(Ethernet.localIP());
 delay(1000);
 
 if(Udp.beginPacket(RemIP, RemPort))
  {
   Udp.endPacket();
   UdpRemState = HIGH;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Udp connection...");
  } 
 delay(1000);
 lcd.clear();
 lcd.print("...Radiator 1...");
 lcd.setCursor(0,1);
 lcd.print("Kamer ");
 lcd.setCursor(6,1);
 lcd.print(Kamer);
 delay(1000);
 lcd.clear();
 //lcd regel 1
 lcd.setCursor(0,0);
 lcd.print("CV=");
 lcd.setCursor(3,0);
 lcd.write(CVstate);
 lcd.setCursor(5,0);
 lcd.print("M=");
 lcd.setCursor(7,0);
 lcd.write(Move);
 lcd.setCursor(9,0);
 lcd.print("S=");
 lcd.setCursor(11,0);
 lcd.write(Smoke);
 //lcd-regel 2
 lcd.setCursor(0,1);
 lcd.print("S");
 lcd.setCursor(1,1);
 lcd.print(SetPoint);
 lcd.setCursor(4,1);
 lcd.print("T");
 lcd.setCursor(5,1);
 lcd.print(Temp,1);
 lcd.setCursor(11,1);
 lcd.print("H");
 lcd.setCursor(12,1);
 lcd.print(Hum,1);
 delay(1000);

 //Start metingen
 Temp = dht.readTemperature(0); //leest temperatuur uit in C = 0, was ook een float    
 Hum = dht.readHumidity(); 
 //RadiatorUit();
 SetPoint = SetPointOld;
 Serial.println("Showupd: ");
 ShowUdp();
 CVuit();
}

//Bewegingssensor
void MoveDetect()
{
 if(digitalRead(pirPin) == HIGH)
  {
  lcd.backlight();
  lcd.setCursor(15,0);
  lcd.write("|");
  pirState = HIGH;
  Move = 'H';
  Timer = 0;
  }
 if(digitalRead(pirPin) == LOW)
  {
   lcd.setCursor(15,0);
   lcd.write("_");
   pirState = LOW;
   Move = 'L';
   Timer++;
  }
 lcd.setCursor(7,0);
 lcd.write(Move);
 lcd.setCursor(13,0);
 lcd.print(Timer);
} 

void ShowUdp()
{
 //String Buffer = Kamer + String(Temp,1) + String(Hum,1) + CVstate + Move + Smoke; //oud werkte
 // Kamer[5], Temperature[4], Setpoint[4], Humidity[4], CVstate[1], Move[1], Smoke[1] totale lengte: 20 chars
 // Kamer[0-4], Temperature[5-8], Setpoint[9-12], Humidity[13-16], CVstate[17], Move[18], Smoke[19] totale lengte: 20 chars
 //String Buffer = Kamer + String(Temp,1) + String(SetPoint) + String(Hum,1) + CVstate + Move + Smoke; //oude situatie verschuift
 String Buffer = Kamer + TempString + SetPointString + HumString + CVstate + Move + Smoke;
 //Buffer.toCharArray(UdpMsg, 16); //oud werkte
 Buffer.toCharArray(UdpMsg, 18);
 Serial.print("Buffer: "); Serial.println(Buffer);
 Udp.beginPacket(RemIP, RemPort);
 Udp.write(UdpMsg); //25 max lang
 Udp.endPacket();
}

/*
void softReset() // Restarts program from beginning but does not reset the peripherals and registers
{
asm volatile ("  jmp 0");
}  
*/

/*
int RadiatorUit()
{ 
 rotateDichtFull(15000);
 delay(3000);
 if (motorState == HIGH) rotateDichtFull(5000); //Radiatorkraan Dicht
 return(0);
}

int RadiatorAan()
{ 
 rotateOpenFull(15000);
 delay(3000);
 if (motorState == LOW) rotateOpenFull(5000); //Radiatorkraan Open
 return(1);
}
*/

// Post CVaan bericht, display en regel motor
void CVaan()
{
 CVstate = 'H';
 digitalWrite(CVled, HIGH);
 lcd.setCursor(3,0);
 lcd.write(CVstate);
}  

// Post CVuit bericht, display en regel motor
void CVuit()
{
 CVstate = 'L';
 digitalWrite(CVled, LOW);
 lcd.setCursor(3,0);
 lcd.write(CVstate);
 //if(!RadiatorUit()) RadiatorUit();
} 

void loop()
{
 Hum = dht.readHumidity(); //leest RH uit in %, was orgineel een float waarde
 Temp = dht.readTemperature(); //leest temperatuur uit in C, was ook een float       
 
 //for (int i=0; i < SAMPLES ; i++) 
 // {
  // potVal = analogRead(potPin); 
 //  potVal /= SAMPLES ;
 // }
 
 //SetPoint = (potVal+0.5)/8; //bereik Potmater 0-128, bereik Setpoint 0-30 C dus delen door 4
 //SetPoint = (potVal)/20; //bereik Potmater 0-128, bereik Setpoint 0-30 C dus delen door 4 
 SetPoint = (analogRead(potPin)/15); //bereik Potmater 0-128, bereik Setpoint 0-30 C dus delen door 4 

 if ( (pirState == LOW) && (Timer > MAXSTOOKTIJD) ) 
  { 
   if (CVstate == 'H') CVuit(); 
   lcd.noBacklight(); 
   Timer = MAXSTOOKTIJD;
     //RadiatorUit();
  }
 else
  {
   if (Temp > SetPoint && CVstate == 'H') CVuit();
   //if (Temp <= SetPoint && CVstate == LOW) //hoort bij PIR sensor
   if (Temp < SetPoint && CVstate == 'L') CVaan();
  } 
 
 MoveDetect();
 ShowActVal();
 delay(250); //vertraag 10ms
 //if (UdpTimer > UDPSHOW && UdpRemState) 
 ShowUdp();
}

// Display actuele status
void ShowActVal()
{
 lcd.setCursor(1,1);
 lcd.print("   ");
 lcd.setCursor(1,1);
 lcd.print(SetPoint);
 lcd.setCursor(5,1);
 lcd.print("   ");
 lcd.setCursor(5,1);
 lcd.print(Temp,1);
 lcd.setCursor(12,1);
 lcd.print("   ");
 lcd.setCursor(12,1);
 lcd.print(Hum,1);
 SetPointOld = SetPoint;
 
 //Zorg ervoor dat altijd 3 digits zijn, voorloop '0' toevoegen i.v.m. verschuiven UDP bericht
 if (Temp < 10) TempString ='0' + String(Temp,1);
 else TempString = String(Temp,1);
 
 if (SetPoint < 10) SetPointString ='0' + String(SetPoint);
 else SetPointString = String(SetPoint);
 
 if (Hum < 10) HumString ='0' + String(Hum,1);
 else HumString = String(Hum,1);
}

#include <ATT.h>
#include <BT.h>
#include <NewSoftSerial.h>
#include <SSerial2Mobile.h>
#include <EEPROM.h>

/*
Zend SMS en email via BlueTooth naar mobiele telefoon.

created Nov 2011
by Jack Cop
*/

//#define RXpin 19 //BlueTooth-module TX groene draad  //geldt voor Mega
//#define TXpin 18 //BlueTooth-module RX gele draad    //geldt voor Mega
#define RXpin 0 //BlueTooth-module TX groene draad     //geldt voor Lilypad
#define TXpin 1 //BlueTooth-module RX gele draad       //geldt voor Lilypad

//om date in EEPROM memory op te slaan kunnen de functies read() en write() worden gebruikt
// #include <EEPROM.h>
// value = EEPROM.read(a);
// for(int i=0; i<512; i++) EEPROM.write(i,i); 512 bytes beschikbaar bij Lilypad.
//bijvoorbeeld opslaan van BT-adress, Koenummer, Tijd, Telefoonnumer dat gebeld moet worden.
//deze data kan via een PHP of C++ applicatie worden ingevoerd en naar de Arduino worden gestuurd.

const int power = 500;  // Powersupply = 5V(500) or 3V(300)
const int xpin = 3;     //A3-analog input X-axis of the accelerometer
const int ypin = 2;     //A2-analog input Y-axis
const int zpin = 1;     //A1-analog input Z-axis (alleen bij 3-axis models)
int staartHoog = 0;
int staartLaag = 0;
int ligtLinks = 0;
int ligtRechts = 0;
char status[20];


int tijd = 2000;       //wachttijd voordat SMS wordt verstuurd
int switchLed = 13; //output LED microswitch sensor onboard 13 anders connectie 6
int tel = 0; //test of loop is doorlopen
int hoogGeweest = 0; //test op loop is doorlopen

void setup()
{
 pinMode(switchLed, OUTPUT); // set direction of led pin
 pinMode(2, INPUT);            //sensorinput
  
 NewSoftSerial mySerial(RXpin,TXpin);
 //SSerial2Mobile phone = SSerial2Mobile(RXpin,TXpin);
 mySerial.begin(38400);
 //phone.init();

 for(;;)
 {
  if(analogRead(xpin)<(power-50)) ligtRechts=1;
   else ligtRechts =0;
  if(analogRead(xpin)>(power+50)) ligtLinks=1;
   else ligtLinks=0;
  if(analogRead(ypin)<(power-50)) staartHoog=1;
   else staartHoog=0;
  if(analogRead(ypin)>(power+50)) staartLaag=1;
   else staartLaag=0;
 
 /*       
  if(staartHoog==1 && hoogGeweest==0) 
   {
    delay(tijd);
    if(ligtRechts==1 || ligtLinks==1)
     {
      //phone.sendTxt("0649982176","L"); 
      mySerial.println("Ligt");
     } 
    //phone.sendTxt("0649982176","H"); 
    mySerial.println("Staartomhoog");
    //phone.sendEmail("jack.cop@kpnplanet.nl","koe");
    digitalWrite(switchLed,HIGH);
    hoogGeweest=1;
   } 
  if(staartLaag)
   {
    hoogGeweest=0;
    digitalWrite(switchLed,LOW);
   } 
 }
}
     
   
   if(digitalRead(2)==HIGH&&hoog==0) // test op sensorinput 2
   {
    delay(tijd); // wachttijd voordat SMS wordt verstuurd
   */
    if((ligtRechts && staartHoog)==1) mySerial.println("Rechterzij + Staartomhoog");
    if((ligtRechts && staartLaag)==1) mySerial.println("Rechterzij + Staartomlaag");
    if((ligtLinks && staartHoog)==1) mySerial.println("Linkerzij + Staartomhoog");
    if((ligtLinks && staartLaag)==1) mySerial.println("Linkerzij + Staartomlaag");
    if(staartHoog) mySerial.println("Staartomhoog");
    if(staartLaag) mySerial.println("Staartomlaag");
    delay(1000);
  
  /*
    if(digitalRead(2)==HIGH)
     {
      digitalWrite(switchLed,HIGH);   //LED om sensoractief te laten zien
      phone.sendTxt("0649982176","H"); //Staart omhoog
      phone.sendEmail("jack.cop@kpnplanet.nl","Koemail");
      tel=0;
      hoog=1;
     }
   }
  if(digitalRead(2)==LOW)
   {
    hoog=0; //sensorinput 2 
    digitalWrite(switchLed,LOW);
   } 
 }
 */
 }
} 



void loop(){}

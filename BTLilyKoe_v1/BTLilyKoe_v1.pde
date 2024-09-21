#include <ATT.h>
#include <BT.h>
#include <NewSoftSerial.h>
#include <SSerial2Mobile.h>
//#include <EEPROM.h>

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

const int power = 300;  // Powersupply = 5V(500) or 3V(300)
const int xpin = 3;     //A3-analog input X-axis of the accelerometer
const int ypin = 2;     //A2-analog input Y-axis
const int zpin = 1;     //A1-analog input Z-axis (alleen bij 3-axis models)
int Hoog = 0;
int staartHoog = 0;
int staartLaag = 0;
int ligtLinks = 0;
int ligtRechts = 0;
char status[40];
char staartTijdChar[5];
char oldstatus[40];

int msgTijd=2000;       //wachttijd voordat SMS wordt verstuurd
int staartTijd=0;
int switchLed = 13; //output LED microswitch sensor onboard 13 anders connectie 6
int tel = 0; //test of loop is doorlopen
int hoogGeweest = 0; //test op loop is doorlopen
int state;
//NewSoftSerial mySerial(RXpin,TXpin);
SSerial2Mobile phone = SSerial2Mobile(RXpin,TXpin);

void setup()
{
 pinMode(switchLed, OUTPUT); // set direction of led pin
 pinMode(2, INPUT);            //sensorinput
  
 //Code voor serial port bijv. PC
 // mySerial.begin(38400);
 //strcpy(status,"Beginwaarde");


 
 //Code voor phone SMS
 SSerial2Mobile phone = SSerial2Mobile(RXpin,TXpin);
 phone.init();
 




for(;;)
  {
   //AD beweginsgsensor
   if(analogRead(xpin)<(power-50)) ligtRechts=1; //ligtRechts=1;
    else ligtRechts=0;
   if(analogRead(xpin)>(power+50)) ligtLinks=1;
    else ligtLinks=0;
   if(analogRead(ypin)<(power-50)) staartHoog=1;
    else staartHoog=0;
   if(analogRead(ypin)>(power+50)) staartLaag=1;
    else staartLaag=0;
   
   if((ligtRechts && staartHoog)==1) strcpy(status,"Rechterzij + Staartomhoog"); statusUpdate();
   if((ligtRechts && staartLaag)==1) strcpy(status,"Rechterzij + Staartomlaag"); statusUpdate();
   if((ligtLinks && staartHoog)==1) strcpy(status,"Linkerzij + Staartomhoog"); statusUpdate();
   if((ligtLinks && staartLaag)==1) strcpy(status,"Linkerzij + Staartomlaag"); statusUpdate();
   if(staartHoog) strcpy(status, "Staartomhoog"); statusUpdate();
   if(staartLaag) strcpy(status, "Staartomlaag"); statusUpdate();
   


   /*
   //Code voor het versturen van SMS
   if((ligtRechts && staartHoog)==1) phone.sendTxt(telnr, "Rechterzij + Staartomhoog");
   if((ligtRechts && staartLaag)==1) phone.sendTxt(telnr, "Rechterzij + Staartomlaag");
   if((ligtLinks && staartHoog)==1) phone.sendTxt(telnr, "Linkerzij + Staartomhoog");
   if((ligtLinks && staartLaag)==1) phone.sendTxt(telnr, "Linkerzij + Staartomlaag"); 
   */

    
    
    //Switch
    if(digitalRead(2)==HIGH && Hoog==0) // test op sensorinput 2
    {
     digitalWrite(switchLed,HIGH);   //LED om sensoractief te laten zien
     Hoog=1;
    }
   if(digitalRead(2)==LOW && Hoog==1)
    {
     itoa(staartTijd, staartTijdChar, 10);
     //strcat(staartTijdChar, "ms. Hoog");
     //mySerial.println(staartTijdChar);
     phone.println(staartTijdChar);
     //phone.sendTxt("0649982176",staartTijdChar); //Staart omhoog
     //phone.sendEmail("jack.cop@kpnplanet.nl",staartTijdChar);
     //strncpy(staartTijdChar,"    ",4);
     staartTijd=0;
     Hoog=0;                         // test op sensorinput 2 
     digitalWrite(switchLed,LOW);
    } 
       if(staartTijd>7200) staartTijd=0; //check op > 2uur
   staartTijd++;
   delay(1000);                       // Teller hoeveel sec. staart hoog is geweest
  } 
} 


void statusUpdate()
 {
  //if (strcmp(status, oldstatus)!=0) mySerial.println(status);  //statusverandering
  if (strcmp(status, oldstatus)!=0) phone.println(status);  //statusverandering
  strcpy(oldstatus,status); 
 } 

void loop(){}




/*
   if (staartTijd > msgTijd)
    {
     itoa(staartTijd, staartTijdChar, 10);
     strcat(staartTijdChar, "ms. Hoog");
     //mySerial.println("staartTijdChar");
     phone.sendTxt("0649982176",staartTijdChar); //Staart omhoog
     //phone.sendEmail("jack.cop@kpnplanet.nl",staartTijdChar);
     staartTijd=0;
    }  
*/

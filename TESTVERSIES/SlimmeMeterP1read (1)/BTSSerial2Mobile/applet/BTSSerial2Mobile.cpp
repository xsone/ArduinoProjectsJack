#include <ATT.h>
#include <BT.h>
#include <NewSoftSerial.h>
#include <SSerial2Mobile.h>

/*
Zend SMS en email via BlueTooth naar mobiele telefoon.

created Nov 2011
by Jack Cop
*/

#define RXpin 19 //BlueTooth-module TX groene draad 
#define TXpin 18 //BlueTooth-module RX gele draad

#include "WProgram.h"
void setup();
void loop();
NewSoftSerial mySerial(RXpin, TXpin); // test BT kan later weg

int tijd = 2000; //wachttijd voordat SMS wordt verstuurd
int switchLed = 13; //output LED microswitch sensor onboard 13 anders connectie 6
int tel = 0; //test of loop is doorlopen
int hoog = 0; //test op loop is doorlopen
char buffer[90];

void setup()
{
 pinMode(switchLed, OUTPUT); // set direction of led pin
 pinMode(2, INPUT);          //sensorinput
  
 Serial.begin(9600);
 SSerial2Mobile phone = SSerial2Mobile(RXpin,TXpin);
 //phone.init();
 Serial.print(phone.readline());
 delay(1000);
 /*
 for(;;)
 {
  if(digitalRead(2)==HIGH&&hoog==0) // test op sensorinput 2
   {
    delay(tijd); // wachttijd voordat SMS wordt verstuurd
    if(digitalRead(2)==HIGH)
     {
      digitalWrite(switchLed,HIGH);   //LED om sensoractief te laten zien
      phone.sendTxt("0649982176","KoeSMS");
      phone.sendEmail("jack.cop@kpnplanet.nl","KoeEmail");
      tel=0;
      hoog=1;
     }
   }
  if(digitalRead(2)==LOW)
   {
    hoog=0; //sensorinput 2 
    digitalWrite(switchLed,LOW);
   } 
 }*/
}

void loop(){}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


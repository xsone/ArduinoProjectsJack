/*********************************************************************
 **  Description:                                                    **
 **  This file is a sample code for your reference.                  **
 **                                                                  **
 **  Copyright (C) 2011 ElecFreaks Corp.                             **
 **  Created by ElecFreaks Robi.W /29 Sep 2011                      **
 **                                                                  **
 **  http://www.elecfreaks.com                                       **
 *********************************************************************/
#include <NewSoftSerial.h>
//#include <EEPROM.h>
//#include <TimerOne.h>

#define rxPin 4 //was 2 (conflict met sensorinput)
#define txPin 5 //was 3

//NewSoftSerial mySerial(rxPin, txPin);
int AlarmTijd = 10; //tijdstip waarna alarm moet worden gegeven
int EepromAdres = 0;
 
void Callback()
{
   Serial.println("------->  Callback Send AT");
   //mySerial.print("AT\r\n");
}
 
void setup()
{
  // define pin modes for tx, rx pins:
              
   pinMode(rxPin, INPUT);
   pinMode(txPin, OUTPUT);
   //mySerial.begin(38400);
   Serial.begin(38400);
   Serial.println("------->  Start Sending AT");
   //delay(2000);
   //PhoneInit();
   //Timer1.initialize(2000000);             // setting callback is 2s
   //Timer1.attachInterrupt(Callback); 
   
 //  EEPROM.write(EepromAdres, AlarmTijd);  //Schrijf alamrTijd naar geheugen op adres #0
 //  AlarmTijd = EEPROM.read(EepromAdres);  //Lees AlarmTIjd uit EEPROM gegheugen 
 //  Serial.println(AlarmTijd); 
}

void loop(){}



/*
void PhoneInit()
{
  Serial.println("AT"); delay(500);
    mySerial.println("AT"); delay(500);
  Serial.println("AT+RESET"); delay(500);
    mySerial.println("AT+RESET"); delay(500);
  Serial.println("AT+ORGL"); delay(500);
    mySerial.println("AT+ORGL"); delay(500);
  Serial.println("AT+INIT"); delay(500);
    mySerial.println("AT+INIT"); delay(500);
  Serial.println("AT+CLASS=0040"); delay(500);
    mySerial.println("AT+CLASS=0040"); delay(500); //adres BTshield nodig bij Andoid 2.x versies wegens fout in kernel
 //Serial.println("AT+ENSNIFF=0012,06,071015"); delay(500); //niet nodig
 // mySerial.println("AT+ENSNIFF=0012,06,071015"); delay(500); //niet nodig
  Serial.println("AT+INQ"); delay(500);
    mySerial.println("AT+INQ"); delay(500);
 // Serial.println("AT+LINK=7445,8a,4d7fea"); delay(500); //pair direct met samasung GT-S5300 phone
 //   mySerial.println("AT+LINK=7445,8a,4d7fea"); delay(500); //pair direct met samasung GT-S5300 phone
}
    

void loop()
{
 mySerial.println("100");
 //mySerial.print("\r\n");
 delay(100);
 mySerial.println("50 ");
  //mySerial.print(waarde++);
// Serial.println(waarde);
//  if(waarde==99) waarde=0;
//  waarde++;

  waarde = EEPROM.read(a);

  Serial.print(a);
  Serial.print("\t");
  Serial.print(waarde);
  Serial.println();

  a = a + 1;

  if (a == 512)
    a = 0;

  delay(500);




}



/*
void loop()
{
  int i = 0;
  char someChar[32] = {0};
  // when characters arrive over the serial port...
  if(Serial.available()) {
    do{
      someChar[i++] = Serial.read();
      //As data trickles in from your serial port you are grabbing as much as you can, 
      //but then when it runs out (as it will after a few bytes because the processor 
      //is much faster than a 9600 baud device) you exit loop, which then restarts, 
      //and resets i to zero, and someChar to an empty array.So please be sure to keep this delay 
      delay(3);                  
 
    }while (Serial.available() > 0);
 
    mySerial.println(someChar);
    Serial.println(someChar);
  }
 
  while(mySerial.available() > 0) 
      Serial.print((char)mySerial.read());    
}
*/

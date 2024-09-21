/*
 Arduino Mega + BT communication
 BT Tx > Mega Pin19 (RxD119)
 BT Rx > Mega Pin18 (TxD118)
 
 created 16 sept. 2011
 by Jack Cop
 
 */

#include <ATT.h>
#include <BT.h>
#include <NewSoftSerial.h>
#include <SSerial2Mobile.h>

#define RXpin 19 //BlueTooth-module TX groene draad 
#define TXpin 18 //BlueTooth-module RX gele draad
#define BUFFER 40
#define BUFFSIZ 90

NewSoftSerial mySerial(RXpin, TXpin);
//SSerial2Mobile phone = SSerial2Mobile(RXpin,TXpin);

char Buffer[BUFFSIZ];
int Index = 0;

char str[20] = {0};
char instr[40];
char c;
int LED = 13;
int t=0;
int index=0;

int tijd = 2000; //wachttijd voordat SMS wordt verstuurd
int switchLed=13; //output LED microswitch sensor onboard 13 anders connectie 6
int tel = 0;
int hoog = 0;

void setup()
{
 pinMode(switchLed, OUTPUT); // set direction of led pin
 pinMode(22, INPUT);            //sensorinput
 
 //default SparkFun BT module baud rate
 Serial.begin(9600);
 mySerial.begin(38400);
 BT_init();
}


void BT_init()
{
  Serial.println("..start BT verbinding...\r\n");
  mySerial.println("AT\r\n");
  delay(500);
  Serial.println(mySerial.read());
  mySerial.println("AT+INIT\r\n");
  delay(500);
  Serial.println(mySerial.read());
  mySerial.println("AT+INQ\r\n");
  delay(500);
  Serial.println(mySerial.read());
  mySerial.println("AT+ADDR?\r\n");
  Serial.println(mySerial.read());
  mySerial.println("AT+MRAD?\r\n");
  Serial.println(mySerial.read());
   //BT_commando("AT\r\n");
  delay(1000);
  Serial.println("..door AT...\r\n");
  //Serial1.println("AT");
  //phone.readline();
  //Serial.println(buffer);
  /*
  //mySerial.write("AT");
  delay(500);
  //Serial.println(mySerial.read());
  //Serial.println(NewSoftSerila.read(),BYTE);
  
  index = 0; // start at begninning
  while (1) {
      c = mySerial.read();
      Serial.println(c);
      if (c == -1)
        continue;
     // Serial.print(c);
      if (c == '\n')
        continue;
      if ((index == BUFFSIZ-1) || (c == '\r')) {
        Buffer[index] = 0;
        return;
      }
      Buffer[index++]= c;
      Serial.println(c);
  }
 Serial.println(Buffer);//there's no write in this version of Arduino
 delay(500);
 */   
  
  
  /*  
  if (mySerial.available() > 0)
   {
    for (int i = 0; i < 10; i++)
     {
      Buffer[i] = mySerial.read();//data corrupted here
      i++;
      break;
     }
    }
  Serial.print(Buffer);//there's no write in this version of Arduino
  delay(500);
}  
*/
  
  
  
  
  
  //BT_commando("AT+INIT\r\n");
  //BT_commando("AT+INQ\r\n");
  //BT_commando("AT+ADDR?\r\n");
  //BT_commando("AT+MRAD?\r\n");
  
//  BT_commando("AT+STATE?\r\n");
  //if (strcmp(str,"OK")) Serial.println("Antwoord is OK .. ga verder\r\n");
 // if (strcmp(str,"OK") Serial.println("antwoord is OK .. ga verder");
  //BT_commando("AT+MRAD=23, D7, 7C0\r\n");
  //BT_commando("AT+STATE?\r\n");
  
  //BT_commando("AT+LINK=23, D7, 7C0573\r\n");
  //BT_commando("AT+CMGF=0\r\n");  
  //delay(300);  
  //BT_commando("AT+CMGS=18\r\n"); 
  //delay(300);
  //BT_commando("07911326060032F011000B911346992871F60004AA044B616C66"); 
  //delay(300);
  
  //BT_commando("ATDT0649982176\r\n");
  //digitalWrite(12,HIGH);
//  Serial1.println("AT\r\n");
//  Serial.println(Serial1.read());
//  delay(5000);
  
//  Serial.println(Serial1.read());
//  delay(300);
//  Serial1.println("ATDT\r\n");
//  Serial.println(Serial1.read());
  ///BT_commando("AT+CMGS=18\r\n"); 
  //delay(300);
  //BT_commando("07911326060032F011000B911346992871F60004AA044B616C66"); 
//  delay(300);
//  Serial.println("einde BT-init...\r\n");
  //Serial1.print("ATE0\r\n");
  //itoa(Serial1.read(),string,16);
  //Serial.print(string);
  //delay(500);
  //Serial1.print("ATWS\r\n");
  //itoa(Serial1.read(),string,16);
  //delay(500);
  //Serial.print(string); 
  
  //Serial.println("\r\r...init end...");
  //delay(500);
  
}


void BT_commando(char *str)
{
 byte inByte;
 int i=0;
 int t=0;
   
 if (mySerial.available())
  {
   mySerial.println(str);
   delay(1000);
   Serial.println((char)mySerial.read());
  } 
} 

 
void loop(){}


#include <SoftwareSerial.h>
//#include <NewSoftSerial.h>
#define rxPin 4
#define txPin 5
SoftwareSerial mySerial(rxPin, txPin); // RX, TX
//NewSoftSerial mySerial(rxPin, txPin);

int ledPin = 13; //output LED microswitch sensor onboard 13 anders connectie 6
int sensorPin = 8;
int tel = 0; //test of loop is doorlopen
int hoogGeweest = 0; //test op loop is doorlopen
int state;
int val;
int hoogInt;
char hoogStr[10];
char newstatus[40];
char oldstatus[40];


void setup()
{
 pinMode(rxPin, INPUT);
 pinMode(txPin, OUTPUT);
 mySerial.begin(38400);
  
 pinMode(ledPin, OUTPUT); // set direction of led pin
 pinMode(sensorPin, INPUT);            //sensorinput
 Serial.begin(38400); 
 Serial.println("------->  Start Sending AT");
 PhoneInit();
}

void PhoneInit()
{
  Serial.println("AT+RESET"); delay(500);
    mySerial.println("AT+RESET"); delay(500);
  //Serial.println("AT+ORGL"); delay(500);
  //  mySerial.println("AT+ORGL"); delay(500);
  Serial.println("AT+INIT"); delay(500);
    mySerial.println("AT+INIT"); delay(500);
  Serial.println("AT+ENSNIFF=0012,06,071015"); delay(500);
    mySerial.println("AT+ENSNIFF=0012,06,071015"); delay(500);
  Serial.println("AT+CLASS=0040"); delay(500);
    mySerial.println("AT+CLASS=0040"); delay(500); //adres BTshield
  Serial.println("AT+INQ"); delay(500);
    mySerial.println("AT+INQ"); delay(500);
 // Serial.println("AT+LINK=7445,8a,4d7fea"); delay(500); //pair direct met samasung GT-S5300 phone
 //   mySerial.println("AT+LINK=7445,8a,4d7fea"); delay(500); //pair direct met samasung GT-S5300 phone

//  Serial.println("AT+LINK=12, 06, 071015"); delay(500);
//    mySerial.println("AT+LINK=12, 06, 071015"); delay(500); //adres BTshield
 // Serial.println("AT+PAIR=7445,8a,4d7fea,30"); delay(500);
 //   mySerial.println("AT+PAIR=7445,8a,4d7fea,30"); delay(500); //adres BTshield
 //Serial.println("AT+LINK=15, 83, F128CC"); delay(500);
}  

void statusUpdate()
 {
  strcpy(oldstatus,"987654321+9876543210");
   if (strcmp(newstatus, oldstatus)!=0) 
   {
    Serial.println(newstatus);  //statusverandering
    mySerial.println(newstatus);  //statusverandering
   } 
  strcpy(oldstatus,newstatus); 
 } 


void loop()
{
 delay(1000);
 tel++;
 if(tel=30000) mySerial.println("OK");
 if (digitalRead(sensorPin) == LOW) 
 {     
  digitalWrite(ledPin, HIGH);
  hoogInt++;
  itoa(hoogInt,hoogStr,10);
  strcpy(newstatus,"123456789+123456789"); statusUpdate();
  
  Serial.println(hoogStr);
  mySerial.println(hoogStr);
  //mySerial.println("test");
  //Serial.println("test");
  
  delay(1000); // Teller hoeveel sec. staart hoog is geweest
 } 
 else
  {
   digitalWrite(ledPin, LOW);
   digitalWrite(sensorPin, HIGH);
  } 
 //while(mySerial.available() > 0) Serial.print((char)mySerial.read());  
}

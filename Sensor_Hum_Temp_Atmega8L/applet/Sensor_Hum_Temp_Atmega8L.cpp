//Sensor KY015 Arduino Sample Code

#include <Time.h>
#define dht_dpin 23 //A0 //was A5

#include "WProgram.h"
void setup();
void PhoneInit();
void loop();
void InitDHT();
void ReadDHT();
byte read_dht_dat();
byte bGlobalErr;
byte dht_dat[5];

char HumiChar[10] = "0";
char TempChar[10] = "0";

const long oneSecond = 1000;
const long oneMinute = oneSecond * 60;
const long oneKwart  = oneMinute * 15;
const long oneHour   = oneMinute * 60;
const long oneDay    = oneHour * 24;
int timer;


void setup()
{
  Serial.begin(38400);
  InitDHT();
  PhoneInit();
  //setTime(12,0,0,1,1,13); //0:00uur 1/1/2013
  Serial.println("Humidity and temperature");
  
}
 
void PhoneInit()
{
 Serial.println("AT+RESET"); delay(500);
 Serial.println("AT+INIT"); delay(500);
 Serial.println("AT+CLASS=0040"); delay(500);
 //Serial.println("AT+ENSNIFF=0011,07,190012"); delay(500); //1e BT-device name TEST
 Serial.println("AT+ENSNIFF=0012,06,071015"); delay(500); //2e BT-device name HC06
 Serial.println("AT+INQ"); delay(500);
 //Serial.println("AT+PAIR=7445,8a,4d7fea,30"); delay(500); //pair direct met samasung GT-S5300 phone
 Serial.println("AT+LINK=7445,8a,4d7fea"); delay(500); //pair direct met samasung GT-S5300 phone
 //Serial.println("AT+LINK=15, 83, F128CC"); delay(500); // link direct met samsung SGH-160 phone
}  


void loop()
{
  ReadDHT();
  switch (bGlobalErr)
  {
   case 0:
    Serial.print(hour()); Serial.print(":");
    Serial.print(minute()); Serial.print(":");
    Serial.print(second()); Serial.print(":");
    Serial.print(day()); Serial.print(":");
    Serial.print(month()); Serial.print(":");
    Serial.print(year()); Serial.print(":");
    
    Serial.print(dht_dat[0], DEC);
    Serial.print(".");
    Serial.print(dht_dat[1], DEC);
    Serial.print("%  ");
    
    Serial.print(dht_dat[2], DEC);
    Serial.print(".");
    Serial.print(dht_dat[3], DEC);
    Serial.println("C  ");
   break;
   case 1:
    Serial.println("Error 1: DHT start condition 1 not met.");
   break;
   case 2:
    Serial.println("Error 2: DHT start condition 2 not met.");
   break;
   case 3:
    Serial.println("Error 3: DHT checksum error.");
   break;
   default:
    Serial.println("Error: Unrecognized code encountered.");
   break;
 }
 delay(1000); // elk kwartier
}

 

void InitDHT()
{
 pinMode(dht_dpin,OUTPUT);
 digitalWrite(dht_dpin,HIGH);
}

 
void ReadDHT()
{
 bGlobalErr=0;
 byte dht_in;
 byte i;
 digitalWrite(dht_dpin,LOW);
 delay(20);
 digitalWrite(dht_dpin,HIGH);
 delayMicroseconds(40);
 pinMode(dht_dpin,INPUT);
 //delayMicroseconds(40);
 dht_in=digitalRead(dht_dpin);

 if(dht_in)
  {
   bGlobalErr=1;
   return;
  }
  delayMicroseconds(80);
  dht_in=digitalRead(dht_dpin);
 if(!dht_in)
  {
   bGlobalErr=2;
   return;
  }
  delayMicroseconds(80);
 for (i=0; i<5; i++) dht_dat[i] = read_dht_dat();
 pinMode(dht_dpin,OUTPUT);
 digitalWrite(dht_dpin,HIGH);
 
 byte dht_check_sum = dht_dat[0]+dht_dat[1]+dht_dat[2]+dht_dat[3];
  if(dht_dat[4]!= dht_check_sum) {bGlobalErr=3;}
};

 

 byte read_dht_dat()
 {
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++)
   {
    while(digitalRead(dht_dpin)==LOW);
    delayMicroseconds(30);
    if (digitalRead(dht_dpin)==HIGH)
    result |=(1<<(7-i));
    while (digitalRead(dht_dpin)==HIGH);
   }
  return result;
 }

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


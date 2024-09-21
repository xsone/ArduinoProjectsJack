//#include <EEPROM.h>

/*
Zend SMS en email via BlueTooth naar mobiele telefoon.

created Nov 2011
by Jack Cop
*/

//#define RXpin 19 //BlueTooth-module TX groene draad  //geldt voor Mega
//#define TXpin 18 //BlueTooth-module RX gele draad    //geldt voor Mega
//#define RXpin 0 //BlueTooth-module TX groene draad     //geldt voor Lilypad
//#define TXpin 1 //BlueTooth-module RX gele draad       //geldt voor Lilypad

//om date in EEPROM memory op te slaan kunnen de functies read() en write() worden gebruikt
// #include <EEPROM.h>
// value = EEPROM.read(a);
// for(int i=0; i<512; i++) EEPROM.write(i,i); 512 bytes beschikbaar bij Lilypad.
//bijvoorbeeld opslaan van BT-adress, Koenummer, Tijd, Telefoonnumer dat gebeld moet worden.
//deze data kan via een PHP of C++ applicatie worden ingevoerd en naar de Arduino worden gestuurd.

#include <Time.h>

// Time definities
const long oneSecond = 1000;
const long oneMinute = oneSecond * 60;
const long oneHour   = oneMinute * 60;
const long oneDay    = oneHour * 12;
int timer = 0;


#define zero_x 400 // referentiewaarde Y-as is alleen relevant voor staartmeting: zero_y=0°; up_y=90°; down_y=270°
#define zero_y 400 // referentiewaarde Y-as is alleen relevant voor staartmeting: zero_y=0°; up_y=90°; down_y=270°
#define zero_z 400 // referentiewaarde Y-as is alleen relevant voor staartmeting: zero_y=0°; up_y=90°; down_y=270°
#define factor 0.9 //een hoekverdraaiing van 90 graden komt overeen met 100 eenheden dus factor=90/100=0.9

unsigned int value_x;
unsigned int value_y;
unsigned int value_z;

int angle_x;
int angle_y;
int angle_z;

int angle_x_old;
int angle_y_old;
int angle_z_old;


void setup()
{
 setTime(0,0,0,1,1,13); // set time to noon Jan 1 2013
 Serial.begin(38400); 
 PhoneInit();
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
 value_x = analogRead(3);
 value_y = analogRead(2);
 value_z = analogRead(1);
 
 angle_x=90+((value_x-zero_x)*factor); //een hoekverdraaiing van 90 graden komt overeen met 100 eenheden dus factor=90/100=0.9
 angle_y=90+((value_y-zero_y)*factor); //een hoekverdraaiing van 90 graden komt overeen met 100 eenheden dus factor=90/100=0.9
 angle_x=90+((value_z-zero_z)*factor); //een hoekverdraaiing van 90 graden komt overeen met 100 eenheden dus factor=90/100=0.9
 
 
 if(angle_x!=angle_x_old)
  {
   Serial.print(hour());
   Serial.print(":");
   Serial.print(minute());
   Serial.print(":");
   Serial.print(second());   
   Serial.print(" X: ");
   Serial.println(angle_x);
   angle_x_old=angle_x;
  } 
 if(angle_y!=angle_y_old)
  {
   Serial.print(hour());
   Serial.print(":");
   Serial.print(minute());
   Serial.print(":");
   Serial.print(second());
   Serial.print(" Y: ");
   Serial.println(angle_y);
   angle_y_old=angle_y;
  } 
 //Serial.println(angle_z);

 //Serial.print(" deg");
 //Serial.print("\n");
 if(timer==10)
 {
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.println();
  timer=0;
 } 
 delay(oneSecond);
}

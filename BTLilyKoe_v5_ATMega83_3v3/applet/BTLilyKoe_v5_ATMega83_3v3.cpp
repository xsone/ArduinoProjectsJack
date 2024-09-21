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
//#include <Bluetooth_HC05.h>
#include <NewSoftSerial.h>

#define ref_x 400 // 400 bij ronde ADXL; 300 bij vierkante ADXL; referentiewaarde Y-as is alleen relevant voor staartmeting: zero_y=0\u00b0; up_y=90\u00b0; down_y=270\u00b0
#define ref_y 400 // referentiewaarde Y-as is alleen relevant voor staartmeting: zero_y=0\u00b0; up_y=90\u00b0; down_y=270\u00b0
#define ref_z 400 // referentiewaarde Y-as is alleen relevant voor staartmeting: zero_y=0\u00b0; up_y=90\u00b0; down_y=270\u00b0
#define factor 0.9 //een hoekverdraaiing van 90 graden komt overeen met 100 eenheden dus factor=90/100=0.9
#define angle_div 50

#include "WProgram.h"
void setup();
void PhoneInit();
void Message();
void loop();
double value_x=0;
double value_y=0;
double value_z=0;

double angle_x=0;
double angle_y=0;
double angle_z=0;

double angle_x_old=0;
double angle_y_old=0;
double angle_z_old=0;

int RXpin = 2;
int TXpin = 3;

double test=0;
//Bluetooth_HC05 hc05;
NewSoftSerial mySerial(RXpin,TXpin);


void setup()
{
 setTime(00,00,00,01,01,13); // set time to noon Jan 1 2013
 Serial.begin(38400);
 mySerial.begin(38400);
 PhoneInit();
} 
 
void PhoneInit()
{
 
  /* Speed: 38400; HC-05 RESET: pin 2; HC-05 MODE (PIO11): pin 3 */
  //hc05.begin(38400, 2, 3, HC05_MODE_COMMAND);
  //hc05.hardReset();
  /* Wait until HC-05 starts */
  //delay(700);
  /* Allow HC-05 to initiate connections */
  //hc05.setRole(HC05_ROLE_SLAVE);
  //hc05.setDeviceClass(40, HC05_DEFAULT_TIMEOUT);
  /* Cannot connect without this */
  //hc05.initSerialPortProfile();
  /* It's ridiculous: HC-05 cannot connect to anything without inquiring -
     no matter will it found any device or not!
     Iquiring may take pretty much time, thus 10 seconds of timeout. */
  
  //hc05.inquire(NULL, 60000);

  /* Slave module says "+ADDR:11:4:290255" on "AT+ADDR?" command */
  //BluetoothAddress slave = { 0x00, 0x11, 0x04, 0x29, 0x02, 0x55 };
  //hc05.connect(slave);

  
   
 Serial.println("AT+RESET"); delay(500);
 mySerial.println("AT+RESET"); delay(500);
 Serial.println("AT+INIT"); delay(500);
 mySerial.println("AT+INIT"); delay(500);
 //Serial.println("AT+ORGL"); delay(1000);
 //mySerial.println("AT+ORGL"); delay(1000);
 //Serial.println("AT+NAME=BTACC01"); delay(1000);
 //mySerial.println("AT+NAME=BTACC01"); delay(1000);
 Serial.println("AT+CLASS=40"); delay(500);
 mySerial.println("AT+CLASS=40"); delay(500);
 //Serial.println("AT+ENSNIFF=0012,06,071015"); delay(500); //2e BT-device name HC06
 //mySerial.println("AT+ENSNIFF=0012,06,071015"); delay(500); //2e BT-device name HC06
 Serial.println("AT+INIT"); delay(500);
 mySerial.println("AT+INIT"); delay(500);
 
 Serial.println("AT+INQ"); delay(500);
 mySerial.println("AT+INQ"); delay(500);
/*
 Serial.println("AT+PAIR=7445,8a,4d7fea,30"); delay(500); //pair direct met samasung GT-S5300 phone
 mySerial.println("AT+PAIR=7445,8a,4d7fea,30"); delay(500); //pair direct met samasung GT-S5300 phone
 Serial.println("AT+LINK=7445,8a,4d7fea"); delay(500); //pair direct met samasung GT-S5300 phone
 mySerial.println("AT+LINK=7445,8a,4d7fea"); delay(500); //pair direct met samasung GT-S5300 phone
*/

 //Serial.println("AT+PAIR=9c02,98,86b12e"); delay(500); //pair direct met samasung GT-P3110 tablet
 //mySerial.println("AT+PAIR=9c02,98,86b12e"); delay(500); //pair direct met samasung GT-P3110 tablet
 //Serial.println("AT+LINK=9c02,98,86b12e"); delay(500); //pair direct met samasung GT-P3110 tablet
 //mySerial.println("AT+LINK=9c02,98,86b12e"); delay(500); //pair direct met samasung GT-P3110 tablet
 
 //Serial.println("AT+INQ"); delay(30000);
 //Serial.println("AT+INQM=1,9,48"); delay(500);
 //Serial.println("AT+INQ"); delay(500);
 
 
 //Serial.println("AT+ENSNIFF=0011,07,190012"); delay(500); //1e BT-device name TEST
 //Serial.println("AT+ENSNIFF=0012,06,071015"); delay(500); //2e BT-device name HC06
 //Serial.println("AT+PAIR=7445,8a,4d7fea,30"); delay(500); //pair direct met samasung GT-S5300 phone
 //Serial.println("AT+LINK=7445,8a,4d7fea"); delay(500); //pair direct met samasung GT-S5300 phone
 //Serial.println("AT+LINK=15, 83, F128CC"); delay(500); // link direct met samsung SGH-160 phone
}  

void Message()
{
 mySerial.print(hour());
 mySerial.print(":");
 mySerial.print(minute());
 mySerial.print(":");
 mySerial.print(second());   
 mySerial.print(":");

 mySerial.print((int)angle_x);
 //Serial.print("\t");
 //Serial.print(angle_x_old);
 mySerial.print(":");
 mySerial.print((int)angle_y);
 //Serial.print("\t");
 //Serial.print(angle_y_old);
 mySerial.print(":");
 mySerial.println((int)angle_z);

}   
 
void loop() 
{
 value_x = analogRead(1);  // ATmega8L_3v3 bl/rd(gr) ADC3 pin 26
 value_y = analogRead(2);  // ATmega8L_3v3 gl/gr(rd) ADC3 pin 25
 value_z = analogRead(3);  // ATmega8L_3v3 rz/gy(gl) ADC3 pin 24
 
 angle_x=90+(abs(value_x-ref_x)*factor); //een hoekverdraaiing van 90 graden komt overeen met 100 eenheden dus factor=90/100=0.9
 angle_y=90+(abs(value_y-ref_y)*factor); //een hoekverdraaiing van 90 graden komt overeen met 100 eenheden dus factor=90/100=0.9
 angle_z=90+(abs(value_z-ref_z)*factor); //een hoekverdraaiing van 90 graden komt overeen met 100 eenheden dus factor=90/100=0.9
 
 
 
 //Message();
}
 
/* 
 
 // Bij VCC = 3V3 geldt:
 // 0 = 385; 90 = 304; 270 = 460
 
 test
 Serial.print("X: ");
 Serial.print(value_x);
 Serial.print("\t");
 Serial.print(angle_x);
 Serial.print("\n");
 
 Serial.print("Y: ");
 Serial.print(value_y);
 Serial.print("\t");
 Serial.print(angle_y);
 Serial.println("\n");
 
 //Serial.print("Z: ");
 //Serial.print(value_z);
 //Serial.print("\t");
 //Serial.print(angle_z);
 //Serial.print("\n");
 
 delay(5000);
 

 
 
 if(abs(angle_x-angle_x_old)>100)
  {
   //Message();
   angle_x_old=angle_x;
  } 
  
 if(abs(angle_y-angle_y_old)>angle_div)
  {
   Message();
   angle_y_old=angle_y;
  } 
}  

 
// test=(abs(angle_y-angle_y_old));
 
// if(test>50)
//  {
//   Message();
//   angle_y_old=angle_y;
   
// if(abs(angle_z-angle_z_old)>20)
//  {
//   Message();
//   angle_z_old=angle_z;
//  }  
*/

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


//#include <stdio.h>
//#include <stdlib.h>

//communicatie met GSM Siemens MC60
//#include <NewSoftSerial.h> 
//#include <SSerial2Mobile.h>

#define rxPin 0
#define txPin 1
//#define ledPin 13

void SMS();

void setup()
{
  
  Serial.begin(9600);          // start serial communication at 9600bps

  Serial.print("AT+CMGF=0\r\n");  
  delay(300);  
  Serial.print("AT+CMGS=18\r\n"); 
  delay(300);
  Serial.print("07911326060032F011000B911346992871F60004AA044B616C66"); 
  delay(300);
  Serial.print(0X1A,BYTE);
  Serial.print("\r\n");
  delay(300);
}

void loop(){}




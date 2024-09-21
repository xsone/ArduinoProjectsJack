// I2C SRF10 or SRF08 Devantech Ultrasonic Ranger Finder 
// by Nicholas Zambetti <http://www.zambetti.com>
// and James Tichenor <http://www.jamestichenor.net> 

// Demonstrates use of the Wire library reading data from the 
// Devantech Utrasonic Rangers SFR08 and SFR10

// Created 29 April 2006

//#include <stdio.h>
//#include <stdlib.h>

//LCD display 2 regels
#include <LCD4Bit_mod.h> 
LCD4Bit_mod lcd = LCD4Bit_mod(2); 

//I2C lib
#include <Wire.h>

#define rxPin 0
#define txPin 1
//#define ledPin 13

//variabelen (SRF02, I2C sensor)
int reading = 0;
char readingCharWaarde[5] = "0";
unsigned int AlarmIntWaarde = 25;
char AlarmCharWaarde[5];

//Keyboard waarden
/*
char msgs[5][15] = {"Right Key OK ", 
                    "Up Key OK    ", 
                    "Down Key OK  ", 
                    "Left Key OK  ", 
                    "Select Key OK" };
*/
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

int SensorRead();
int i=0;
int SMS();

void setup()
{
  Wire.begin();                // join i2c bus (address optional for master)
  Serial.begin(9600);          // start serial communication at 9600bps
 
  itoa(AlarmIntWaarde,AlarmCharWaarde,10);
  lcd.init();
  lcd.clear();
  lcd.printIn("SMS [cm] ");
  lcd.cursorTo(1,10);
  lcd.printIn(AlarmCharWaarde);
}




void loop()
{
 
  adc_key_in = analogRead(0);    // read the value from the sensor  
  digitalWrite(13, HIGH);  
  key = get_key(adc_key_in);		        // convert into key press
 
 // lcd.printIn(msgs[key]);
 //RIGHT delay(500);
  
  switch(key)
  {	
//   case 0: lcd.printIn("RIGHT"); break;
   case 1: AlarmIntWaarde++; itoa(AlarmIntWaarde,AlarmCharWaarde,10); lcd.cursorTo(1,10); lcd.printIn(AlarmCharWaarde); break;
   case 2: AlarmIntWaarde--; itoa(AlarmIntWaarde,AlarmCharWaarde,10); lcd.cursorTo(1,10); lcd.printIn(AlarmCharWaarde); break;
//   case 3: lcd.printIn("LEFT"); break;
  
  /*
   case 4: 
     {
       itoa(AlarmIntWaarde,AlarmCharWaarde,10);
       lcd.printIn("SMS[cm]: ");
       lcd.printIn(AlarmCharWaarde);
       switch(key)
         {
           lcd.cursorTo(2, 0);  //line=2, x=0
           lcd.printIn("SMS U/D/S: ");
           case 1: AlarmIntWaarde++; itoa(AlarmIntWaarde,AlarmCharWaarde,10); lcd.cursorTo(2,11); lcd.printIn(AlarmCharWaarde); break;
           case 2: AlarmIntWaarde--; itoa(AlarmIntWaarde,AlarmCharWaarde,10); lcd.cursorTo(2,11); lcd.printIn(AlarmCharWaarde); break;
           case 4: lcd.cursorTo(2,11); lcd.printIn(AlarmCharWaarde); break;
           default: break;
         }  
     }   
   */  
   
   default: break;
  }
 //SensorRead();
 if (SensorRead()<AlarmIntWaarde) SMS();
 delay(1000);
}


int SensorRead()
{
  // step 1: instruct sensor to read echoes
  Wire.beginTransmission(112); // transmit to device #112 (0x70)
                               // the address specified in the datasheet is 224 (0xE0)
                               // but i2c adressing uses the high 7 bits so it's 112
 
  Wire.send(0x00);             // sets register pointer to the command register (0x00)  
  Wire.send(0x51);             // command sensor to measure in "inches" (0x50) 
                               // use 0x51 for centimeters
                               // use 0x52 for ping microseconds
  Wire.endTransmission();      // stop transmitting

  // step 2: wait for readings to happen
  delay(70);                   // datasheet suggests at least 65 milliseconds

  // step 3: instruct sensor to return a particular echo reading
  Wire.beginTransmission(112); // transmit to device #112
  Wire.send(0x02);             // sets register pointer to echo #1 register (0x02)
  Wire.endTransmission();      // stop transmitting

  // step 4: request reading from sensor
  Wire.requestFrom(112, 2);    // request 2 bytes from slave device #112



  // step 5: receive reading from sensor
  if(2 <= Wire.available())    // if two bytes were received
  {
    reading = Wire.receive();  // receive high byte (overwrites previous reading)
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.receive(); // receive low byte as lower 8 bits
    Serial.println(reading);   // print the reading
    lcd.cursorTo(2, 0);  //line=2, x=0
    lcd.printIn("     ");
    itoa(reading,readingCharWaarde,10);
    lcd.cursorTo(2,0);
    lcd.printIn(readingCharWaarde);
   }
 return reading;
}


int SMS()
{
  if(i==0)
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
      i=1;
      return(1);
   }
  else i=1;
}


// Convert ADC value to key number
int get_key(unsigned int input)
{
	int k;
    
	for (k = 0; k < NUM_KEYS; k++)
	{
		if (input < adc_key_val[k])
		{
           
    return k;
        }
	}
    
    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed
    
    return k;
}

/*
 * Write Example
 *
 * This sketch creates a new file and writes 100 lines to the file.
 * No error checks on write in this example.
 */
 
#include <Fat16.h>
#include <Fat16util.h> // use functions to print strings from flash memory

#include "WProgram.h"
void error_P(const char *str);
void writeNumber(uint32_t n);
void setup(void);
void loop();
SdCard card;
Fat16 file;

int hour = 0;
int minu = 0;
int sec = 0;
int tel = 0;

int n = 0;
int p = 0;
int sensor = 0;
volatile int state = LOW;

// store error strings in flash to save RAM
#define error(s) error_P(PSTR(s))
void error_P(const char *str)
{
  PgmPrint("error: ");
  SerialPrintln_P(str);
  if (card.errorCode) {
    PgmPrint("SD error: ");
    Serial.println(card.errorCode, HEX);
  }
  while(1);
}

/*
 * Write an unsigned number to file
 */
void writeNumber(uint32_t n)
{
  uint8_t buf[10];
  uint8_t i = 0;
  do {
    i++;
    buf[sizeof(buf) - i] = n%10 + '0';
    n /= 10;
  } while (n);
  file.write(&buf[sizeof(buf) - i], i); // write the part of buf with the number
}

void setup(void)
{
  Serial.begin(9600);
 
  pinMode(2, INPUT);    //sensorinput
  pinMode(3, OUTPUT);   //test LED groen = sensor active
  pinMode(5, OUTPUT);   //test LED rood = on/off
  pinMode(10, OUTPUT);  //line for SD-card

  // initialize the SD card
  if (!card.init()) error("card.init");
  
  // initialize a FAT16 volume
  if (!Fat16::init(card)) error("Fat16::init");

  // create a new file
  char name[] = "Koe.TXT";
  for (uint8_t i = 0; i < 100; i++) {
    name[5] = i/10 + '0';
    name[6] = i%10 + '0';
    // O_CREAT - create the file if it does not exist
    // O_EXCL - fail if the file exists
    // O_WRITE - open for write
    if (file.open(name, O_CREAT | O_EXCL | O_WRITE)) break;
  }
  if (!file.isOpen()) error ("file.open");
  PgmPrint("Writing to: ");
  Serial.println(name);
  
  digitalWrite(5, HIGH); //test start meting/logging
  delay(1000);
  digitalWrite(5, LOW);

while(digitalRead(4)!=HIGH)
  {
    delay(1000);    // secondenteller 1000ms
    sec = sec + 1;  // basic conditionals
    if (sec > 59) {
     sec = 0;
     minu = minu + 1;
    } 
    if (minu > 59) {
     minu = 0;
     hour = hour + 1;
    }
    if (hour > 23) {
     hour = 0;
    }
   
    if(digitalRead(2)==HIGH) {  //sensorinput 2
      tel++;
      if (tel>10){
        digitalWrite(3,HIGH);   //LED om sensoractief te laten zien
        delay(500);
        digitalWrite(3,LOW);
        file.print(hour);      //printen naar SD-card
        file.print(":");  
        file.print(minu);
        file.print(":");    
        file.print(sec);
        file.println("");
       
        Serial.print(hour);   //test via serialline
        Serial.print(":");  
        Serial.print(minu);
        Serial.print(":");    
        Serial.print(sec);
        Serial.println("");
        tel=0;
      }
    } //einde timerlus
} //einde setup
file.close();
PgmPrintln("Done");       //SD schrijf actie
digitalWrite(5, HIGH);    //test met LED of data goed is weggeschreven
delay(1000);
digitalWrite(5, LOW);
return;
}

void loop()
{
}




int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


//Voltcrdaf KWh meter op zonnepanelen.

#include <LCD4Bit_mod.h>
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//#include <etherShield.h>
#include "WProgram.h"
void setup();
void ZonRead();
void loop();
LCD4Bit_mod lcd = LCD4Bit_mod(2);

volatile int state = LOW;
volatile unsigned long int ZonKWhWaarde = 0;
volatile unsigned long int ZonIntWaarde = 0;
int InteruptTellerZon = 0;
char ZonChar[10] = "0";
char ZonKWhChar[10] = "0";

void setup()
{
  lcd.init();
  lcd.clear();
  lcd.cursorTo(1,0);
  lcd.printIn("ZonPuls: ");
  //lcd.printIn(ZonChar);
  lcd.cursorTo(2,0);
  lcd.printIn("ZonKWh: ");
 // lcd.printIn(ZonKWhChar);
  
  Serial.begin(9600);			// voor debugging
  Serial.println("ZonneEnergyLogger Programma gestart...");
  //Serial.println(Ethernet.localIP());
  
  pinMode(13, OUTPUT);  //LED indicator KWh meter
  digitalWrite(13, LOW); //KWh meter
  delay(2000);
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);

  pinMode(3, INPUT);    //sensorinput Gas INT1
  digitalWrite(3, LOW);    //sensorinput Gas INT1
  attachInterrupt(4, ZonRead, FALLING);   //interrupt 4 digital pin 19 MEGA-board
}

// Uitlezen Kwhsensor  
void ZonRead()
{
 digitalWrite(13, HIGH);
 Serial.print("ZonPuls: ");
 Serial.println(ZonIntWaarde, DEC);
 itoa(ZonIntWaarde,ZonChar,10);
 
 
 digitalWrite(13, LOW);
 ZonIntWaarde++;


 if(ZonIntWaarde==1000) //1000 S0-pulsen is 1Kwh Zonneenergie
  {
   ZonKWhWaarde++;
   Serial.print("ZonKWhTeller:  ");
   Serial.println(ZonKWhWaarde, DEC);
   itoa(ZonKWhWaarde,ZonKWhChar,10);
  
   
  } 
 if(ZonIntWaarde==999999) ZonIntWaarde=0; //KWhmeter Max. waarde bereikt
 //digitalWrite(3, LOW);
}

void loop()
{
 lcd.cursorTo(1,10);
 lcd.printIn(ZonChar);
  lcd.cursorTo(2,10);
   lcd.printIn(ZonKWhChar);
 
}


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


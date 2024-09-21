//Sensor KY015 Arduino Sample Code

#include <Time.h>
#include <LCD4Bit_mod.h>
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//#include <etherShield.h>
LCD4Bit_mod lcd = LCD4Bit_mod(2);


char WaterChar[10] = "0";
unsigned int WaterIntWaarde = 0;
unsigned int InteruptTellerWater = 0;

/*
int ledPin = 13;



void setup()
{
  pinMode(ledPin, OUTPUT);    
  lcd.init();
  lcd.clear();
  lcd.cursorTo(1,0);
  lcd.printIn("Intr: ");
  Serial.begin(9600);
  attachInterrupt(2, WaterRead, CHANGE); //interrupt digital pin 2
}
 



void loop()
{
}

void WaterRead()
{
  digitalWrite(ledPin, LOW);
  if(InteruptTellerWater==1)
    {
     WaterIntWaarde++;
     InteruptTellerWater=0;
     digitalWrite(ledPin, HIGH);
     itoa(WaterIntWaarde,WaterChar,10);
     lcd.cursorTo(1,6);
     lcd.printIn(WaterChar);
    }
   else InteruptTellerWater++;
}

*/


int ledPin = 13;
volatile int state = LOW;

void setup()
{
  pinMode(ledPin, OUTPUT);    
  lcd.init();
  lcd.clear();
  lcd.cursorTo(1,0);
  lcd.printIn("Intr: ");
  Serial.begin(9600);
  attachInterrupt(4, blink, CHANGE);  //Mega int.4 is pin19, andere interrupt blocked by LCD
}

void loop()
{
  digitalWrite(ledPin, state);
  itoa(WaterIntWaarde,WaterChar,10);
  lcd.cursorTo(1,6);
  lcd.printIn(WaterChar);
}

void blink()
{
  state = !state;
  WaterIntWaarde++;
  //itoa(WaterIntWaarde,WaterChar,10);
  //lcd.cursorTo(1,6);
  //lcd.printIn(WaterChar);
  
  //delay(100);
}


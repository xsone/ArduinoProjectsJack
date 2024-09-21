//Voltcrdaf KWh meter op zonnepanelen.

//#include <LCD4Bit_mod.h>
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//#include <etherShield.h>
//LCD4Bit_mod lcd = LCD4Bit_mod(2);

#include <SPI.h>
#include <LiquidCrystal.h>
#include <LCDKeypad.h>
LCDKeypad lcd;


volatile int state = 0;
volatile unsigned long int ZonKWhWaarde = 0;
volatile unsigned long int ZonIntWaarde = 0;
int InteruptTellerZon = LOW;
char ZonChar[10] = "0";
char ZonKWhChar[10] = "0";

unsigned long lastMillis = 0;
int pulseLength = 0;

void setup()
{
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ZonPuls: ");
  lcd.setCursor(0,1);
  lcd.print("ZonKWh : ");
  
  Serial.begin(9600);			// voor debugging
  Serial.println("ZonneEnergyLogger Programma gestart...");
  //Serial.println(Ethernet.localIP());
  
  pinMode(2, INPUT);    //sensorinput INT0
  digitalWrite(2, LOW); //KWh meter
  pinMode(3, INPUT_PULLUP);    //sensorinput INT1
  digitalWrite(3, LOW); //KWh meter

  
  pinMode(13, OUTPUT);  //LED indicator KWh meter
  digitalWrite(13, LOW); //KWh meter
  delay(2000);
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);

  //pinMode(3, INPUT);    //sensorinput Gas INT1
  //digitalWrite(3, LOW);    //sensorinput Gas INT1
  //attachInterrupt(4, ZonRead, FALLING);   //interrupt 4 digital pin 19 MEGA-board
  
  //attachInterrupt(1, ZonRead, RISING);   //interrupt 0 digital pin 2 Due-board
  attachInterrupt(1, ZonRead, CHANGE);   //interrupt 1 digital pin 3 Due-board
}

// Uitlezen Kwhsensor  
void ZonRead()
{
 pulseLength = millis() - lastMillis;
        //store the time for the next pulse
   lastMillis = millis();
  
  
 if(InteruptTellerZon == 1)
  {
   ZonIntWaarde++;
   InteruptTellerZon = 0;
   digitalWrite(13, HIGH);
   
   
        
  
   
   return;
  } 
 else 
  {
   InteruptTellerZon++;  
   digitalWrite(13, LOW);
   return;
  }
 digitalWrite(3, LOW);
// state = !state;
 //digitalWrite(13, state);
 //Serial.print("ZonPuls: ");
 //Serial.println(ZonIntWaarde, DEC);
 //digitalWrite(2, HIGH); //KWh meter
 //digitalWrite(3, HIGH); //KWh meter
}


void loop()
{
 lcd.setCursor(10,0);
 lcd.print(ZonIntWaarde);
 lcd.setCursor(10,1);
 //lcd.print(ZonKWhWaarde);
 lcd.print(pulseLength);
  
 //Serial.print("ZonPuls: ");
 //Serial.println(ZonIntWaarde, DEC);
 //digitalWrite(2, LOW); //KWh meter
 //digitalWrite(3, LOW); //KWh meter
 if(ZonIntWaarde==1000) ZonKWhWaarde++; //1000 S0-pulsen is 1Kwh Zonneenergie
 if(ZonIntWaarde==999999) ZonIntWaarde=0; //KWhmeter Max. waarde bereikt
 
 //digitalWrite(13, LOW);
 
 //Serial.print("ZonKWhTeller:  ");
 //Serial.println(ZonKWhWaarde, DEC);
}


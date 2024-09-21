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

int pulseLength = 0;
unsigned long lastMillis = 0;
boolean solarInput = false;
boolean sensorIsOn = false;

int S0pin = 3;


void setup()
{
  //lcd.begin(16,2);
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("ZonPuls: ");
  //lcd.setCursor(0,1);
  //lcd.print("ZonKWh : ");
  
  Serial.begin(9600);			// voor debugging
  Serial.println("ZonneEnergyLogger Programma gestart...");
  //Serial.println(Ethernet.localIP());
  
  //pinMode(2, INPUT);    //sensorinput INT0
  //digitalWrite(2, LOW); //KWh meter
  //pinMode(S0pin, INPUT);    //sensorinput INT1
  //digitalWrite(s0pin, LOW); //KWh meter

  pinMode(13, OUTPUT);  //LED indicator KWh meter
  digitalWrite(13, LOW); //KWh meter
  delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);

  pinMode(S0pin, INPUT_PULLUP);    //sensorinput Gas INT1
  sensorIsOn = false; 
  //digitalWrite(3, LOW);    //sensorinput Gas INT1
  //attachInterrupt(4, ZonRead, FALLING);   //interrupt 4 digital pin 19 MEGA-board
  //attachInterrupt(1, ZonRead, CHANGE);   //interrupt 1 digital pin 3 Due-board
}

void loop()
{
  //lcd.setCursor(10,0);
  //lcd.print(ZonIntWaarde);
  //lcd.setCursor(10,1);
  //lcd.print(ZonKWhWaarde);  
  
  ZonRead();
  if(ZonIntWaarde==1000) ZonKWhWaarde++; //1000 S0-pulsen is 1Kwh Zonneenergie
  if(ZonIntWaarde==999999) ZonIntWaarde=0; //KWhmeter Max. waarde bereikt
}  
  
void ZonRead()
{ 
  // read the digital input
    solarInput = digitalRead(S0pin);
    // rising edge?
    if(solarInput && sensorIsOn == false)
    {
        sensorIsOn=true;
        
    }
    // falling edge?
    if(!solarInput && sensorIsOn == true)
    {
        /*
        // store the time between the last two pulses
        pulseLength = millis() - lastMillis;
        //store the time for the next pulse
        lastMillis = millis();
        
        //lcd.setCursor(10,1);
        //lcd.print(pulseLength);  
        Serial.print("Lastmillis: ");
        Serial.println(lastMillis, DEC); 
        Serial.print("Pulslength: ");
        Serial.println(pulseLength, DEC); 
        */

        // update counters
        ZonIntWaarde++;
        sensorIsOn = false;
        Serial.print("ZonPuls: ");
        Serial.println(ZonIntWaarde, DEC);
        Serial.print("ZonKWh: ");
        Serial.println(ZonKWhWaarde, DEC); 
        
    }  
  
        //pulseLength = 0;
} 


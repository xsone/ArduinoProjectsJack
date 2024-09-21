
/*
 * Web Server
 *
 * A simple web server that shows the value of the analog input pins.
 */

#include <LCD4Bit_mod.h>
//#include <LiquidCrystal.h>
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//#include <etherShield.h>
LCD4Bit_mod lcd = LCD4Bit_mod(2);

//Keyboard waarden
char msgs[5][15] = {"Right Key OK ", 
                    "Up Key OK    ", 
                    "Down Key OK  ", 
                    "Left Key OK  ", 
                    "Select Key OK" };
int  adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

int sensorPin = 4;    // select the input pin for the potentiometer
int ledPin = 13;     // select the pin for the LED
unsigned int SensorValue = 0;  // variable to store the value coming from the sensor
int zwartwaarde = 0;

// Sensor reading
//volatile long int GasIntWaarde = 0;
char GasCharWaarde[10] = "0";
char GasM3CharWaarde[10] = "0";
//volatile long int WaterIntWaarde = 0;
char WaterCharWaarde[10] = "0";
//volatile long int ElecIntWaarde = 0;
char ElecCharWaarde[10] = "0";
volatile int state = HIGH;
//int InteruptTellerGas = 0;
//int InteruptTellerWater = 0;

double lastValueSent = 100; 
double currentValue = 100;

// Sensor reading
volatile long int GasIntWaarde=0;
volatile long int GasM3Waarde=0;
volatile long int WaterIntWaarde=0;
volatile long int WaterM3Waarde=0;
volatile long int ElecKwhWaarde = 0;

int ElecIntWaarde = 0;
int ElecSensorValue = 0;
int ZwartWaarde=0; //detectie zwarte vlakje KWh
int InteruptTellerGas = 0;
int InteruptTellerWater = 0;


// RTC
int hour = 0;
int minu = 0;
int sec = 0;
int tel = 0;

// Uitlezen keyboard
int get_key(unsigned int input)
{
  int k;
  
  for (k = 0; k < NUM_KEYS; k++)
    {
     if (input < adc_key_val[k]) return k;
    }
     if (k >= NUM_KEYS) k = -1; return k;
}


void UpdateWaarden()
{
  switch (key)
   {
     //lcd.blink();
     case 1: ltoa(GasIntWaarde++,GasCharWaarde,10); break;
     case 2: ltoa(GasIntWaarde--,GasCharWaarde,10); break;
     //lcd.noBlink();
   }
}

// Uitlezen Watersensor
void WaterRead()
{
  digitalWrite(4, HIGH);
  if(InteruptTellerWater>2)
    {
     WaterIntWaarde++;
     InteruptTellerWater=0;
     digitalWrite(4, LOW);
     if(WaterIntWaarde>1000) //1000 pulsen = 1M3 water
      {
       WaterM3Waarde++;
       WaterIntWaarde=0;
      } 
    }
   else InteruptTellerWater++;
}

// Uitlezen Gassensor  
void GasRead()
{
  digitalWrite(ledPin, LOW);
  if(InteruptTellerGas==2)
   {
     GasIntWaarde++;
      digitalWrite(ledPin, HIGH);
      
     InteruptTellerGas=0;
     if(GasIntWaarde==10) //1000 pulsen = 1M3 gas
      {
       GasM3Waarde++;
       GasIntWaarde=0;
      } 
    }
  else InteruptTellerGas++;
}

void ElecRead()
{
 ElecSensorValue=analogRead(sensorPin);
 if(ElecSensorValue>850&&ElecSensorValue<950)
  {
   Serial.print("SPIEGEL: ");
   Serial.println(ElecSensorValue);
   digitalWrite(ledPin,HIGH);
   if(zwartwaarde==1)
    {
     itoa(ElecIntWaarde++,ElecCharWaarde,10);
     zwartwaarde=0;
    }
  }
 else 
  {
   digitalWrite(ledPin, LOW);
   zwartwaarde=1;
  }
 //if(ElecIntWaarde>600) //600 omwentelingen = 1kWh
 // {
 //  ElecKwhWaarde++;
 //  ElecIntWaarde=0;
 // }
}



void setup()
{
 Serial.begin(9600);
 pinMode(6, OUTPUT);  
 pinMode(5, OUTPUT);  
 pinMode(4, OUTPUT);  
 digitalWrite(6, LOW); //KWh meter
 digitalWrite(5, LOW); //Gas meter
 digitalWrite(4, LOW); //Water meter
  
 pinMode(2, INPUT);    //sensorinput Water INT0
 pinMode(3, INPUT);    //sensorinput Gas INT1
 pinMode(ledPin, OUTPUT);  
 attachInterrupt(0, WaterRead, CHANGE); //interrupt digital pin 2
 attachInterrupt(1, GasRead, CHANGE);   //interrupt digital pin 3

 lcd.init();
 lcd.clear();
 lcd.cursorTo(1,0);
 lcd.printIn("Gas: ");
 lcd.cursorTo(2,0);
 lcd.printIn("M3 : ");
}

//  while(key!=4)



void loop()
  {	
   
   
   itoa(GasIntWaarde,GasCharWaarde,10);
   lcd.cursorTo(1,6);
   lcd.printIn(GasCharWaarde);
   
   itoa(GasM3Waarde,GasM3CharWaarde,10);
   lcd.cursorTo(2,6);
   lcd.printIn(GasM3CharWaarde);
   
   //adc_key_in = analogRead(0);    // read the value from the sensor  
   //key = get_key(adc_key_in); 

   //ElecRead();
   
   

   

/*  
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
  */   
     
  
  /*   
     //Serial.print(hour);   //test via serialline
     //Serial.print(":");  
     //Serial.print(minu);
     //Serial.print(":");    
     //Serial.print(sec);
     //Serial.print(">");
     //Serial.print("Sen: ");
     //Serial.print(GasCharWaarde);
     //Serial.println("");
  switch(key)
   {
     case 1: ltoa(GasIntWaarde++,GasCharWaarde,10); lcd.cursorTo(1,8); lcd.printIn(GasCharWaarde); break;               break;
     case 2: ltoa(GasIntWaarde--,GasCharWaarde,10); lcd.cursorTo(1,8); lcd.printIn(GasCharWaarde); break;
     //case 4: WriteSDcard(); break;
     case 0: UpdateWaarden(); break;
   }
   */
} 





// Spectrometer_led10
// clear the LCD
//void clearLCD() {
//  Serial.print(12, BYTE);
//  delay(1);
//}
// start a new line
//void newLine() {
//  Serial.print(10, BYTE);
//}  

#include <EEPROM.h>
//#include <EEPROMMex.h>
//#include <EEPROMVar.h>


// variable declaration
int inPinLeer = 2; // black button on pin 2
int inPinHerken = 3; // red button on pin 3

int LeerState; // is red button pressed
int HerkenState; // is black button pressed
int val = 0;
int analogPin = 1; // sensor output connected to Analog Pin 1 hier komt een photo transistor aan te zitten
unsigned int sensorValue = 0; // variable to store the ADC value of the photodiode

char inChar;
String Stof;

int ir = 8;  // IR led on output pin 8
int red = 9; // red led on output pin 9
int orange = 10; // orange led on output pin 10
int yellow = 11; // yellow led on output pin 11
int blue = 12; // blue led on output pin 12

int pinArray[] = {8, 9, 10, 11, 12}; // pin address array
int count = 0; // for next loop counter
int sampleValue[5]; // sample value array

// standaard waarden diverse stoffen
int niks[] = {965,987,937,990,992}; 
int niksMarge = 5;

int plastic[] = {948,988,938,992,992}; 
int plasticMarge = 5;

int papier[] = {1003,1018,1020,1021,1021}; 
int papierMarge = 5;

String Sample = "";
String oldSensorValue = "START: ";

// initialize settings
void setup() 
{  
 Serial.begin(9600);//set baud rate
 pinMode(inPinLeer, INPUT); // set Leer switch pin als input
 pinMode(inPinHerken, INPUT); // set Herken switch pin als input
 
 pinMode(ir, OUTPUT); // set IR led pin as output
 pinMode(red, OUTPUT); // set red led pin as output
 pinMode(orange, OUTPUT); // set orange led pin as output
 pinMode(yellow, OUTPUT); // set yellow led pin as output
 pinMode(blue, OUTPUT); // set blue led pin as output

 // set pins voor output
 for (count=0 ;count < 5; count++) 
  {
    pinMode(pinArray[count], OUTPUT);
   }

 // Print startup text
 delay(100);
 Serial.println("Spectrometer; (c) Jack; Rev. 1.20 june 2015");
 delay(3000); // keep the welcome screen on for 3 seconds
 digitalWrite(inPinLeer, HIGH); 
 digitalWrite(inPinHerken, HIGH); 
}


// program starts here
void loop() 
{
 Serial.println("...Leer Mode (pin 2).");
 Serial.println("...Herken Mode (pin 3)...");
 delay(50);
 Serial.println("...Start...");
 Serial.println();

 // wait button press
 for (;;)
  {  
   /*
   LeerState = digitalRead(inPinLeer); // test for red button press
   HerkenState = digitalRead(inPinHerken); // test for black button press  
      
   if (HerkenState == LOW) Herken(); // red button pressed
   else digitalWrite(inPinHerken, HIGH); 
   if (LeerState == LOW) Leer();  // black button pressed goto learn; // jump to learn subroutine    
   else digitalWrite(inPinLeer, HIGH); 
   */ 
   
   
   if (Serial.available() > 0)
    {
     inChar = Serial.read();
     if (inChar == 'l') Leer();
     if (inChar == 'h') Herken();
     if (inChar == 's') Opslaan();
     if (inChar == 'r') Lees();
    } 
  } 
}

//Opslaan in EEPROM, bij Arduino UNO is dit slechts 1 kB
void Opslaan()
{
 for (count=0; count < 5 ;count++) 
  { 
   digitalWrite(pinArray[count], HIGH);  //turn the led on
   delay(100); // wait for stabilization
   sensorValue = analogRead(analogPin);  // read the value from the sensor
   Sample = ( Sample + '|' + String(sensorValue) );
   //oldSensorValue = String(sensorValue);
   //EEPROM.writeInt(count, sensorValue);
   //EEPROM_writeAnything(count, sensorValue);
   Serial.print(sensorValue); Serial.print("-");  //send as a ascii encoded number
    if (count == 0) Serial.print("IR   ");
    if (count == 1) Serial.print("Red   ");
    if (count == 2) Serial.print("Orange    ");
    if (count == 3) Serial.print("Yellow  ");
    if (count == 4) Serial.print("Blue "); 
   digitalWrite(pinArray[count], LOW); // turn off led
  }
  Serial.println();
  Serial.println("Aan welke stof toekennen?...(Sluit af met #)");
 
   do {
    if (Serial.available() > 0) {
      //Serial.println("s");
      inChar = Serial.read();
      if ( inChar == '\n' || inChar == '\r' || inChar== '#' ) break;
      Stof += inChar;
    }
  }while(inChar != '\n' && inChar!= '\r');    
  
  Sample = (Sample + '=' + Stof); 
  
  for (int i=0; i < sizeof(Sample); i++)
   {
    EEPROM.write(0, Sample[i]);
   } 
  Serial.println(Sample);
  Serial.println("...Einde Opslaan...");
  Serial.println();
  
  
  //for (int i=0; i < sizeof(Sample); i++)
  // {
    Serial.println( EEPROM.read(0) );
  // } 
  
  
  //String Test = EEPROM.read(0);
  //Serial.println(Test);
  Serial.println();
  Sample = "";
  Stof = "";
}




//Teruglezen uit EEPROM  
void Lees()
{
  Serial.println("...Teruglezen uit EEPROM...");
  for (count=0; count < 5 ;count++) 
  { 
    unsigned int value; //= EEPROM.readInt(count);
    //unsigned int value = EEPROM.readAnything(count);
    Serial.print(count);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.println();
  }
  Serial.println("...Teruglezen EEPROM...");
  Serial.println();
}

// Leer functie
void Leer()
 {
  Serial.println("...Leer Mode...");
  
  for (count=0; count < 5 ;count++) 
  { 
   digitalWrite(pinArray[count], HIGH);  //turn the led on
   delay(100); // wait for stabilization
   sensorValue = analogRead(analogPin);  // read the value from the sensor 
   Serial.print(sensorValue); Serial.print("-");  //send as a ascii encoded number
   delay(30);
   if (count == 0) Serial.print("IR   ");
   if (count == 1) Serial.print("Red   ");
   if (count == 2) Serial.print("Orange    ");
   if (count == 3) Serial.print("Yellow  ");
   if (count == 4) Serial.print("Blue ");
   digitalWrite(pinArray[count], LOW); // turn off led
  }
  Serial.println();
  Serial.println("...Einde Learn...");
  Serial.println();
 }
 
// Herken functie
void Herken()
{
 Serial.println("...Herken Mode...");
 for (count=0; count < 5 ;count++) 
  { 
   digitalWrite(pinArray[count], HIGH); //turn on led
   delay(100); // wait for led to stabilize
   sensorValue = analogRead(analogPin);  // read the value from the sensor
   sampleValue[count] = sensorValue; // store value in matrix
   digitalWrite(pinArray[count], LOW); // turn off led
   
   if ( (sampleValue[count] >= (plastic[count] - plasticMarge)) && (sampleValue[count] <= (plastic[count] + plasticMarge)) ) { Serial.println("plastic Sample  ");  break; } 
   if ( (sampleValue[count] >= (niks[count] - niksMarge)) && (sampleValue[count] <= (niks[count] + niksMarge)) ) { Serial.println("Niks Sample  ");  break; } 
   if ( (sampleValue[count] >= (papier[count] - papierMarge)) && (sampleValue[count] <= (papier[count] + papierMarge)) ) { Serial.println("Papier Sample  ");  break; } 
 
   digitalWrite(pinArray[count], LOW); // turn off led
  }
 Serial.println();
 Serial.println("...Einde Herken...");
 Serial.println();
}

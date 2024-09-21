//////////////////////////////////////////////////////////////////
//(c) juni 2015
//
// Bootloader geladen in een ATMEGA8-16PU
// Met een DIAMEX-AVR
// Let op COM-port kleiner dan COM10 anders wert deze nie
// Kies voor Arduino IDE versie 0017
// Kies voor bootlodel ATmega8-noxtal
// Kies voor AVRISP mkII
//
// Werkt met een BlueTooth initialisatie
// Werkt met echte graden hoeken X,Y,Z
// Simple code for the ADXL335, prints calculated orientation via serial
//////////////////////////////////////////////////////////////////

#define rxPin 4 //was 2 (4 op shield ivm conflict met sensorinput; geldt ook voor ATMega83_3v3 NoXtal)
#define txPin 5 //was 3 (5 op shield ivm conflict)
 
//Analog read pins
#include "WProgram.h"
void setup();
void PhoneInit();
void loop();
const int xPin = 3; // ATmega8L_3v3 bl/rd(gr) ADC3 pin 26
const int yPin = 2; // ATmega8L_3v3 bl/rd(gr) ADC3 pin 25
const int zPin = 1; // ATmega8L_3v3 bl/rd(gr) ADC3 pin 24
 
//The minimum and maximum values that came from the accelerometer while standing still
//You very well may need to change these
//int minVal = 265; oorspr.
//int maxVal = 402; oorspr.

unsigned const int minVal = 300;
unsigned const int maxVal = 455;

// Initialize 
void setup()
{
 Serial.begin(38400); // 38400 voor BT communication
 Serial.println("------->  Start Sending AT");
 digitalWrite(19, HIGH);
 PhoneInit();
}


// Initialisatie BT
void PhoneInit()
{
 Serial.println("AT+RESET"); delay(500);
    // readline();
 Serial.println("AT+INIT"); delay(500);
    // readline();
 Serial.println("AT+NAME=\"ACC-BT02\""); delay(500);
    // readline();
 Serial.println("AT+CLASS=0040"); delay(500);
    // readline();
 Serial.println("AT+ENSNIFF=0012,06,071015"); delay(500); //2e BT-device name HC06
 Serial.println("AT+INQ"); delay(5000);
}  

 
void loop()
{
  //read the analog values from the accelerometer
  int xRead = analogRead(xPin);
  int yRead = analogRead(yPin);
  int zRead = analogRead(zPin);
 
  //bereken de hoek in graden bereik van 0 tot 360 was 180
  int xAng = map(xRead, minVal, maxVal, 0, 180);
   if (xAng < 0) xAng = 0;
  int yAng = map(yRead, minVal, maxVal, 0, 180);
   if (yAng < 0) yAng = 0;
  int zAng = map(zRead, minVal, maxVal, 0, 180);
   if (zAng < 0) zAng = 0;
 
  //stuur waarden naar output 
  Serial.print(xAng); 
  Serial.print(":"); 
  Serial.print(yAng); 
  Serial.print(":"); 
  Serial.println(zAng); 
 
  delay(500);//just here to slow down the serial output - Easier to read
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


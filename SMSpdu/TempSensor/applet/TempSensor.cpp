//Senosr KY001, Temperature Sensor Module o.b.v. DS18B20
//Connect the signal to Arduino digital 10, and open serial port monitor:

#include <OneWire.h>
#include <Wire.h>
 
/* DS18S20 Temperature chip i/o
 
 */
 
#include "WProgram.h"
void setup(void);
void loop(void);
void setup();
void loop();
void I2C();
OneWire ds(10); // on pin 10
 
void setup(void) {
 // initialize inputs/outputs
 // start serial port
 Serial.begin(9600);
 Wire.begin(); // join i2c bus (address optional for master)
}
 
 
 
void loop(void) {
 byte i;
 byte present = 0;
 byte data[12];
 byte addr[8];
 int Temp;

// I2C();


 if ( !ds.search(addr)) {
      //Serial.print("No more addresses.n");
      ds.reset_search();
      return;
 }
 
 Serial.print("R="); //R=28 Not sure what this is
 for( i = 0; i < 8; i++) {
   Serial.print(addr[i], HEX);
   Serial.print(" ");
 }
 
 if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!n");
      return;
 }
 
 if ( addr[0] != 0x28) {
      Serial.print("Device is not a DS18S20 family device.n");
      return;
 }
 
 ds.reset();
 ds.select(addr);
 ds.write(0x44,1);        // start conversion, with parasite power on at the end
 
 delay(1000);    // maybe 750ms is enough, maybe not
 // we might do a ds.depower() here, but the reset will take care of it.
 
 present = ds.reset();
 ds.select(addr);
 ds.write(0xBE);   // Read Scratchpad
 
 Serial.print("P=");
 Serial.print(present,HEX);
 Serial.print(" ");
 for ( i = 0; i < 9; i++) {        // we need 9 bytes
   data[i] = ds.read();
   Serial.print(data[i], HEX);
   Serial.print(" ");
 }
 Temp=(data[1]<<8)+data[0];//take the two bytes from the response relating to temperature
 
 Temp=Temp>>4;//divide by 16 to get pure celcius readout
 
  //next line is Fahrenheit conversion
 //Temp=Temp*1.8+32; // comment this line out to get celcius
 
 Serial.print("T=");//output the temperature to serial port
 Serial.print(Temp);
   Serial.print(" ");
 
 
 Serial.print(" CRC=");
 Serial.print( OneWire::crc8( data, 8), HEX);
 Serial.println();
}
// Project 12 - Pulse Monitor Test Script

 

int ledPin = 13;

int sensorPin = 0;

 

double alpha = 0.75;

int period = 20;

double change = 0.0;

 

void setup()                   

{

  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

}

 

void loop()                    

{

    static double oldValue = 0;

    static double oldChange = 0;

    int rawValue = analogRead(sensorPin);

    double value = alpha * oldValue + (1 - alpha) * rawValue;

 

    Serial.print(rawValue);

    Serial.print(",");

    Serial.println(value);

 

    oldValue = value;

    delay(period);

}
// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this
// I2C: analog input pins A4 (SDA) and A5 (SCL) bij Duemilanove
// Bij aansluiten Johnson Controls board PCF8574 als volgt: A4 (SDA) naar pin 3 (SCL) en A5 (SCL) naar pin 2 (SDA)

// Created 29 March 2006

#include <Wire.h>

//void setup()
//{
//  Wire.begin(); // join i2c bus (address optional for master)
//}

int n = 0x00;

void I2C()
{
  for(n=0x00;n<=0xFF;n++)
  {
    //Wire.beginTransmission(38); // transmit to device (0x4E)slave address PCF8574 chip op relaispcb; writemode (00100110)
    Wire.beginTransmission(36); // transmit to device (0x4E)slave address PCF8574 chip op pcb XP9103; writemode (00100110)
    // Wire.beginTransmission(39); // transmit to device (0x4E)slave address PCF8574 chip; readmode (00100111)
    Wire.send(n);  /* aan */
    Wire.endTransmission();    // stop transmitting
    delay(500);
   }
 }


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


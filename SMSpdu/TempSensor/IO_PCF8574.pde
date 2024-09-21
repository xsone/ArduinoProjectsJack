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


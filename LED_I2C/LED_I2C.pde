// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(56); // transmit to device #76 (0x4C)
 // Wire.send("x is ");        // sends five bytes
 // Wire.send(x);
Wire.send(0x00);  /* aan */
Wire.send(0x1E);
//delay(200);
/*
Wire.send(0xEE);              // sends one byte  
Wire.send(0xFF);              // sends one byte  
Wire.send(0x00);              // sends one byte  
Wire.send(0xEE);              // sends one byte  
*/
Wire.endTransmission();    // stop transmitting

/*
Wire.requestFrom(38, 2);    // request 2 bytes from slave device #112
  // step 5: receive reading from XP
  if(2 <= Wire.available())    // if two bytes were received
  {
    reading = Wire.receive();  // receive high byte (overwrites previous reading)
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.receive(); // receive low byte as lower 8 bits
    Serial.println(reading);   // print the reading
  }
//x++;
delay(500);
*/
}

/*
SimpleSend
This sketch transmits a short text message using the VirtualWire library
connect the Transmitter data pin to Arduino pin 12

(c) nov. 2015 werkt uistekend met FS1000A transmitter op 5V

*/
#include <VirtualWire.h>

const int led_pin = 13;

void setup()
{
// Initialize the IO and ISR
vw_setup(2000); // Bits per sec
pinMode(led_pin, OUTPUT);
}
void loop()
{
send("Hallo Jack");
digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
delay(2000);
digitalWrite(led_pin, LOW);
}

void send (char *message)
{
vw_send((uint8_t *)message, strlen(message));
vw_wait_tx(); // Wait until the whole message is gone
}


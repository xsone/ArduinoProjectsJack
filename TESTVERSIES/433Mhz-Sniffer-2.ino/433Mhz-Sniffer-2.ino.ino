#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
RCSwitch mySwitch = RCSwitch();
void setup() {
 Serial.begin(9600);
 mySwitch.enableReceive(0);
}
void loop() {
 if (mySwitch.available()) {
 int value = mySwitch.getReceivedValue();
 if (value == 0) {
 Serial.print("Unknown encoding");
 } else {
 Serial.print("Received ");
 Serial.print( mySwitch.getReceivedValue() );
 Serial.print(" / ");
 Serial.print( mySwitch.getReceivedBitlength() );
 Serial.print("bit ");
 Serial.print("Protocol: ");
 Serial.println( mySwitch.getReceivedProtocol() );
 }
 mySwitch.resetAvailable();
 }
}

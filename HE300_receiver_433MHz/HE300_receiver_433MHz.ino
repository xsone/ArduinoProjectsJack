#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

const int TXpin = 10;

void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(TXpin);
  Serial.println("RCSwitch ready");
}

void loop() {
  //Turn ON HE300 
  mySwitch.send(1234,0,true);
  delay(2000);
  //Turn ON RCSwitch
  mySwitch.send(1234,24);
  delay(2000);
  //Turn OFF HE300 
  mySwitch.send(1234,0,false);
  delay(2000);
  //Turn OFF RCSwitch
  mySwitch.send(1233,24);
  delay(2000);
}

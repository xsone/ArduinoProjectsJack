/*
This code managed as much radio/ir code as it can!
 433Mhz : Generic code/ HomeEasy code / VirtualWire
 IR : Most IR codes

 Library needed:
 IRremote : https://github.com/shirriff/Arduino-IRremote
 433mhzforarduino : https://bitbucket.org/fuzzillogic/433mhzforarduino/wiki/Home
 VirtualWire: http://www.airspayce.com/mikem/arduino/VirtualWire/VirtualWire-1.27.zip

 Setup:
 - connect a 433MHz receiver on digital pin 2.
 - connect IR receiver on digital pin 3
 */

#include <RemoteReceiver.h>
#include <NewRemoteReceiver.h>
//#include <SensorReceiver.h>
#include <InterruptChain.h>
#include <VirtualWire.h>
//#include <IRremote.h>

//IRrecv irrecv(8);
//decode_results results;

void setup() {
  Serial.begin(115200);

  // Interrupt -1 to indicate you will call the interrupt handler with InterruptChain
  RemoteReceiver::init(-1, 2, showOldCode);

  // Again, interrupt -1 to indicate you will call the interrupt handler with InterruptChain
  NewRemoteReceiver::init(-1, 2, showNewCode);

  // And once more, interrupt -1 to indicate you will call the interrupt handler with InterruptChain
  //SensorReceiver::init(-1, showTempHumi);

  // Set interrupt mode CHANGE, instead of the default LOW.
  InterruptChain::setMode(0, CHANGE);

  // On interrupt, call the interrupt handlers of remote and sensor receivers
  InterruptChain::addInterruptCallback(0, RemoteReceiver::interruptHandler);
  InterruptChain::addInterruptCallback(0, NewRemoteReceiver::interruptHandler);
  //InterruptChain::addInterruptCallback(0, SensorReceiver::interruptHandler);
  
   //Receive
  vw_set_rx_pin(2);
  vw_setup(2000);
  vw_rx_start();

  
  // irrecv.enableIRIn(); // Start the receiver
  
}

void loop() {
  // You can do other stuff here!
   uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  char* radiocode;
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    buf[buflen] = '\0';
    radiocode = (char *)buf;
    String radiocode_string = "T:" + String(radiocode);
    Serial.println(radiocode_string);
  
  }
  
 // irsniffer();
}


/*
void irsniffer(){
   if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
  
    irrecv.resume(); // Receive the next value
  }
}
*/

// shows the received code sent from an old-style remote switch
void showOldCode(unsigned long receivedCode, unsigned int period) {
  // Print the received code.
  Serial.print("Code: ");
  Serial.print(receivedCode);
  Serial.print(", period: ");
  Serial.print(period);
  Serial.println("us.");
}

// Shows the received code sent from an new-style remote switch
void showNewCode(NewRemoteCode receivedCode) {
  // Print the received code.
  Serial.print("Addr ");
  Serial.print(receivedCode.address);

  if (receivedCode.groupBit) {
    Serial.print(" group");
  } 
  else {
    Serial.print(" unit ");
    Serial.print(receivedCode.unit);
  }

  switch (receivedCode.switchType) {
    case NewRemoteCode::off:
      Serial.print(" off");
      break;
    case NewRemoteCode::on:
      Serial.print(" on");
      break;
    case NewRemoteCode::dim:
      Serial.print(" dim");
      Serial.print(receivedCode.dimLevel);
      break;
  }

  if (receivedCode.dimLevelPresent) {
    Serial.print(", dim level ");
    Serial.print(receivedCode.dimLevel);
  }

  Serial.print(", period: ");
  Serial.print(receivedCode.period);
  Serial.println("us.");
}

// Shows the received sensor data
void showTempHumi(byte *data) {
  // is data a ThermoHygro-device?
  if ((data[3] & 0x1f) == 0x1e) {
    // Yes!
    byte channel, randomId;
    int temp;
    byte humidity;

    // Decode the data
   // SensorReceiver::decodeThermoHygro(data, channel, randomId, temp, humidity);

    // Print temperature. Note: temp is 10x the actual temperature!
    Serial.print("Temperature: ");
    Serial.print(temp / 10); // units
    Serial.print('.');
    Serial.println(temp % 10); // decimal
  }
}

/*
 * Demo for RF remote switch receiver.
 * For details, see NewRemoteReceiver.h!
 *
 * Connect the transmitter to digital pin 11, and the receiver to digital pin 2.
 *
 * When run, this sketch waits for a valid code from a new-style the receiver,
 * decodes it, and retransmits it after 5 seconds.
 */

#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>

const int ledPin = 13;// the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 2000;           // interval at which to blink (milliseconds)

const int receiverPin = 11;
unsigned long receivedCode_address = 1108871;
unsigned int receivedCode_period = 249;

NewRemoteTransmitter transmitter(receivedCode_address, receiverPin, receivedCode_period);

void setup() {
  Serial.begin(115200);

  // NewRemoteReceiver::disable();

  // Need interrupts for delay()
  interrupts();

  // Wait 5 seconds before sending.
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    transmitter.sendGroup(ledState);
  }
}

// Callback function is called only when a valid code is received.
void showCode(NewRemoteCode receivedCode) {
  // Note: interrupts are disabled. You can re-enable them if needed.

  // Print the received code.
  Serial.print("Addr ");
  Serial.print(receivedCode.address);

  switch (receivedCode.switchType) {
    case NewRemoteCode::off:
      Serial.print(" off");
      break;
    case NewRemoteCode::on:
      Serial.print(" on");
      break;
  }

  Serial.print(", period: ");
  Serial.print(receivedCode.period);
  Serial.println("us.");
}

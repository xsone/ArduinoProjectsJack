/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */
int incomingByte = 0;
String inputString = "";

void setup() {
  // initialize serial:
  Serial.begin(115200);
  Serial.println("Serial oke P1");
}

void loop() {
  // print the string when a newline arrives:
  while (Serial.available()) {
    // get the new byte:
    //Serial.write("test...");
    incomingByte = Serial.read();
    
    incomingByte &= ~(1 << 8);
    char inChar = (char)incomingByte;
    inputString += inChar;

    //Serial.print(inChar);

    if (inChar == '!') {
      Serial.print(inputString);
      //Serial.println("\n\nData received!\n\n\n");
    } 
  }
}



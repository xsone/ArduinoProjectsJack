/*
  Mega analogWrite() test
 	
  This sketch fades LEDs up and down one at a time on digital pins 2 through 13.  
  This sketch was written for the Arduino Mega, and will not work on previous boards.
 	
  The circuit:
  * LEDs attached from pins 2 through 13 to ground.

  created 8 Feb 2009
  by Tom Igoe
  
  This example code is in the public domain.
  
 */
// These constants won't change.  They're used to give names
// to the pins used:
int outPin1 = 9;
int outPin2 = 10;
int voltage = 255;

int pauze = 2000;

void setup() {
 analogWrite(outPin1, 0);
 analogWrite(outPin2, voltage);
   rust();
 analogWrite(outPin1, voltage);
 analogWrite(outPin2, 0);
 delay(2000);
}
 
  // set pins 2 through 13 as outputs:



void loop() {
 linksom();
 rust();
 rechtsom();
 rust();
}

void rust(){
 delay(2000);
 analogWrite(outPin1, 0);
 analogWrite(outPin2, 0);
 delay(2000);
} 

void linksom(){
 analogWrite(outPin1, 0);
 analogWrite(outPin2, voltage);
} 
  
void rechtsom(){
 analogWrite(outPin1, voltage);
 analogWrite(outPin2, 0);
} 


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
int outPin1 = 22;
int outPin2 = 23;
int voltage = 255;

int pauze = 2000;

void setup() {
 
 pinMode(outPin1, OUTPUT);
 pinMode(outPin2, OUTPUT);  
  
 digitalWrite(outPin1, 0);
 digitalWrite(outPin2, voltage);
   rust();
 digitalWrite(outPin1, voltage);
 digitalWrite(outPin2, 0);
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
 digitalWrite(outPin1, 0);
 digitalWrite(outPin2, 0);
 delay(2000);
} 

void linksom(){
 digitalWrite(outPin1, 0);
 digitalWrite(outPin2, voltage);
} 
  
void rechtsom(){
 digitalWrite(outPin1, voltage);
 digitalWrite(outPin2, 0);
} 


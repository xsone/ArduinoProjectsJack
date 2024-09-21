#define anout_pin 9 // toerental regeling ventilator 0-10V (van 5V omgezet naar 10V m.b.v transistor)

int potPIN = A1;  // select the input pin for the potentiometer
int ledPIN = 7; // select the output pin for the LED
int potVAL = 0;  // variable to store the value coming from the potentiometer

void setup() {
Serial.begin(9600);      // set the baud rate for the serial window
pinMode(ledPIN, OUTPUT); // declare the ledPin as an OUTPUT, ANALOG IN is an 
pinMode(anout_pin, OUTPUT);
digitalWrite(ledPIN, HIGH);  // set the ledPIN HIGH, which turns on the LED
delay(2000);               // do nothing for (potVAL) milliseconds
digitalWrite(ledPIN, LOW);                         // input by default
}

void loop() {
potVAL = analogRead(potPIN); // read the value from the potentiometer
Serial.print(potVAL);      // print potVAL in the arduino serial window
Serial.print("\t");      // print potVAL in the arduino serial window
potVAL = (potVAL/20);    // scale and shift the potVAL to be a useful 
Serial.println(potVAL);      // print potVAL in the arduino serial window
analogWrite(anout_pin, potVAL);
digitalWrite(ledPIN, HIGH);  // set the ledPIN HIGH, which turns on the LED
delay(potVAL);               // do nothing for (potVAL) milliseconds
digitalWrite(ledPIN, LOW);   // set the ledPIN LOW, which turns off the LED
delay(potVAL);               // do nothing for (potVAL) milliseconds
}


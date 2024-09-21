/*
  Input Pull-up Serial

  This demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital
  input on pin 2 (watersensor vlotter) and prints the results to the Serial Monitor.

  The circuit:
  - momentary switch attached from pin 2 to ground
  - built-in LED on pin 13, with relais connected to control waterpump 12VDC

  Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
  20K-ohm resistor is pulled to 5V. This configuration causes the input to read
  HIGH when the switch is open, and LOW when it is closed.

  created 1 juni 2024
  by Jack Cop

*/

const int maxTimer = 5; //beschermen van pomp niet langer dan ? sec. doordraaien
const int vlotterPin = 2;
const int relaisPin = 13;
int timerVal = 0;
bool timerStatus = false;

void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(vlotterPin, INPUT_PULLUP);
  pinMode(relaisPin, OUTPUT);
}

void loop() {
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(vlotterPin);
  //print out the value of the pushbutton
  //Serial.println(sensorVal);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal == HIGH & timerStatus == true) {
    digitalWrite(relaisPin, LOW);
    Serial.println("relais LOW");
    timerStatus = false;   
    timerVal = 0; 
  } 
  if (sensorVal == LOW & timerStatus == false) {
    digitalWrite(relaisPin, HIGH);
    Serial.println("relais HIGH");
    delay(1000);
    Serial.print("timerVal: ");
    Serial.println(timerVal, DEC);
    timerVal++;
    if(timerVal >= maxTimer){
      digitalWrite(relaisPin, LOW);
      Serial.println("relais LOW na TIMER...: ");
      timerStatus = true;
      timerVal = 0;
    }
  }
}

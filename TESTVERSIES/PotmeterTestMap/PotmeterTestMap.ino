// code for controlling the brightness of LED by a Potentiometer with the map() function.
int potPin = A0 ; // analog pin from the potentiometer
int LED = 3; // pin that is connected to the LED
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
}
void loop() {
  // read the analog in value:
  sensorValue = analogRead(potPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 825, 0, 200);
  // change the analog out value:
  analogWrite(LED, outputValue);
  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);
  Serial.print("output = ");
  Serial.println(outputValue);
 
  delay(200);
}

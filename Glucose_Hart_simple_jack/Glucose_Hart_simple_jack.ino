/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/

//5 volts / 1024 units or, 0.0049 volts (4.9 mV) per unit
// 928 * 4.9mV = 4.500mV


// These constants won't change. They're used to give names to the pins used:
//Settings Glucose sensor
const int sensorPinGluc = A1;  // Analog input pin that the potentiometer is attached to
int sensorValueGluc = 0;        // value read from the pot
int sensorTempGluc = 0;
int sensorCalGluc = 0;
int sensorValueGlucOld = 0;
int calValGluc = 0;
int deltaValGluc = 50;

//Settings Hartslag sensor
int sensorPinHart = 0;
int heartBeat = 0;
int heartBeatTest = 0;
int sensorTempHart = 0;
int sensorCalHart = 0;
int calValHart = 0;

//Settings Timer
unsigned long timer;
unsigned long interval = 10000; // 10 seconder
unsigned long lastTime = 0;
boolean rising = false;
boolean falling = false;

void setup(){
 // initialize serial communications at 9600 bps:
 Serial.begin(9600);
 Serial.println("Start Glucose & Hartslag meting...");
 glucCal();
 hartCal();
} 

//Calibratie Glucose sensor
void glucCal() {
 for (calValGluc=0; calValGluc<=5; calValGluc++){
 sensorTempGluc += analogRead(sensorPinGluc);
 //Serial.print("SensorTempGluc: ");
 //Serial.println(sensorTempGluc);
 delay(1000);
 }
 sensorCalGluc =  sensorTempGluc/calValGluc;
 //Serial.print("Sensor calibratie waarde Gluc: ");
 //Serial.println(sensorCalGluc);
 delay(2000);
} 


//Calibratie Hartslag sensor
void hartCal() {
 for (calValHart=0; calValHart<=5; calValHart++){
  sensorTempHart += analogRead(sensorPinHart);
  //Serial.print("SensorTempHart: ");
  //Serial.println(sensorTempHart);
  delay(1000);
 }
 sensorCalHart =  sensorTempHart/calValHart;
 //Serial.print("Sensor calibratie waarde Hart: ");
 //Serial.println(sensorCalHart);
 delay(2000);
 //Calibratie: bepaal maximum sensorwaarde
 //Doe verschil van ongeveer 50
}

void loop() {
//Uitlezen Glucose sensor
   // read the analog in value:
  sensorValueGluc = analogRead(sensorPinGluc);
  //Serial.println(sensorValue);
  
  if (sensorValueGluc > (sensorCalGluc - deltaValGluc) && sensorValueGluc < (sensorCalGluc + deltaValGluc)) {
    //Serial.print("Glucose: ");
    //Serial.print(",");
    //Serial.println(sensorValueGluc);
    sensorValueGlucOld = sensorValueGluc;
  }
  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  //delay(1000);

//Uitlezen Hartslag sensor
 if (analogRead(sensorPinHart)>= sensorCalHart && (rising == false)) {
  heartBeat++;
  heartBeatTest = 500; //laat in plotter puls zien
  rising = true;
 }   
 if (analogRead(sensorPinHart) < (sensorCalHart - 11) && (rising == true)) {
  rising = false;
  heartBeatTest = 0;
 }   
 //timer += millis();
 if(millis() - lastTime > interval) {
  lastTime = millis();  //timer die loopt tot interval
  Serial.print("BPM: ");
  Serial.print(",");
  Serial.println(heartBeat*6); //keer 6 voor omrekenen naar minuut
/*
  Serial.print(",");
  Serial.print("Glucose: ");
  Serial.print(",");
  Serial.println(sensorValueGlucOld);
 */ 
  heartBeat = 0; //na 1 minuut weer opnieuw beginnen
 }
  /*
     Serial.print(",");                
     Serial.print(heartBeatTest);      //test voor plotter puls
     Serial.print(",");                //seperator voor 2 waarden in plotter
     Serial.println(analogRead(sensorPinHart)); //laat signaal van sensor zien
     delay(500);
  */   
} 

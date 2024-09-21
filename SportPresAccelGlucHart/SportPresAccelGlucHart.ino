

/*  Getting_BPM_to_Monitor prints the BPM to the Serial Monitor, using the least lines of code and PulseSensor Library.
 *  Tutorial Webpage: https://pulsesensor.com/pages/getting-advanced
 *
--------Use This Sketch To------------------------------------------
1) Displays user's live and changing BPM, Beats Per Minute, in Arduino's native Serial Monitor.
2) Print: "♥  A HeartBeat Happened !" when a beat is detected, live.
2) Learn about using a PulseSensor Library "Object".
4) Blinks LED on PIN 13 with user's Heartbeat.
--------------------------------------------------------------------*/
#define DELAY 250 //delay in ms
#define HEADER "##"

//Settings algemeen
const int VCCPin = A4;
const int GNDPin = A0;
const int blinkPin = 13;          // The on-board Arduino LED, close to PIN 13.

//Settings Druk sensor
const int ForcePinLeft = A5;
const int ForcePinRight = A6;
int ForceValueLeft = 0;
int ForceValueRight = 0;

//Settings Versnellings sensor
const int xPin = A3;                  // x-axis of the accelerometer
const int yPin = A2;                  // y-axis
const int zPin = A1;                  // z-axis (only on 3-axis models)
unsigned const int minVal = 310;
unsigned const int maxVal = 475;

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

void setup() {   
  Serial.begin(9600);          // For Serial Monitor
  analogReference(DEFAULT);  
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(ForcePinLeft, INPUT);   
  pinMode(ForcePinRight, INPUT);  
  pinMode(A0, OUTPUT);
  pinMode(A4, OUTPUT);
  digitalWrite(A4, HIGH);
  digitalWrite(A0, LOW);
}

void loop() {
  //analogReference(EXTERNAL);
  int xData = analogRead(xPin);
  int yData = analogRead(yPin);
  int zData = analogRead(zPin);
  int accData= sqrt((xData*xData)+(yData*yData)+(zData*zData));



  //bereken de hoek in graden bereik van 0 tot 360 was 180
  int xAng = map(xData, minVal, maxVal, 0, 180);
   if (xAng < 0) xAng = 0;
  int yAng = map(yData, minVal, maxVal, 0, 180);
   if (yAng < 0) yAng = 0;
  int zAng = map(zData, minVal, maxVal, 0, 180);
   if (zAng < 0) zAng = 0;
 
 ForceValueLeft = analogRead(ForcePinLeft);
 //delay(DELAY);
 ForceValueRight = analogRead(ForcePinRight);
 //delay(DELAY);
 Serial.println(String(HEADER)+','+String(ForceValueLeft)+','+String(ForceValueRight)+','+ String(xAng)+','+String(yAng)+','+String(zAng)+','+String(accData)+',');
 delay(DELAY);                    // considered best practice in a simple sketch.
}

  

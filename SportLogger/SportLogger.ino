

/*  Getting_BPM_to_Monitor prints the BPM to the Serial Monitor, using the least lines of code and PulseSensor Library.
 *  Tutorial Webpage: https://pulsesensor.com/pages/getting-advanced
 *
--------Use This Sketch To------------------------------------------
1) Displays user's live and changing BPM, Beats Per Minute, in Arduino's native Serial Monitor.
2) Print: "♥  A HeartBeat Happened !" when a beat is detected, live.
2) Learn about using a PulseSensor Library "Object".
4) Blinks LED on PIN 13 with user's Heartbeat.
--------------------------------------------------------------------*/
#include "DHT.h"
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#define DHTPIN1 2     // what pin we're connected to
#define DHTTYPE DHT11  // DHT 11 sensor RH en T
#define DELAY 250 //delay in ms
#define HEADER "##"


const int blinkPin = 13;          // The on-board Arduino LED, close to PIN 13.
const int ForcePinLeft = A5;
const int ForcePinRight = A6;
const int pulsePin = A7;                 // Pulse Sensor purple wire connected to analog pin 0
const int VCCPin = A4;
const int xPin = A3;                  // x-axis of the accelerometer
const int yPin = A2;                  // y-axis
const int zPin = A1;                  // z-axis (only on 3-axis models)
const int GNDPin = A0;

volatile int BPM;
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

int ForceValueLeft = 0;
int ForceValueRight = 0;
float Temperature = 0.0;

unsigned const int minVal = 310;
unsigned const int maxVal = 470;

DHT dht1(DHTPIN1, DHTTYPE);

void setup() {   
  Serial.begin(9600);          // For Serial Monitor
  dht1.begin();
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(ForcePinLeft, INPUT);   
  pinMode(ForcePinRight, INPUT);  
  //analogReference(EXTERNAL);  
  pinMode(A0, OUTPUT);
  pinMode(A4, OUTPUT);
  digitalWrite(18, HIGH);
  digitalWrite(14, LOW);
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
}

void loop() {
  //analogReference(EXTERNAL);
  int xData = analogRead(xPin);
  int yData = analogRead(yPin);
  int zData = analogRead(zPin);
  int accData= sqrt((xData*xData)+(yData*yData)+(zData*zData));

Serial.print(xData);
Serial.print("\t");
Serial.print(yData);
Serial.print("\t");
Serial.println(zData);
 
  //bereken de hoek in graden bereik van 0 tot 360 was 180
  int xAng = map(xData, minVal, maxVal, 0, 180);
   if (xAng < 0) xAng = 0;
  int yAng = map(yData, minVal, maxVal, 0, 180);
   if (yAng < 0) yAng = 0;
  int zAng = map(zData, minVal, maxVal, 0, 180);
   if (zAng < 0) zAng = 0;
 
 ForceValueLeft = analogRead(ForcePinLeft);
 delay(DELAY);
 ForceValueRight = analogRead(ForcePinRight);
 delay(DELAY);
 Temperature = dht1.readTemperature();
 delay(DELAY);
 if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
        digitalWrite(blinkPin, HIGH);
        QS = false;                      // reset the Quantified Self flag for next time   
     }
 delay(20);
 digitalWrite(blinkPin, LOW);
 Serial.println(String(HEADER)+';'+String(BPM)+';'+String(ForceValueLeft)+';'+String(ForceValueRight)+';'+String(Temperature)+';'+ String(xAng)+';'+String(yAng)+';'+String(zAng)+';'+String(accData)+';');
 delay(DELAY);                    // considered best practice in a simple sketch.
}

  

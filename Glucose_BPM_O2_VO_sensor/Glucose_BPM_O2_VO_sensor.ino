/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInput
*/
#define SAMPLES 100
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  (blauw)
#define HEADER "##"

#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.
#include <DHT.h>   

byte sensorInterruptFlow = 1;  // 1 = digital pin 3 
byte sensorPinFlow       = 3;
// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;
volatile byte pulseCountFlow;  
float flowRate;
float oldFlowRate;
float maxFlowRate;
float lastFlowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

//voor hartslag
unsigned long timer;
unsigned long interval = 10000; // 10 seconden dan maal 6
unsigned long lastTime = 0;
boolean rising = false;
boolean falling = false;
int heartBeat = 0;
int heartBeatTest = 0;
volatile int BPM;                   // used to hold the pulse rate

int sensorPinGlucose = A0;    // select the input pin for the potentiometer
int sensorPinHeart = A1;    // select the input pin for the potentiometer
int blinkPin = 13;      // select the pin for the LED
int sensorValueGlucose = 0;  // variable to store the value coming from the sensor
int sensorValueHeart = 0;
int sensorValueGlucoseNa = 0;
int sensorValueGlucoseGem = 0;
int sensorDif = 250;
int sampleTeller = 0;
int sampleDrempel = 90;
//int BPM = 0;
int sensorThresholdHeart = 550;
boolean stateValue = LOW;

// these variables are volatile because they are used during the interrupt service routine!

volatile int Signal;                // holds the incoming raw data

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
 
  // declare the ledPin as an OUTPUT:
  pinMode(blinkPin, OUTPUT);
  pinMode(sensorPinGlucose, INPUT);  
  pinMode(sensorPinHeart, INPUT);  
  digitalWrite(blinkPin,HIGH);
  delay(2000);
  digitalWrite(blinkPin,LOW);
  //delay(2000);
  //gitalWrite(ledPin,HIGH);
  //delay(2000);
  //digitalWrite(ledPin,LOW);
  pinMode(sensorPinFlow, INPUT);
  digitalWrite(sensorPinFlow, HIGH);


  pulseCountFlow    = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterruptFlow, pulseCounterFlow, FALLING);
  dht.begin();
}


/*
Interrupt Service Routine
 */
void pulseCounterFlow()
{
  // Increment the pulse counter
  pulseCountFlow++;
  lastFlowRate = flowRate;
}


void loop() {
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  int glucose = analogRead(sensorPinGlucose);

  //BPM Hart sensor gedeelte
  if (analogRead(sensorPinHeart)> 600 && (rising == false)) {
     heartBeat++;
     heartBeatTest = 500; //laat in plotter puls zien
     rising = true;
  }   
  if (analogRead(sensorPinHeart) < 500 && (rising == true)) {
     rising = false;
     heartBeatTest = 0;
  }   
  if(millis() - lastTime > interval) {
    lastTime = millis();  //timer die loopt tot interval
    //Serial.print("BPM: ");
    //Serial.println(heartBeat); //keer 6 voor omrekenen naar minuut
    BPM = heartBeat * 6;
    heartBeat = 0; //na 1 minuut weer opnieuw beginnen
    Serial.println('{' + String(BPM) + ';' + String(temp) + ';' + String(hum) + ';' +  String(lastFlowRate) + ';' + String(glucose) + '}');
  }
  //Einde BPM hart gedeelte


   if (oldFlowRate > maxFlowRate) maxFlowRate = oldFlowRate;
   oldFlowRate = flowRate;
   //Serial.print("maxFlow: "); Serial.print(" "); Serial.println(int(maxFlowRate));  // Print the integer part of the variable
   //Serial.print("Flow: "); Serial.print(" "); Serial.println(int(flowRate));  // Print the integer part of the variable
   //Serial.println(String(HEADER)+';'+String(ForceValueLeft)+';'+String(ForceValueRight)+';'+ String(xAng)+';'+String(yAng)+';'+String(zAng)+';'+String(accData)+';');
    //Serial.println(String(HEADER) + "BPM: " + String(BPM) + ';' + "TEMP: " + String(temp) + ';' + "HUM: " + String(hum) + ';' + "MAXFLOW: " + String(maxFlowRate) + ';' + "FLOW: " + String(flowRate) + ';' + "GLUC: " + String(sesnorValueGlucose) + '}');
    //Serial.println('{' + String(BPM) + ';' + String(temp) + ';' + String(hum) + ';' +  String(flowRate) + ';' + String(glucose) + '}');
   
  /* JSON voorbeeld 
   {{"timestamp": "2019-01-13 17:36:41", "value1": 12.3, "value2": 100},
    {"timestamp": "2019-01-11 17:14:15", "value1": 21.3, "value2": 95},
    {"timestamp": "2019-01-11 17:11:45", "value1": 21.8, "value2": 98},
    {"timestamp": "2019-01-11 17:10:53", "value1": 21.6, "value2": 91}}
   */ 
  
  
  //Glucose sensor
  /*
    if(sensorValue < 450 && sensorValue > 400 ) 
     {
      sensorValueGem += sensorValue;
      //Serial.println(sensorValue);
     }
    //if (sensorDif > 250) 
    if (sensorDif > 150 && stateValue == LOW)
     {
      BPM++;
      //Serial.println(BPM);
      stateValue == HIGH;
      sensorValueNa = sensorValue;
      digitalWrite(ledPin,HIGH);
     } 
    delay(1000);
    sensorDif = abs(sensorValue - sensorValueNa);
    //Serial.println(sensorDif);
    digitalWrite(ledPin,LOW);
    //if ( (sensorValueNa - sensorValue >= sensorDif) && stateValue == HIGH)
    if (sensorDif < 50 && stateValue == HIGH)
     {
      stateValue == LOW;
      digitalWrite(ledPin,stateValue);
     }
  */


  //Flow sensor
   if((millis() - oldTime) > 1000)    // Only process counters once per second
    { 
      // Disable the interrupt while calculating flow rate and sending the value to
      // the host
      detachInterrupt(sensorInterruptFlow);
          
      // Because this loop may not complete in exactly 1 second intervals we calculate
      // the number of milliseconds that have passed since the last execution and use
      // that to scale the output. We also apply the calibrationFactor to scale the output
      // based on the number of pulses per second per units of measure (litres/minute in
      // this case) coming from the sensor.
      flowRate = ((1000.0 / (millis() - oldTime)) * pulseCountFlow) / calibrationFactor;
      
      // Note the time this processing pass was executed. Note that because we've
      // disabled interrupts the millis() function won't actually be incrementing right
      // at this point, but it will still return the value it was set to just before
      // interrupts went away.
      oldTime = millis();
      
      // Divide the flow rate in litres/minute by 60 to determine how many litres have
      // passed through the sensor in this 1 second interval, then multiply by 1000 to
      // convert to millilitres.
      flowMilliLitres = (flowRate / 60) * 1000;
      
      // Add the millilitres passed in this second to the cumulative total
      totalMilliLitres += flowMilliLitres;
        
      //unsigned int frac;
      
      // Print the flow rate for this second in litres / minute
      //Serial.print("Flow rate: ");
      //Serial.println(int(flowRate));  // Print the integer part of the variable
      //Serial.print("L/min");
      //Serial.print("\t");       // Print tab space
  
      // Print the cumulative total of litres flowed since starting
      //Serial.print("Output Liquid Quantity: ");        
      //Serial.print(totalMilliLitres);
      //Serial.println("mL"); 
      //Serial.print("\t");       // Print tab space
      //Serial.print(totalMilliLitres/1000);
      //Serial.print("L");
      
  
      // Reset the pulse counter so we can start incrementing again
      pulseCountFlow = 0;
      
      // Enable the interrupt again now that we've finished sending output
      attachInterrupt(sensorInterruptFlow, pulseCounterFlow, FALLING);
    }
    //delay(20);
}

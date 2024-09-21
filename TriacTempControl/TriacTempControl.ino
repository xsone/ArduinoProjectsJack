/*
 Triac Dimmer o.b.v. temperatuur sensor

 created 18 May 2024
 by Jack Cop
 
 */

#include <SPI.h>
#include <TriacDimmer.h>
#include "DHT.h"

#define DHTPIN 4     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 (besteld bij dealextreme.com blauw)
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

//int minFanSpeed = 75; //is 0V
//int maxFanSpeed = 255; //is 10V

int minFanSpeed = 0; //is 0V
int maxFanSpeed = 255; //is 10V

float minTemperature = 20;
float maxTemperature = 35;

float temperatureOld = 0;
float temperature = 0;

unsigned char sync = 8; //sync pin
unsigned char channel_1 = 9; // channel 1 pin

const int ZCP = 2; //zero cross detection triac
const int dimPin = 13; //PWM poort van Nano (kan ook LEDpin 13 zijn om dimmer te testen)

unsigned int dimValue = 0;
unsigned int mapValue = 0;
float brightness = 0;

unsigned int drempelWaarde = 10; // indien meer dan 10W wordt opgewekt dan aan
unsigned int maxLoad = 500; //maximale vermogen van verwarminsgelement of boiler of andere belasting


void setup() {
  pinMode(ZCP, INPUT);
  pinMode(dimPin, OUTPUT);
  digitalWrite(dimPin, LOW);
  
  TriacDimmer::begin();
  attachInterrupt(digitalPinToInterrupt(2), ZeroCross, CHANGE);

  dht.begin();
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 Serial.print("Triac Dimmer gestart..");
 delay(500);
 //fanSpeed = minFanSpeed;
 temperature = dht.readTemperature(); //lees temperatuur uit in C, was ook een float
}

void loop() {
  temperature = dht.readTemperature(); //lees temperatuur uit in C, was ook een float
  Serial.print("Temperatuur: ");
  Serial.println(temperature);
  
  float brightness = abs(floatMap(temperature, minTemperature, maxTemperature, minFanSpeed, maxFanSpeed));
  //float brightness = floatMap(temperature, minTemperature, maxTemperature, 0, 1);
  Serial.print("Brightness: ");
  Serial.println(brightness);
  digitalWrite(dimPin, brightness);
  
  TriacDimmer::setBrightness(channel_1, brightness); 
} 


//Triac control
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


 
//    //Temperatuur stijgt
//  if (TempOld < (Temp+1) )
//   {
//    fanSpeed += 50;
//    Timer=0;
//   } 
//
//  //Temperatuur daalt
//  if (TempOld > (Temp-1) ) 
//   {
//    fanSpeed -= 50;
//    Timer++;
//   } 
//  
//  //Temperatuur gaat omlaag en niet meer gedouched
//  if (Timer > StopTime)
//   {
//    fanSpeed = minFanSpeed;
//    Timer = 0;
//   }
//
//  if(fanSpeed <= minFanSpeed) fanSpeed = minFanSpeed;
//  if(fanSpeed >= maxFanSpeed) fanSpeed = maxFanSpeed;
//
//  analogWrite(anout_pin, 255-fanSpeed);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
//  DisplayData();
//  TempOld = Temp;
//  delay(500);


//  if (digitalRead(ZCP) == HIGH) {
//    ZeroCross();
////    //dimValue = 5000;
//    dimValue = map(elecACTverbruik.toInt(),100,3000,0,7400); 
//    Serial.println(dimValue); 
//  }
//}


void ZeroCross(){
  digitalWrite(dimPin, HIGH);
  delayMicroseconds(dimValue);  
  digitalWrite(dimPin, LOW);
}

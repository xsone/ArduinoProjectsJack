/* MQ-3 Alcohol Sensor Circuit with Arduino */
#include <DHT.h>

// define
#define DHTPIN 6
#define DHTTYPE DHT11   // DHT 11 besteld bij DX.com

DHT dht(DHTPIN, DHTTYPE);


byte dht_dat[5];

const int AOUTpin=0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int DOUTpin=10;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino

//const int DOUTpin7=7;//VCC 5 Volt buzzer
//const int DOUTpin6=6;//GND 0 Volt buzzer
//const int DOUTpin5=5;//GND 0 Volt buzzer

// Sensor opwarmen duurt ca. 20 sec.
// Schone lucht van 35% RH, 26C ongeveer 40-50 bij alcohol naar
// Clearsil: 400-800
// Nagellak remover Aceton: 400-500
// Alcohol 600-700

float limit;
float value;
int buzzpin = 11;


void setup() {


Serial.begin(9600);//sets the baud rate

dht.begin();

//pinMode(DOUTpin6, OUTPUT);//sets the pin as an input to the arduino
//digitalWrite (DOUTpin6, LOW);
//pinMode(DOUTpin7, OUTPUT);//sets the pin as an input to the arduino
//digitalWrite (DOUTpin7, HIGH);
//pinMode(DOUTpin5, OUTPUT);//sets the pin as an input to the arduino
//digitalWrite (DOUTpin5, LOW);
pinMode(buzzpin,OUTPUT);
buzz(200);
pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
}

void buzz(unsigned char time)
{
analogWrite(buzzpin,170);
delay(time);
analogWrite(buzzpin,0);
delay(time);
}




void loop()
{
 delay(200);
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  Serial.print("{\"Humi\":"); 
  Serial.print(h,2);
  Serial.print(",\"Temp\":"); 
  Serial.print(t,2);
  Serial.print(",\"Sens\":");
  Serial.print(value,2);//prints the alcohol value
  Serial.println("}");
  
  value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
  limit= digitalRead(DOUTpin);//reads the digital value from the alcohol sensor's DOUT pin
  
  //Serial.print("Limit: ");
  //Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
  delay(100);
 if (limit == LOW){
  digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator
  digitalWrite(DOUTpin, LOW);//if limit has been reached, LED turns on as status indicator
  buzz(200);
  }
 else digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off
 digitalWrite(DOUTpin, HIGH);//if limit has been reached, LED turns on as status indicator
}


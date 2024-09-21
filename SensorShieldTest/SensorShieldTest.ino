/*

(c) 2020 Jack Cop
Sensorshield with Arduino Duemilanove board
Smoke sensor MQ-6 (FC-22) connected to analog A1
Alcohol sensor MQ-3 (FC-22) connected to analog A0
LDR Keyes K853518 connected to analog A3


 
 All the resources for this project:
https://www.electromaniaweb.wordpress.com 
https://www.arduino.cc
https://www.youtube.com/watch?v=BIf_mpnsZvY

*******/
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.

#define DHTPIN 7  
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//SD initialisatie
File dataFile;
File root;
const int chipSelect = 8; //CS voor Sparkfun anders CS=4
bool SDstate = true;
bool flowState = false;

//Log settings
String logFile = "sportlog.csv";  //naam kort houden
int logFileNr = 1;
String logFileExt = ".csv";

const String lim = ",";    // Data Streamer expects a comma delimeter
String logData = "";
String huidigeTijd = "";
unsigned long logInterval = 3 * 1000;     // logInterval
unsigned long lastTimeLogger = 0;
unsigned long actualTime = 0;
unsigned int seconde = 0;
unsigned int minuut = 0;
unsigned int uur = 0;


//define inputs
byte sensorInterruptFlow = 0;  // 0 = digital pin 2 
// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;
volatile byte pulseCountFlow = 0;  
float flowRate = 0.0;
float oldFlowRate = 0;
float maxFlowRate = 0;
float lastFlowRate = 0;
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime;
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

const int alcoholLED = 10;
const int smokeLED = 11;
const int gasLED = 13;
const int sensorPinFlow = 2;

int smokeSensor;
int alcoholSensor;
float humiditySensor;
float temperatureSensor;
int gasSensor;

const int alcoholA0 = A0;
const int smokeA1 = A1;
const int gasA3 = A3;

const int buzzer = 4;
const int alarmLED = 13;

//define settings and thresholds
int smokeThreshold = 550;
int alcoholThreshold = 200;
int gasThreshold = 300;

const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

int tel = 0;

void setup() {
 Serial.begin(9600); 

 initSDcard();

 pinMode(sensorPinFlow, INPUT);
 digitalWrite(sensorPinFlow, HIGH);
 attachInterrupt(sensorInterruptFlow, pulseCounterFlow, FALLING);
  
 pinMode(alcoholLED, OUTPUT);
 pinMode(alcoholA0, INPUT);
  
 pinMode(smokeLED, OUTPUT);
 pinMode(smokeA1, INPUT);
 
 pinMode(gasLED, OUTPUT);
 pinMode(gasA3, INPUT);
 
 dht.begin();
  
 pinMode(buzzer, OUTPUT);  
 //Serial.println("Time,Flow,Alcohol,Smoke,Gas,Temperature,Humidity");
}

void loop() {
// while (tel <= 10)
//  {
   smokeDetect();
   alcoholDetect();
   gasDetect();
   //tempHumDetect();
   getActualTime();
   //logData = (huidigeTijd + ',' + String(flowRate) + "," + String(alcoholSensor) + ',' + String(smokeSensor) + ',' + String(gasSensor) + ',' + String(temperatureSensor) + ',' + String(humiditySensor) + ',' );
   //logData = (String(tel) + ',' + String(alcoholSensor) + ',' + String(smokeSensor) + ',' + String(gasSensor));
   //logData = (String(alcoholSensor) + ',' + String(smokeSensor));
   //Serial.println(logData);
   Serial.print(alcoholSensor);     //read xpin and send value over serial
   Serial.print(',');                 //send a "tab" over serial
   Serial.print(smokeSensor);
   Serial.print(',');                 //send a "tab" over serial
   Serial.println(gasSensor);
   delay (500); 
//   tel++;
//   if (tel > 10) tel = 0;
//  } 
 
 //periodieke logging
 //if (oldFlowRate > 0.0){
 //  if (oldFlowRate > maxFlowRate) maxFlowRate = oldFlowRate;
   
  //} 
 /* 
  if(millis() - lastTimeLogger > logInterval) 
   {
   logData = (huidigeTijd + ',' + String(maxFlowRate) + "," + String(alcoholSensor) + ',' + String(smokeSensor) + ',' + String(gasSensor) + ',' + String(temperatureSensor) + ',' + String(humiditySensor) + ',' );
   if (SDstate) writeFile(logFile, logData);
   lastTimeLogger = millis();
   //Serial.println(">>");
   Serial.println(logData);
   //Serial.println(">>...Data Logged...");
   if (SDstate) readFile(logFile);
  }
  */ 

  if (oldFlowRate != 0 && flowState){
  //if (oldFlowRate != flowRate){
   //if (oldFlowRate > maxFlowRate) maxFlowRate = oldFlowRate
   //logData = (huidigeTijd + ',' + String(flowRate) + "," + String(alcoholSensor) + ',' + String(smokeSensor) + ',' + String(gasSensor) + ',' + String(temperatureSensor) + ',' + String(humiditySensor) + ',' );
   //Serial.println(logData);
   flowState = false;
  } 
 
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
  //} 
  oldFlowRate = flowRate;
  if (flowRate == 0) flowState = true;
}

void initSDcard()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    SDstate = false;
    // don't do anything more:
    return;
  }

 /*
  logFile = logFile + logFileNr + logFileExt;
  Serial.print("File name: ");
  Serial.println(logFile);

  `                                                                                                                                                 File entry = root.openNextFile();
  if (entry.isDirectory()) {
    Serial.println(dataFile.name());
  }
 
  //int logFileNr = StringToInt( logFile.substring(logFile.length()-1,logFile.length()) );
  logFileNr = String.toInt(logFile.substring(8,9));
  Serial.print("Filenr: ");
  Serial.println(logFileNr);
  if (logFileNr == 9) logFileNr = 1;
  logFileNr++;
  */
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open(logFile, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("Time;Alcohol;Smoke;Gas;Temperature;Humidity"); //Write the first row of the excel file
    dataFile.close();
  }
}

void writeFile(String fileName, String dataLog)
{
 // so you have to close this one before opening another.
 dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataLog);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataLog);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.print("Error opening: ");
    Serial.println(fileName);
  }
}

void readFile(String fileName)
{
 dataFile = SD.open(fileName);
 if (dataFile) {
   Serial.println(fileName);
   // read from the file until there's nothing else in it:
   while (dataFile.available()) {
     Serial.write(dataFile.read());
  }
   // close the file:
   dataFile.close();
 } else {
    // if the file didn't open, print an error:
    Serial.print("Error opening: ");
    Serial.println(fileName);
 }
} 

void getActualTime()
{
  actualTime = millis();
  if (actualTime >= 6000) seconde++;
  if (seconde >= 60){
    minuut++;
    seconde = 0;
  }
 if (minuut >= 60){
  uur++;
  minuut = 0; 
 }
 if (uur >= 24){
  uur = 0;
 }
 huidigeTijd = String(uur) + ":" + String(minuut) + ":" + String(seconde);
}

void pulseCounterFlow()
{
  // Increment the pulse counter
  pulseCountFlow++;
  lastFlowRate = flowRate;
}

void alcoholDetect()
{
  alcoholSensor = analogRead(alcoholA0);
  //Serial.print("Alcohol A0: ");
  //Serial.println(alcoholSensor);
  // Checks if it has reached the threshold value
  if (alcoholSensor > alcoholThreshold)
  {
   digitalWrite(alcoholLED, LOW);
   tone(buzzer, 1000, 200);
  }
  else
  {
   digitalWrite(alcoholLED, HIGH);
   noTone(buzzer);
  }
}  
/*
  if(value<200)
  {
      display.println("You are sober.");
  }
  if (value>=200 && value<280)
  {
      display.println("You had a beer.");
  }
  if (value>=280 && value<350)
  {
      display.println("Two or more beers.");
  }
  if (value>=350 && value <450)
  {
      display.println("I smell Oyzo!");
  }
  if(value>450)
  {
     display.println("You are drunk!");
  }
*/

void smokeDetect()
{
 smokeSensor = analogRead(smokeA1);
 //Serial.print("Smoke A1: ");
 //Serial.println(smokeSensor);
 // Checks if it has reached the threshold value
 if (smokeSensor > smokeThreshold)
  {
    digitalWrite(smokeLED, LOW);
    tone(buzzer, 500, 200);
    delay(500);
  }
  else
  {
    digitalWrite(smokeLED, HIGH);
    noTone(buzzer);
  }
}

/*
void flameDetect()
{
 //flameSensor = analogRead(flameA2);
 flameSensor = map(analogRead(flameA2), 0, 1023, 0, 25);
 // Checks if it has reached the threshold value
 if (flameSensor > flameThreshold)
  {
    digitalWrite(flameLED, LOW);
    tone(buzzer, 300, 200);
    delay(500);
  }
  else
  {
    digitalWrite(flameLED, HIGH);
    noTone(buzzer);
  }
}
*/

void gasDetect()
{
 gasSensor = analogRead(gasA3);
 //Serial.print("Gas A3: ");
 //Serial.println(gasSensor);
 // Checks if it has reached the threshold value
 if (gasSensor > gasThreshold)
  {
    digitalWrite(gasLED, LOW);
    tone(buzzer, 500, 200);
    delay(500);
  }
  else
  {
    digitalWrite(gasLED, HIGH);
    noTone(buzzer);
  }
}
  

void tempHumDetect()
{
  humiditySensor = dht.readHumidity();
  temperatureSensor = dht.readTemperature();
  if (isnan(humiditySensor) || isnan(temperatureSensor) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //Serial.print("Temperature: "); 
  //Serial.print(temperatureSensor);
  //Serial.println(" *C ");
  
  //Serial.print("Humidity: "); 
  //Serial.print(humiditySensor);
  //Serial.println(" %");  
}

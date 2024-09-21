/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

const int chipSelect = 8; //CS voor Sparkfun anders CS=4
String logFile = "sportlog.csv";  //naam kort houden
String logData = "5566";
File myFile;
long logInterval = 10 * 1000;     // logInterval
long lastTimeLogger = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  writeFile(logFile, logData);
  readFile(logFile);
}

void loop() {
 
//periodieke logging
if(millis() - lastTimeLogger > logInterval) 
  {
   lastTimeLogger = millis();
   writeFile(logFile, logData);
   logData = "12345;";
   Serial.println("...Data Logged...");
   readFile(logFile);
  }
}


void writeFile(String fileName, String dataLog)
{
 // so you have to close this one before opening another.
 myFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (myFile) {
    myFile.println(dataLog);
    myFile.close();
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
 myFile = SD.open(fileName);
 if (myFile) {
   Serial.println(fileName);
   // read from the file until there's nothing else in it:
   while (myFile.available()) {
     Serial.write(myFile.read());
  }
   // close the file:
   myFile.close();
 } else {
    // if the file didn't open, print an error:
    Serial.print("Error opening: ");
    Serial.println(fileName);
 }
}  

// Temperature meting Ingaande en Uitgaande CV leiding
// Voor een goed redemenet moet delta In/Out ca. 20 C zijn
// (c) Jack Cop nov. 2016

#include "DHT.h"
#include <SD.h>

#define DHTPIN1 6     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN1, DHTTYPE);

const int chipSelect = 4;

void setup() {
  Serial.begin(9600); 
  
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
 
  Serial.println("DHT11 Temp en RH...");
  dht.begin();
}

void loop() {
   // make a string for assembling the data to log:
  String dataString = "";

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float Rhum = dht.readHumidity();
  // Read temperature as Celsius
  float Temp = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(Temp) || isnan(Rhum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  dataString = ("H:" + (String)Rhum + ";" + "T: " + (String)(Temp));
  //Serial.print(dataString);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
 //   dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  // Wait a few seconds between measurements.
  delay(1800000); //elk halfuur
}

// Temperature meting Ingaande en Uitgaande CV leiding
// Voor een goed redemenet moet delta In/Out ca. 20 C zijn
// (c) Jack Cop nov. 2016

#include "DHT.h"

#define DHTPIN1 2     // what pin we're connected to
//#define DHTPIN2 7     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11  // DHT 11 

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht1(DHTPIN1, DHTTYPE);
//DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  Serial.println("DHT11 TempIn TempOut test!");
 
  dht1.begin();
 // dht2.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //float h = dht.readHumidity();
  // Read temperature as Celsius
  float TempIn = dht1.readTemperature();
 // float TempOut = dht2.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(TempIn)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature IN : "); 
  Serial.print(TempIn);
  Serial.println(" *C ");
 // Serial.print("Temperature OUT: "); 
 // Serial.print(TempOut);
 // Serial.println(" *C ");
}

/* 
*  Arduino, Temp, Humidity, WiFi, MySQL and Highcharts remixed by Tim Scott
*  From a Simple WiFi weather station with Arduino, the DHT11 sensor & the CC3000 chip 
*  Writtent by Marco Schwartz for Open Home Automation
*  https://learn.adafruit.com/wifi-weather-station-arduino-cc3000/introduction
*  Part of the code is based on the work done by Adafruit on the CC3000 chip & the DHT11 sensor
*  https://learn.adafruit.com/adafruit-cc3000-wifi
*/

// Include required libraries
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "SimpleDHT.h"
//#include <SHT1x.h>
//#include<stdlib.h>


// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// WiFi network (change with your settings !)
#define WLAN_SSID1       "TRAFFICBOX1"        // cannot be longer than 32 characters!
#define WLAN_SSID2       "TRAFFICBOX2"        // cannot be longer than 32 characters!
#define WLAN_PASS       "Z3zhhjyS2nbm"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_TRY        1 //aantal keren dat poging wordt gedaan te verbinden met Wifi Netwerk 1 = 10 sec.

// Specify data and clock connections and instantiate SHT1x object
//#define dataPin  9
//#define clockPin 8
//SHT1x sht1x(dataPin, clockPin);
//static char tempbuffer[10];
//static char humidbuffer[10];
#define DHTPIN 6

SimpleDHT22 dht22(DHTPIN);
float humidity = 0;
float temperature = 0;

// Create CC3000 & DHT instances

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);
                                         
// Local server IP, port, and repository (change with your settings !)
uint32_t ip = cc3000.IP2U32(192,168,178,20);//your computers ip address
int port = 80;//your webserver port (8888 is the default for MAMP)
String repository = "http://192.168.178.20/phpMyAdmin/";//the folder on your webserver where the sensor.php file is located
                                         
void setup(void)
{
 Serial.begin(115200);
    
 // Initialise the CC3000 module
 if (!cc3000.begin())
  {
    while(1);
  }

 Serial.println(F("\nDeleting old connection profiles"));
 if (!cc3000.deleteProfiles()) {
   //Serial.println(F("Failed!"));
   while(1);
 }
 
 // Connect to  WiFi network TRAFFICBOX 1 of 2
 if(cc3000.connectToAP(WLAN_SSID1, WLAN_PASS, WLAN_SECURITY, WLAN_TRY)){
  Serial.print("Verbonden met Wifi netwerk: "); Serial.println(WLAN_SSID1);
 }
 else if(cc3000.connectToAP(WLAN_SSID2, WLAN_PASS, WLAN_SECURITY, WLAN_TRY)){
  Serial.print("Verbonden met Wifi netwerk: "); Serial.println(WLAN_SSID2);
 }
    
 // Check DHCP
 Serial.println(F("Request DHCP"));
 while (!cc3000.checkDHCP())
 {
    delay(100);
  }  
}

void loop(void)
{
  dht22.read2(&temperature, &humidity, NULL);
  Serial.print("Hum: "); 
  Serial.print(humidity);
  Serial.println(" %RH");
  Serial.print("Temp: "); 
  Serial.print(temperature);
  Serial.println(" °C ");
    
  // Send request
  String request = "GET "+ repository + "temphumlog.php?temperature=" + temperature + "&humidity=" + humidity + " HTTP/1.0";
  send_request(request);
  Serial.println("");
  Serial.print("request: ");
  Serial.println(request);
  Serial.println("");
  // Update every 30 seconds
  delay(300000);
}

// Function to send a TCP request and get the result as a string
void send_request (String request) {
     
    // Connect    
    Serial.println("Starting connection to server...");
    Adafruit_CC3000_Client client = cc3000.connectTCP(ip, port);
    
    // Send request
    if (client.connected()) {
      client.println(request);      
      client.println(F(""));
      Serial.println("Connected & Data sent");
    } 
    else {
      Serial.println(F("Connection failed"));    
    }

    while (client.connected()) {
      while (client.available()) {

      // Read answer
      char c = client.read();
      }
    }
    Serial.println("Closing connection");
    Serial.println("");
    client.close();
}

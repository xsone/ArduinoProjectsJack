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
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// WiFi network (change with your settings !)
#define WLAN_SSID       "TRAFFICBOX2"        // cannot be longer than 32 characters!
#define WLAN_PASS       "Z3zhhjyS2nbm"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define DHTPIN 6

SimpleDHT22 dht22(DHTPIN);
float humidity = 0;
float temperature = 0;

// Create CC3000 & DHT instances

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);
Adafruit_CC3000_Client client;
MySQL_Connection conn((Client *)&client);
                                         
// Local server IP, port, and repository (change with your settings !)
uint32_t ip_server_addr_nas = cc3000.IP2U32(192,168,178,20);//your computers ip address
int port = 80;//your webserver port (8888 is the default for MAMP)

char INSERT_DATA[] = "INSERT INTO temphumlog(temperature, humidity) VALUES(%d, %d);";
char query[128];
char user[] = "Arduino";              // MySQL user login username
char password[] = "#@Xymox123";        // MySQL user login password
char default_db[] = "Energielogger";   //MySQL database name 
byte server_addr_nas[] = { 192, 168, 178, 20 };
//String server_addr_nas = "192.168.178.20";
//String repository = "http://192.168.178.20/phpMyAdmin/";//the folder on your webserver where the sensor.php file is located
                                         
void setup(void)
{
 Serial.begin(115200);
    
 // Initialise the CC3000 module
 if (!cc3000.begin())
  {
    while(1);
  }

  // Connect to  WiFi network
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  //Serial.println("Connected to WiFi network!");
    
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
//  Serial.print("Hum: "); 
//  Serial.print(humidity);
//  Serial.println(" %RH");
//  Serial.print("Temp: "); 
//  Serial.print(temperature);
//  Serial.println(" °C ");

  writeMysql();
  delay(10000);
}

// Function to send a TCP request and get the result as a string
void writeMysql(void) {
     
    // Connect    
    //Serial.println("Starting connection to server...");
    Adafruit_CC3000_Client client = cc3000.connectTCP(ip_server_addr_nas, port);
    //MySQL_Connection conn((Client *)&client);
    // Send request
    if (client.connected()) {
       while (conn.connect(ip_server_addr_nas, 3307, user, password) != true) {
           delay(200);
           Serial.print ( "." );
       }
//      if(conn.connect(ip_server_addr_nas, 3307, user, password, default_db)){
//         MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
//         sprintf(query, INSERT_DATA, temperature, humidity);
//         cur_mem->execute(query);
//         delete cur_mem;
//         //num_fails = 0;
//         delay(500);
//         Serial.println("Connection DB succes...");
//      }  
      client.println(F(""));
      Serial.println("Connected & Data sent");
    } 
    else {
      //Serial.println(F("Connection failed"));    
    }

    while (client.connected()) {
      while (client.available()) {

      // Read answer
      char c = client.read();
      Serial.print("c");
      }
    }
    //Serial.println("Closing connection");
    //Serial.println("");
    client.close();
}

/***************************************************
Wifi Shield van ElecFreaks 2013 met een CC3000MOD wifi-chip van TI
Maakt gebruik van: Adafruit CC3000 Breakout/Shield Simple HTTP Server
Let op!! dat het shield op een Arduino UNO wordt geplaatst, zie power LED licht op, anders werkt het niet
Er wordt een DHT22 (nauwkeuriger) temperatuursensor geplaatst
Ook wordt een rookmelder gekoppeld
Nov. 2022 (c) Jack Cop

CC3000MOD chip: http://learn.adafruit.com/adafruit-cc3000-wifi  
downloadlink library: https://github.com/adafruit/Adafruit_CC3000_Library
****************************************************/

#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <ccspi.h>
#include <string.h>
#include "utility/debug.h"
#include "utility/socket.h"
#include "SimpleDHT.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
//#include <Mail.h>
//#include <SMTPClient.h>

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
#define DHTPIN 6
#define AREF 5

#define WLAN_SSID1      "TRAFFICBOX2"        // cannot be longer than 32 characters!
//#define WLAN_SSID2      "TRAFFICBOX1"        // cannot be longer than 32 characters!
#define WLAN_PASS       "Z3zhhjyS2nbm"

// What page to grab!
//#define WEBSITE      "https://sites.google.com/d/1q42ooL2_uZRaSZBmB8rbTSjsfX6DB8Wt/p/1FujIP97i8Wz9ev61y44_ZSUUCLu5kEVe/edit?pli=1"
//#define WEBPAGE      "192.168.178.20/index.html"

#define WEBSITE      "www.adafruit.com"
#define WEBPAGE      "/testwifi/index.html"

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define LISTEN_PORT           80      // What TCP port to listen on for connections. The HTTP protocol uses port 80 by default.
//#define MAX_ACTION            10      // Maximum length of the HTTP action that can be parsed.
//#define MAX_PATH              64      // Maximum length of the HTTP request path that can be parsed. There isn't much memory available so keep this short!
//#define BUFFER_SIZE           MAX_ACTION + MAX_PATH + 20  // Size of buffer for incoming request data.Since only the first line is parsed this needs to be as large as the maximum action and path plus a little for whitespace andHTTP version.
//#define TIMEOUT_MS            500    // Amount of time in milliseconds to wait for an incoming request to finish.  Don't set this too high or your server could be slow to respond.
#define IDLE_TIMEOUT_MS  3000   

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

SimpleDHT22 dht22(DHTPIN);

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER); // you can change this clock speed
//uint32_t ipAddress = cc3000.IP2U32(192, 168, 178, 70);
//uint32_t netMask = cc3000.IP2U32(255, 255, 255, 0);
//uint32_t defaultGateway = cc3000.IP2U32(192, 168, 178, 1);
//uint32_t dns = cc3000.IP2U32(192, 168, 178, 1);

//Adafruit_CC3000_Server httpServer(LISTEN_PORT);
//uint8_t buffer[BUFFER_SIZE + 1];
//int bufindex = 0;
//char action[MAX_ACTION + 1];
//char path[MAX_PATH + 1];

//float humidity = 0;
//float temperature = 0;

Adafruit_CC3000_Client client;        // For WiFi connections
//Adafruit_CC3000_Server echoServer(80);
MySQL_Connection conn((Client *)&client);

//Mysql MariaDB login
//IPAddress server_addr_nas(192, 168, 178, 20);  
IPAddress server_addr_nas_test(20, 178, 168, 192);  
IPAddress server_addr_nas(192, 168, 178, 20);  
char user[] = "Arduino";              // MySQL user login username
char password[] = "#@Xymox123";        // MySQL user login password
char default_db[] = "Energielogger";   //MySQL database name 
//byte server_addr_nas[] = { 192, 168, 178, 20 };
//String server_addr_nas = "192.168.178.20";

uint32_t ip;

//char INSERT_DATA[] = "INSERT INTO temphumlog(temperature, humidity) VALUES(%d, %d);";
//char query[128];

//unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
//const unsigned long postingInterval = 5000;  // delay between updates, in milliseconds

void setup(void)
{
 //dht.begin(); //start HumTemp sensor

 Serial.begin(115200);
 Serial.println(F("Hello, wifi shield CC3000!")); 
 //Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
   
 // Initialise the module
 Serial.println(F("\nInitializing..."));
 if (!cc3000.begin())
  {
   //Serial.println(F("Couldn't begin()! Check your wiring?"));
   while(1);
  }
 
 /* Delete any old connection data on the module */
 Serial.println(F("\nDeleting old connection profiles"));
 if (!cc3000.deleteProfiles()) {
   //Serial.println(F("Failed!"));
   while(1);
 }

 Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID1);
 if (!cc3000.connectToAP(WLAN_SSID1, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
 }   
// else {
//    Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID2); Serial.println(WLAN_PASS); Serial.println(WLAN_SECURITY);
//    if (!cc3000.connectToAP(WLAN_SSID2, WLAN_PASS, WLAN_SECURITY)) {
//     Serial.println(F("Failed!"));
//     while(1);
//    }
// }
 
// if (!cc3000.setStaticIPAddress(ipAddress, netMask, defaultGateway, dns)) {
//   Serial.println(F("Failed to set static IP!"));
//   while(1);
// }
 //Serial.println(F("Connected!"));

 
 Serial.println(F("Request DHCP"));
 while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  //Display the IP address DNS, Gateway, etc.
  while (!displayConnectionDetails()) {
    delay(1000);
  }

    while (ip == 0) {
    if (! cc3000.getHostByName(WEBSITE, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }

  ip = server_addr_nas_test;
  Serial.println(ip);
  
  cc3000.printIPdotsRev(ip);

 //Adafruit_CC3000_Client test = cc3000.connectTCP(ip, 80);
 if (client.connect(ip, 80)){
  Serial.println("Client connectie oke...");
 } 

 //MySQL_Connection conn((Client *)&test);
  while (conn.connect(ip, 3307, user, password) != true) {
      delay(200);
      Serial.print ( "." );
    }
  
    Serial.println("");
    Serial.println("Connected to SQL Server!");  
 
// if (conn.connect(ip, 80, user, password)) {
//     delay(200);
//     Serial.print ( "." );
//     Serial.println("");
//     Serial.println("Connected to SQL Server!");  
//  }  
// else {
//    Serial.println(F("Connection failed"));    
//    return;
//  }
//  
  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
  //Serial.println(F("\n\nDisconnecting"));
  //cc3000.disconnect();

//  while (conn.connect(server_addr_nas, 3307, ) != true) {
//      delay(200);
//      Serial.print ( "." );
//    }
//  
//    Serial.println("");
//    Serial.println("Connected to SQL Server!");  

// if(conn.connect(server_addr_nas, 3307, user, password, default_db)){
//    //delay(500);
//    Serial.println("Connection DB succes...");
//  }
}

void loop(void)
{

 // Try to get a client which is connected.
 //Adafruit_CC3000_ClientRef client = httpServer.available();

// if (millis() - lastConnectionTime > postingInterval) {
//   lastConnectionTime = millis();

//   dht22.read2(&temperature, &humidity, NULL);
//   Serial.print("Hum: "); 
//   Serial.print(humidity);
//   Serial.println(" %RH");
//   Serial.print("Temp: "); 
//   Serial.print(temperature);
//   Serial.println(" *C ");
   
//   if(conn.connect(server_addr_nas, 3307, user, password, default_db)){
//     MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
//     sprintf(query, INSERT_DATA, temperature, humidity);
//      cur_mem->execute(query);
//      delete cur_mem;
//      //num_fails = 0;
//      delay(500);
//      Serial.println("Connection DB succes...");
//   }  
// }
}


// Tries to read the IP address and other connection details
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
//    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
//    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
//    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
//    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
//    Serial.print(F("\nDNSping: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

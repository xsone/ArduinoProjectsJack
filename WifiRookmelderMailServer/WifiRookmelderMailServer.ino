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
#include "utility/debug.h"
#include "utility/socket.h"
#include "DHT.h"
//#include <Ethernet.h>
//#include <Client.h>
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
#define DHTTYPE DHT22
#define AREF 5

#define WLAN_SSID       "TRAFFICBOX2"        // cannot be longer than 32 characters!
#define WLAN_PASS       "Z3zhhjyS2nbm"

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define LISTEN_PORT           80      // What TCP port to listen on for connections. The HTTP protocol uses port 80 by default.
#define MAX_ACTION            10      // Maximum length of the HTTP action that can be parsed.
#define MAX_PATH              64      // Maximum length of the HTTP request path that can be parsed. There isn't much memory available so keep this short!
#define BUFFER_SIZE           MAX_ACTION + MAX_PATH + 20  // Size of buffer for incoming request data.Since only the first line is parsed this needs to be as large as the maximum action and path plus a little for whitespace andHTTP version.
#define TIMEOUT_MS            500    // Amount of time in milliseconds to wait for an incoming request to finish.  Don't set this too high or your server could be slow to respond.

DHT dht(DHTPIN, DHTTYPE);

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER); // you can change this clock speed
//uint32_t ipAddress = cc3000.IP2U32(192, 168, 178, 70);
//uint32_t netMask = cc3000.IP2U32(255, 255, 255, 0);
//uint32_t defaultGateway = cc3000.IP2U32(192, 168, 178, 1);
//uint32_t dns = cc3000.IP2U32(192, 168, 178, 1);

Adafruit_CC3000_Server httpServer(LISTEN_PORT);
uint8_t buffer[BUFFER_SIZE + 1];
int bufindex = 0;
char action[MAX_ACTION + 1];
char path[MAX_PATH + 1];

float humidity = 99.9;
float temperature = 11.1;

char server[] = "smtp.ziggo.nl";

// Set the panic button pin
byte panicButton = 7;
boolean statusCheck = false;
boolean rookAlarm = false;

Adafruit_CC3000_Client client;        // For WiFi connections

void setup(void)
{
 dht.begin(); //start HumTemp sensor

 Serial.begin(115200);
 Serial.println(F("Hello, wifi shield CC3000!")); 
 Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
  
 // Initialise the module
 Serial.println(F("\nInitializing..."));
 if (!cc3000.begin())
  {
   Serial.println(F("Couldn't begin()! Check your wiring?"));
   while(1);
  }
 
 /* Delete any old connection data on the module */
 Serial.println(F("\nDeleting old connection profiles"));
 if (!cc3000.deleteProfiles()) {
   Serial.println(F("Failed!"));
   while(1);
 }
  
 Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID); Serial.println(WLAN_PASS); Serial.println(WLAN_SECURITY);
 if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
 
// if (!cc3000.setStaticIPAddress(ipAddress, netMask, defaultGateway, dns)) {
//   Serial.println(F("Failed to set static IP!"));
//   while(1);
// }
 Serial.println(F("Connected!"));

 
 Serial.println(F("Request DHCP"));
 while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  // Display the IP address DNS, Gateway, etc.
  while (! displayConnectionDetails()) {
    delay(1000);
  }
  
  // Start listening for connections
  httpServer.begin();
}

void loop(void)
{
 // Try to get a client which is connected.
 Adafruit_CC3000_ClientRef client = httpServer.available();
 
 if (client) {
  while (client.connected()) {
      if (client.available()) {
        Serial.println(F("Client connected."));
        // Process this request until it completes or times out.
        // Note that this is explicitly limited to handling one request at a time!
        // Clear the incoming data buffer and point to the beginning of it.
        bufindex = 0;
        memset(&buffer, 0, sizeof(buffer));
        // Clear action and path strings.
        memset(&action, 0, sizeof(action));
        memset(&path,   0, sizeof(path));
        // Set a timeout for reading all the incoming data.
        unsigned long endtime = millis() + TIMEOUT_MS;
          
        // Read all the incoming data until it can be parsed or the timeout expires.
        bool parsed = false;
        while (!parsed && (millis() < endtime) && (bufindex < BUFFER_SIZE)) {
           if (client.available()) {
              buffer[bufindex++] = client.read();
            }
            parsed = parseRequest(buffer, bufindex, action, path);
          }
       
//        humidity = dht.readHumidity();
//        temperature = dht.readTemperature();
        Serial.print("Humidity: "); 
        Serial.print(humidity);
        Serial.println(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(temperature);
        Serial.println(" *C ");
        break;
      }
  }
  delay(1);
 }
 rookMelder();
}    

//        // Handle the request if it was parsed.
//        if (parsed){  
//         Serial.println(F("Processing request"));
//         Serial.print(F("Action: ")); Serial.println(action);
//         Serial.print(F("Path: ")); Serial.println(path);
//         
//         if (strcmp(action, "GET") == 0) {
//              // Respond with the path that was accessed.
//              // First send the success response code.
//              client.fastrprintln(F("HTTP/1.1 200 OK"));
//              // Then send a few headers to identify the type of data returned and that
//              // the connection will not be held open.
//              client.fastrprintln(F("Content-Type: text/plain"));
//              client.fastrprintln(F("Connection: close"));
//              client.fastrprintln(F("Server: Adafruit CC3000"));
//              // Send an empty line to signal start of body.
//              client.fastrprintln(F(""));
//              // Now send the response data.
//              client.fastrprintln(F("Hello world!"));
//              client.fastrprint(F("You accessed path: ")); client.fastrprintln(path);
//              if (rookAlarm) client.fastrprintln(F("Smoke Alarm HIGH..."));
//              else client.fastrprintln(F("Smoke Alarm LOW..."));
//            }
//            else {
//              // Unsupported action, respond with an HTTP 405 method not allowed error.
//              client.fastrprintln(F("HTTP/1.1 405 Method Not Allowed"));
//              client.fastrprintln(F(""));
//            }

//  }
//   
//   
//   
//   // Check the action to see if it was a GET request.
//   if (strcmp(action, "GET") == 0) {
//    // Respond with the path that was accessed.
//    // First send the success response code.
//    client.fastrprintln(F("HTTP/1.1 200 OK")); // Then send a few headers to identify the type of data returned and that the connection will not be held open.
//    client.fastrprintln(F("Content-Type: text/plain"));
//    client.fastrprintln(F("Refresh: 5"));  // refresh the page automatically every 5 sec
//    client.fastrprintln(F(""));
//    client.fastrprintln(F("<!DOCTYPE HTML>"));
//    client.fastrprintln(F("<html>"));
//    client.fastrprintln(F("<meta http-equiv=refresh content=5>"));
//    client.fastrprintln(F("<body bgcolor=\"#E0FFFF\">"));
//    client.fastrprintln(F("<center><p><h1><align=center>Schuur Monitor  v1.0  </h1></p> "));
//    client.fastrprintln(F("<center><p><h1><align=center>"));
//    client.fastrprintln(F("Connection: close"));
//    client.fastrprintln(F("Server: Adafruit CC3000"));
//    // Send an empty line to signal start of body.
//    client.fastrprintln(F(""));
//    // Now send the response data.
//    client.fastrprintln(F("Status Schuur.."));
//    //client.fastrprint(F("You accessed path: ")); client.fastrprintln(path);
//    client.fastrprint(F("Humidity: "));
//    client.println(humidity);
//    client.fastrprint(F("Temperature: ")); 
//    client.println(temperature);
//    if (rookAlarm) client.fastrprintln(F("Smoke Alarm HIGH..."));
//    else client.fastrprintln(F("Smoke Alarm LOW..."));
//   } 
//   else {
//    // Unsupported action, respond with an HTTP 405 method not allowed error.
//    client.fastrprintln(F("HTTP/1.1 405 Method Not Allowed"));
//    client.fastrprintln(F(""));
//   }
//  }
//  // Wait a short period to make sure the response had time to send before
//  // the connection is closed (the CC3000 sends data asyncronously).
//  delay(100);
//  // Close the connection when done.
//  //Serial.println(F("Client disconnected"));
//  //client.close();
// }
// rookMelder();
//} 



// Return true if the buffer contains an HTTP request.  Also returns the request
// path and action strings if the request was parsed.  This does not attempt to
// parse any HTTP headers because there really isn't enough memory to process
// them all.
// HTTP request looks like:
//  [method] [path] [version] \r\n
//  Header_key_1: Header_value_1 \r\n
//  ...
//  Header_key_n: Header_value_n \r\n
//  \r\n
bool parseRequest(uint8_t* buf, int bufSize, char* action, char* path) {
  // Check if the request ends with \r\n to signal end of first line.
  if (bufSize < 2)
    return false;
  if (buf[bufSize-2] == '\r' && buf[bufSize-1] == '\n') {
    parseFirstLine((char*)buf, action, path);
    return true;
  }
  return false;
}

// Parse the action and path from the first line of an HTTP request.
void parseFirstLine(char* line, char* action, char* path) {
  // Parse first word up to whitespace as action.
  char* lineaction = strtok(line, " ");
  if (lineaction != NULL)
    strncpy(action, lineaction, MAX_ACTION);
  // Parse second word up to whitespace as path.
  char* linepath = strtok(NULL, " ");
  if (linepath != NULL)
    strncpy(path, linepath, MAX_PATH);
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
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

void rookMelder(void)
{
 if(digitalRead(panicButton)==HIGH && statusCheck==false)
  {
   Serial.println(F("Alarm..."));
   if(sendEmail()) Serial.println(F("Email sent"));
   else Serial.println(F("Email failed"));
    statusCheck = true;
    rookAlarm = true;
   }
  else if (digitalRead(panicButton)==LOW)
   {
    statusCheck = false;
    rookAlarm = false;
   }
}


byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

  //if(client.connect(server,587)) {
  if(client.connect(server, 25)) {  
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }

  if(!eRcv()) return 0;

  //change to your public ip
  client.fastrprintln(F("HELO 212.54.34.9"));

  if(!eRcv()) return 0;
  Serial.println(F("Sending From"));

// change to your email address (sender)
  client.fastrprintln(F("MAIL From: jack.cop@ziggo.nl"));

  if(!eRcv()) return 0;

// change to recipient address
  Serial.println(F("Sending To"));
  client.fastrprintln(F("RCPT To: jack.cop@ziggo.nl"));

  if(!eRcv()) return 0;

  Serial.println(F("Sending DATA"));
  client.fastrprintln(F("DATA"));

  if(!eRcv()) return 0;

  Serial.println(F("Sending email"));

// change to recipient address
  client.fastrprintln(F("To: You jack.cop@ziggo.nl"));
  client.fastrprintln(F("From: Me jack.cop@ziggo.nl"));
  client.fastrprintln(F("Subject: Smoke Alarm!\r\n"));
  client.fastrprintln(F("Smoke ALARM!!!"));
  client.fastrprint(F("Humidity: "));
  client.println(humidity);
  client.fastrprint(F("Temperature: ")); 
  client.println(temperature);
  client.fastrprintln(F("."));

  if(!eRcv()) return 0;

  Serial.println(F("Sending QUIT"));
  client.fastrprintln(F("QUIT"));

  if(!eRcv()) return 0;

  client.close();

  Serial.println(F("disconnected"));

  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while(!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.close();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  if(respCode >= '4')
  {
    efail();
    return 0;  
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;
  int loopCount = 0;

  client.println(F("QUIT"));

  while(!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.close();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  client.close();

  Serial.println(F("disconnected"));
}

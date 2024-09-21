/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-http-request
 */

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 5*1000;  // delay between updates, in milliseconds

int    HTTP_PORT   = 8080;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "192.168.178.144";
String PATH_NAME   = "/bridge";
String queryString = "/zones/zn1/status";
//http://192.168.178.144:8080/bridge/zones/zn1/status
//IPAddress ip_nefit(192, 168, 178, 144);

String streamRead;
//String json[] =  "{"id":"/zones/zn1/status","type":"stringValue","writeable":0,"recordable":0,"value":"idle"}";

void setup() {
  Serial.begin(9600);

  // initialize the Ethernet shield using DHCP:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to obtaining an IP address using DHCP");
    while(true);
  }
}

void loop() {
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
    Serial.println();
  }
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

// connect to Bosch-Nefit server on port 8080:
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    // if connected:
    Serial.println("Connected to server");
    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
    //client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    while(client.connected()) {
      if(client.available()){
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        streamRead += c;
      }
    }
    Serial.println(streamRead);
    lastConnectionTime = millis();
    streamRead = "";
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2022, Benoit BLANCHON
// MIT License
//
// This example shows how to parse a JSON document in an HTTP response.
// It uses the Ethernet library, but can be easily adapted for Wifi.
//
// It performs a GET resquest on https://arduinojson.org/example.json
// Here is the expected response:
// {
//   "sensor": "gps",
//   "time": 1351824120,
//   "data": [
//     48.756080,
//     2.302038
//   ]
// }

//Voorbeeld programma Boiler status
// Stream& input;

//StaticJsonDocument<256> doc;
//
//DeserializationError error = deserializeJson(doc, input);
//
//if (error) {
//  Serial.print(F("deserializeJson() failed: "));
//  Serial.println(error.f_str());
//  return;
//}
//
//const char* id = doc["id"]; // "/dhwCircuits/dhw1/operationMode"
//const char* type = doc["type"]; // "stringValue"
//int writeable = doc["writeable"]; // 1
//int recordable = doc["recordable"]; // 0
//const char* used = doc["used"]; // "false"
//const char* available = doc["available"]; // "true"
//const char* infoText = doc["infoText"]; // "intelli"
//const char* value = doc["value"]; // "Off"


//Voorbeeld met True filter:
//StaticJsonDocument<0> filter;
//filter.set(true);
//
//StaticJsonDocument<256> doc;
//
//DeserializationError error = deserializeJson(doc, input, DeserializationOption::Filter(filter));
//
//if (error) {
//  Serial.print(F("deserializeJson() failed: "));
//  Serial.println(error.f_str());
//  return;
//}
//
//const char* id = doc["id"]; // "/dhwCircuits/dhw1/operationMode"
//const char* type = doc["type"]; // "stringValue"
//int writeable = doc["writeable"]; // 1
//int recordable = doc["recordable"]; // 0
//const char* used = doc["used"]; // "false"
//const char* available = doc["available"]; // "true"
//const char* infoText = doc["infoText"]; // "intelli"
//const char* value = doc["value"]; // "Off"


// Stream& input;



//
// https://arduinojson.org/v6/example/http-client/

#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;
MySQL_Connection conn((Client *)&client);

//Mysql MariaDB login
char user[] = "Arduino";              // MySQL user login username
char password[] = "#@Xymox123";        // MySQL user login password
char default_db[] = "Energielogger";   //MySQL database name 
byte server_addr_nas[] = { 192, 168, 178, 20 };

//char INSERT_DATA[] = "INSERT INTO cvlogger(cvStatus, boilerStatus, retourTemp, kamerTemp, kamerHum) VALUES(%d, %d, %d, %d, %d);";
//char query[256];
char INSERT_DATA[] = "INSERT INTO cvlogger(cvStatus, boilerStatus) VALUES(%d, %d);";
char query[128];

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 5000;  // delay between updates, in milliseconds

int    HTTP_PORT   = 8080;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "192.168.178.144";
String PATH_NAME   = "/bridge";

unsigned int cvStatus = 0;
unsigned int boilerStatus = 0;
//float retourTemp = 11;
//float kamerTemp = 22;
//float kamerHum = 33;

String queryString;
//String statusValue[0] = "CV-status: "; 
//String queryString[0] = "/zones/zn1/status";
//String statusValue[1] = "Boiler-status: ";
//String queryString[1] = "/dhwCircuits/dhw1/operationMode";

//String statusValue[];


//CV status
//String cvStatus = "/zones/zn1/status";
//String radiatorJack = "/zones/zn2/status";
//String radiatorMaurits = "/zones/zn3/status";
//String radiatorRuben = "/zones/zn4/status";
//
////Boiler status
//String boilerStatus = "/dhwCircuits/dhw1/operationMode";



//Thermostaat status


//http://192.168.178.144:8080/bridge/zones/zn1/status
//IPAddress ip_nefit(192, 168, 178, 144);

const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 256;
DynamicJsonDocument doc(capacity);
//char status[32] = {0};



void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  while (!Serial) continue;

  // Initialize Ethernet library
  if (!Ethernet.begin(mac)) {
    Serial.println(F("Failed to configure Ethernet"));
    return;
  }

  //MySQL gedeelte
  if(conn.connect(server_addr_nas, 3307, user, password, default_db)){
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    //sprintf(query, INSERT_DATA, cvStatus, boilerStatus, retourTemp, kamerTemp, kamerHum);
    sprintf(query, INSERT_DATA, cvStatus, boilerStatus);
    cur_mem->execute(query);
    delete cur_mem;
    //num_fails = 0;
    delay(500);
    Serial.println("Connection DB succes...");
   }

// httpRequest("/system/sensors/temperatures/indoorAirDigital");
// Serial.println("Temperatuur kamer: " + doc["value"].as<String>()); //test
} 


// this method makes a HTTP connection to the server:
void httpRequest(String queryString) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();
 
  // Connect to HTTP server
  if (!client.connect(HOST_NAME, HTTP_PORT)) {
    Serial.println(F("Connection failed"));
    return;
  }
  //Serial.println(F("Connected!"));

  // Send HTTP request
  client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
  //client.println(F("Host: arduinojson.org"));
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    client.stop();
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  //memset(status, 0, sizeof(status));
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    memset(status, 0, sizeof(status));
    client.stop();
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    client.stop();
    return;
  }

  // Allocate the JSON document
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
  //const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 100;
  //DynamicJsonDocument doc(capacity);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    client.stop();
    return;
  }

  // Extract values
  //Serial.println();
  //Serial.print(F("Response: "));
  //Serial.println(doc["value"].as<const char*>());
  //Serial.println(doc["value"].as<String>());
//  Serial.println(doc["time"].as<long>());
//  Serial.println(doc["data"][0].as<float>(), 6);
//  Serial.println(doc["data"][1].as<float>(), 6);
// Disconnect
//  client.stop();
//  lastConnectionTime = millis();
//  
}


void loop() {
  if (millis() - lastConnectionTime > postingInterval) {
   lastConnectionTime = millis();
   
   httpRequest("/zones/zn1/status");
   if (doc["value"].as<String>().equals("idle") && cvStatus == 1) {
    cvStatus = 0;
    Serial.println("CV van AAN naar UIT > Database");
    if(conn.connect(server_addr_nas, 3307, user, password, default_db)){
      MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
      //sprintf(query, INSERT_DATA, cvStatus, boilerStatus, retourTemp, kamerTemp, kamerHum);
      sprintf(query, INSERT_DATA, cvStatus, boilerStatus);
      cur_mem->execute(query);
      delete cur_mem;
      //num_fails = 0;
      delay(500);
      Serial.println("Connection DB succes...");
    }
   }
   if (doc["value"].as<String>().equals("heat request") && cvStatus == 0){
    cvStatus = 1;
    Serial.println("schrijf CV status AAN > Database");
    if(conn.connect(server_addr_nas, 3307, user, password, default_db)){
      MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
      //sprintf(query, INSERT_DATA, cvStatus, boilerStatus, retourTemp, kamerTemp, kamerHum);
      sprintf(query, INSERT_DATA, cvStatus, boilerStatus);
      cur_mem->execute(query);
      delete cur_mem;
      //num_fails = 0;
      delay(500);
      Serial.println("Connection DB succes...");
    }
   }
   
   
   Serial.print("CV-status: "); Serial.println(cvStatus);
   
//   httpRequest("/heatSources/returnTemperature");
//   Serial.println("Retour-temp: " + doc["value"].as<String>());
//   
//   httpRequest("/dhwCircuits/dhw1/operationMode");
//   Serial.println("Boiler-status: " + doc["value"].as<String>());

   httpRequest("/dhwCircuits/dhw1/state");
   Serial.println("Boiler-status: " + doc["value"].as<String>());

//   httpRequest("/system/sensors/humidity/indoor_h1");
//   Serial.println("Humidity kamer: " + doc["value"].as<String>());
//   
//   httpRequest("/system/sensors/temperatures/indoorAirDigital");
//   Serial.println("Temperatuur kamer: " + doc["value"].as<String>());
//   
   Serial.println();
  }
}

// Performance issue?
// ------------------
//
// EthernetClient is an unbuffered stream, which is not optimal for ArduinoJson.
// See: https://arduinojson.org/v6/how-to/improve-speed/

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any
// serialization  problem.
//
// The book "Mastering ArduinoJson" contains a tutorial on deserialization
// showing how to parse the response from GitHub's API. In the last chapter,
// it shows how to parse the huge documents from OpenWeatherMap
// and Reddit.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤

/*
   WebserverSendjson
   
   - connect to ethernet in DHCP mode
   - send IP on serial output
   - wait for incoming http request with format http://aaa.bbb.ccc.ddd/cantuina
   - answer in json format
   
 */

// include
#include <SPI.h>
#include <Ethernet.h>

// globals
char c;
char cmd;
char _buf[50];
int ethok=0;
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 };
IPAddress ip(192,168,178,15);

//IPAddress ip;

boolean isreqline = true;
String req = String();
String par = String();

// Ethernet server
EthernetServer server(80);

// the setup routine runs once when you press reset:
void setup() {
  // open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.println("-- UART READY");

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

// loop routine
void loop() {
 
  // web server
    EthernetClient client = server.available();   // listen to client connecting
    if (client) {
      Serial.println("ETHERNET : new client http request");
      boolean currentLineIsBlank = true;   // an http request ends with a blank line
      while (client.connected()) {
        Serial.println("client.connected");
        //if (client.available()) {
          //Serial.println("client.available");
          // read char from client
            //c = client.read();
            //Serial.println(c);
          // append to request string
          //if ((isreqline)&&(req.length()<127)) req += c; 
          // stop parsing after first line
         // if (c=='\n') isreqline = false;
          
          // if you've gotten to the end of the line (received a newline character) and the line is blank,
          // the http request has ended, so you can send a reply
          //if ((c=='\n') && currentLineIsBlank) { //oorspronkelijk
         
         
          //if (c=='\n') {  //oorspronkelijk
           
            // if request does not contain "cantuina" keyword send 404
           // if (req.indexOf("cantuina")==-1){
           //     send_404(client);
           //     Serial.println("No CANTUINA");
           //  }  
            // else send JSON response
            //else {
              
              client.println("HTTP/1.1 200 OK");
                Serial.println("HTTP/1.1 200 OK");
              client.println("Content-Type: application/json");  // JSON response type
              client.println("Connection: close");               // close connection after response
              client.println();
              // open JSON
              // example : {"res":"OK","temp":"25.0","pres":"1020","humi":"45"}
              client.print("{");
                Serial.print("{");
              // result
              client.print("\"res\":\"OK\"");
                Serial.print("\"res\":\"OK\"");
              // temperature
              client.print(",\"temp\":\"");
                Serial.print(",\"temp\":\"");
              client.print(25.0);
                Serial.print(25.0);
              //client.print(15.5+random(0,20));
              client.print("\"");
                Serial.print("\"");
              // pressure
              client.print(",\"pres\":\"");
                Serial.print(",\"pres\":\"");
              client.print(980+random(0,50));
                Serial.print(980+random(0,50));
              client.print("\"");
                Serial.print("\"");
              //humidity
              client.print(",\"humi\":\"");
                Serial.print(",\"humi\":\"");
              client.print(25+random(0,50));
                Serial.print(25+random(0,50));
              client.print("\"");
                Serial.print("\"");
              // close json
              client.println("}");
                Serial.println("}");
            //}

            // prepare for next request
            //req = "";
            //isreqline = true;
            // exit
            break;
          } // end if available
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          } 
          else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
       // }  //end if
       // give the web browser time to receive the data
       delay(500);
       // close the connection:
      client.stop();
    } //end while
  //} // end if clinet avail
}  // end loop

void formatIP(IPAddress ii) {
  sprintf(_buf,"%d.%d.%d.%d",ii[0],ii[1],ii[2],ii[3]);
}

static void send_404(EthernetClient client) {
  client.println("HTTP/1.1 404 Not Found");
  client.println("Content-Type: text/html");
  client.println();
  client.println("404 NOT FOUND");
}

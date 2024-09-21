/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

int led2 = 6;
int led1 = 7;

int potPIN = 0;  // select the input pin for the potentiometer
int ledPIN = 13; // select the output pin for the LED
float potVAL = 0;  // variable to store the value coming from the potentiometer
char potVALchar = 0;

int analogChannel = 0;

char c;
int k;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 };
IPAddress ip(192,168,178,15);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
 // Open serial communications and wait for port to open:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  digitalWrite(led1, HIGH);
  delay(1000); 
  digitalWrite(led1, LOW);
  
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
       Serial.println("client.connected");
      //if (client.available()) {
      //  Serial.println("client.available");
      
       potVAL = analogRead(potPIN); // read the value from the potentiometer
       c = client.read();  // toetsen inlezen
         Serial.println(c);
 
        //client.println("HTTP/1.1 200 OK");
        //   Serial.println("HTTP/1.1 200 OK");
        //client.println("Content-Type: text/html"); //eerder HTML
        //client.println("Connection: close");  // the connection will be closed after completion of the response
        //client.println();
          

       if (c == 'e'){
        client.println("HTTP/1.1 200 OK");
                  Serial.println("HTTP/1.1 200 OK");
                client.println("Content-Type: application/json");  // JSON response type
                  //client.println("Content-Type: text/html"); //eerder HTML
                client.println("Connection: close");  // the connection will be closed after completion of the response
                client.println();
                // open JSON
                // example : {"res":"OK","temp":"25.0","pres":"1020","humi":"45"}
                client.print("{");
                 Serial.print("{");
                // result
                client.print("\"res\":\"OK\"");
                 Serial.print("\"res\":\"OK\""); 	  
                client.print(",\"temp\":\"");
                 Serial.print(",\"temp\":\"");
                 // client.print(25.0); //testwaarde
                 //  Serial.print(25.0); //testwaarde
                client.print(potVAL);
                 Serial.print(potVAL);
                client.print("\"");
                 Serial.print("\"");
                 
                /*
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
                 */
                 
                // close json
                client.println("}");
                 Serial.println("}");
                c = '?'; 
                break;
       }         
       if (c == 'a'){
                // client.println("HTTP/1.1 200 OK");
                //     Serial.println("HTTP/1.1 200 OK");
                //  client.println("Content-Type: text/html"); //eerder HTML
                //  client.println("Connection: close");  // the connection will be closed after completion of the response
                //  client.println();
                
                 digitalWrite(led1, HIGH);
                 client.println("LED1 is ON");
                 Serial.println("LED1 is ON");
                 c = '?';
                 break;
       }         
       if (c == 'b'){
                 digitalWrite(led1, LOW);
                 client.println("LED1 is OFF");
                 Serial.println("LED1 is OFF");
                 c = '?';
                 break;
       }          
       if (c == 'c'){
                 digitalWrite(led2, HIGH);
                 client.print("LED2 is ON");
                 Serial.println("LED2 is ON");
                 c = '?';
                 break;
       }          
       if (c == 'd'){
                 digitalWrite(led2, LOW);
                 client.print("LED2 is OFF");
                 Serial.println("LED2 is OFF");
                 c = '?';
                 break;
       }
//      case '\n': currentLineIsBlank = true;
//      case   else if (c != '\r') {
//       // you've gotten a character on the current line
//       currentLineIsBlank = false;
//   }
   
 
  // einde toetsen etc. 
   
  //} // end ij client available
 } // end while
    // give the web browser time to receive the data
    //delay(1);
    delay(200);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
   } // eind 
} // end loop


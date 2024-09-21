/*
 Chat Server

 A simple server that distributes any incoming messages to all
 connected clients.  To use, telnet to your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:

// Enter a MAC address and IP address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x00, 0x8E, 0x10 }; // Laatste cijfer MAC steeds 1 ophogen, kamer 1 0x11, 0x12 etc.
byte ip[]  = { 192, 168, 178, 80 };                  // IP-Adres van ethershield CV-ketel; kamers 81, 82 etc.
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };               // standdaard subnet 
byte phpserver[] = { 192, 168, 178, 20 };            // IP van de NAS-server

// telnet defaults to port 8888
EthernetServer server(8888);
boolean alreadyConnected = false; // whether or not the client was connected previously
String httpData = "";
String httpData1 = "CVaan";
String httpData2 = "CVuit";
//char httpData[10];
String CVstate = "CV???";

int Relay = 9;  // LED test relay output voor sturen CV-ketel

void setup() {
  // initialize the ethernet device
   pinMode(Relay, OUTPUT);
  //test relais werking
  digitalWrite(Relay, HIGH);
  delay(2000);
  digitalWrite(Relay, LOW);
  
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
    
  server.begin();
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  
 
  }


  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}







void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) 
  {
    boolean currentLineIsBlank = true;
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true;
    }

    if (client.available() > 0) 
     {
      char c = client.read();
      httpData += c;
      //Serial.print(httpData);
      if (c == '\n' && currentLineIsBlank) 
       {
        
        // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    //client.println("Content-Type: application/json");  // JSON response type
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>Arduino CV Control</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>LED</h1>");
                    client.println("<p>Click to switch CV on and off.</p>");
                    client.println("<form method=\"get\">");
                    client.println("</form>");
                    client.println("</body>");
                    client.println("</html>");
                    
        //Serial.println(httpData.length());
        //httpData.trim();
        Serial.println(httpData);
        Serial.println(httpData.substring(5,15));
        if ( httpData.substring(5,15) == "?cmd=CVaan") { Serial.println("CV AAN"); CVstate = "CVaan"; digitalWrite(Relay, HIGH); }
        if ( httpData.substring(5,15) == "?cmd=CVuit") { Serial.println("CV UIT"); CVstate = "CVuit"; digitalWrite(Relay, LOW); }
        //if ( httpData.equals("?cmd=CVaan")) { Serial.println("CV AAN"); CVstate = "CV??aan"; digitalWrite(Relay, HIGH); }
        //if ( httpData.equals("?cmd=CVuit")) { Serial.println("CV UIT"); CVstate = "CVuit"; digitalWrite(Relay, LOW);  }
        //if ( httpData.equals("CVaan")) { Serial.println("CV AAN"); CVstate = "CV??aan"; digitalWrite(Relay, HIGH); }
        //if ( httpData.equals("CVuit")) { Serial.println("CV UIT"); CVstate = "CVuit"; digitalWrite(Relay, LOW);  }
        //Serial.println(CVstate);        
        httpData = "";    // finished with request, empty string
        //break;
       }
     if (c == '\n') currentLineIsBlank = true;
     else if (c != '\r') currentLineIsBlank = false;
    } //end client avail
  } // end if client
}




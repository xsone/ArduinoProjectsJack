/*
 * Web Server
 *
 * A simple web server that shows the value of the analog input pins.
 */

#include <Ethernet.h>
#include <SPI.h>

int celsius,celsiusF;
int led2 = 3;
int led1 = 4;

int potPIN = 0;  // select the input pin for the potentiometer
int ledPIN = 13; // select the output pin for the LED
int potVAL = 0;  // variable to store the value coming from the potentiometer
char potVALchar = 0;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 };
byte ip[] = { 192, 168, 178, 15 };

EthernetServer server(80);
boolean incoming = 0;

char  ReplyBuffer[] = "acknowledged";       // a string to send back

void setup()
{
  Ethernet.begin(mac, ip);
 //server.begin();
  
  Serial.begin(9600);
  Serial.println ("Server gestart...");  
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  digitalWrite(led1, HIGH);
  delay(1000); 
  digitalWrite(led1, LOW);
  
  digitalWrite(led2, HIGH);
  delay(1000); 
  digitalWrite(led2, LOW);
}

void loop()
{
 potVAL = analogRead(potPIN); // read the value from the potentiometer
 potVAL = (potVAL/2 + 16);
 digitalWrite(ledPIN, HIGH);  // set the ledPIN HIGH, which turns on the LED
 delay(potVAL);               // do nothing for (potVAL) milliseconds
 digitalWrite(ledPIN, LOW);   // set the ledPIN LOW, which turns off the LED
 delay(potVAL);               // do nothing for (potVAL) milliseconds
 Serial.println(potVAL);
  
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //params = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
        // client.println("HTTP/1.1 200 OK");
        // client.println("Content-Type: text/html"); 
        // client.println("Connection: close");  // the connection will be closed after completion of the response
	// client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          
         
         client.print("<H1>Temperature<br/>");
         //client.print(celsius);
         client.print(potVAL);
         
         if(potVAL == 52) c = '5';
         //client.print(",");
         //client.print(celsiusF);
         client.print(" C</H1>");
         client.print("<br><br>");
         client.print("<br><br>");
         client.print("<H1>LED control</H1>");
         client.print("<A HREF='1'>Turn off</A><br>");
         client.print("<A HREF='2'>Turn on</A><br><br>");
         
         client.println();
         client.println("<!DOCTYPE HTML>");
         client.println("<html>");

          



          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
   
 //  if (strcmp(params, "?cmd=on") == 0)
  if (c == '1')
   {
     digitalWrite(led1, HIGH);
     client.print("LED1 is ON");
     Serial.println("LED1 is ON");
   }
//  else if (strcmp(params, "?cmd=off") == 0) 
  if (c == '2') 
   {
     digitalWrite(led1, LOW);
     client.print("LED2 is OFF");
     Serial.println("LED2 is OFF");
    }          
   }
  }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

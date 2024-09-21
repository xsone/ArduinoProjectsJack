/*
 * Web Server
 *
 * A simple web server that shows the value of the analog input pins.
 */

#include <Ethernet.h>
#include <SPI.h>

#include <OneWire.h>
//OneWire  ds(A3);  // on pin A3
int celsius,celsiusF;
int outputPin = 3; //the LED pin

int led2 = 7;
int led1 = 6;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 };
byte ip[] = { 192, 168, 178, 16 };

EthernetServer server(8032);
boolean incoming = 0;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back
char c;

void setup()
{
  Serial.begin(9600);
  
  Ethernet.begin(mac, ip);
  server.begin();
    
  //Serial.println ("Server gestart..."); 
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP()); 
  
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
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
       c = client.read();
       Serial.write(c);
      if (c == '\n' && current_line_is_blank) { 
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");  // the connection will be closed after completion of the response
       client.println("Refresh: 5");  // refresh the page automatically every 5 sec  
      client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
      /*  
        client.print("<H1>Temperature<br/>");
          client.print(celsius);
          client.print(",");
          client.print(celsiusF);
          client.print(" C</H1>");
          client.print("<br><br>");
          client.print("<br><br>");
          client.print("<H1>LED control</H1>");
          client.print("<A HREF='?cmd=off'>Turn off</A><br>");
          client.print("<A HREF='?cmd=on'>Turn on</A><br><br>");
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
       */   
          
       // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
      
      
      //  Serial.write(c);
      //  if(incoming && c == ' '){
      //    incoming = 0;
      //  }
      //  if(c == '$'){
      //    incoming = 1;
      //  }
      //Checks for the URL string $1 or $2
      //  if(incoming == '1'){
      //    Serial.println(c);
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
/*
        if (c == '1') {
          // send a standard http response header
          Serial.println("Rode LED is ON");
          digitalWrite(led1, HIGH);
          int rd=digitalRead(led1);
          delay(10);
          server.write("ON");
          delay(10);
          Serial.println(rd);
        } 
        
        if (c == '2') {
          // send a standard http response header
          Serial.println("Rode LED is OFF");
          digitalWrite(led1, LOW);
          int rd=digitalRead(led1);
          delay(10);
          server.write("OFF");
          Serial.println(rd);
        delay(10);
         } 
       
 */  
 
    }
          client.println("</html>");
          break;  
      }
       
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
  }


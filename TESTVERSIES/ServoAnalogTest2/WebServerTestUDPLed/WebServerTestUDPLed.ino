/*
 * Web Server
 *
 * A simple web server that shows the value of the analog input pins.
 */

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

int celsius,celsiusF;
int led2 = 3;
int led1 = 4;

int potPIN = 0;  // select the input pin for the potentiometer
int ledPIN = 13; // select the output pin for the LED
int potVAL = 0;  // variable to store the value coming from the potentiometer
char potVALchar = 0;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 };
byte ip[] = { 192, 168, 178, 16 };

EthernetServer server(8032);
EthernetUDP Udp;
unsigned int localPort = 8032; 
boolean incoming = 0;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

void setup()
{
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
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
         //client.print(",");
         //client.print(celsiusF);
         client.print(" C</H1>");
         client.print("<br><br>");
         client.print("<br><br>");
         client.print("<H1>LED control</H1>");
         client.print("<A HREF='1'>Turn off</A><br>");
         client.print("<A HREF='2'>Turn on</A><br><br>");
         
        // client.println();
        // client.println("<!DOCTYPE HTML>");
        // client.println("<html>");

/*
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          
*/          
          
//  if (strcmp(params, "?cmd=on") == 0)
  if (c == '1')
   {
     digitalWrite(led1, HIGH);
     client.print("LED is ON");
    
   }
//  else if (strcmp(params, "?cmd=off") == 0) 
   else if (c == '2') 
   {
     digitalWrite(led1, LOW);
     client.print("LED is OFF");
    }          


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
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
 
  /*
  
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if(incoming && c == ' '){
          incoming = 0;
        }
        if(c == '$'){
          incoming = 1;
        }
      //Checks for the URL string $1 or $2
        if(incoming == 1){
          Serial.println(c);
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == 'a') {
          // send a standard http response header
          Serial.println("Rode LED is ON");
          digitalWrite(led1, HIGH);
          int rd=digitalRead(led1);
          delay(10);
          server.write("ON");
          delay(10);
          Serial.println(rd);
        } 
        
        if (c == 'b') {
          // send a standard http response header
          Serial.println("Rode LED is OFF");
          digitalWrite(led1, LOW);
          int rd=digitalRead(led1);
          delay(10);
          server.write("OFF");
          Serial.println(rd);
          delay(10);
         } 
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

*/

// if there's data available, read a packet
  int packetSize = Udp.parsePacket();

Serial.println(packetSize);
if(packetSize)
  {
   
   
    //Serial.print("Received packet of size ");
    //Serial.println(packetSize);
    //Serial.print("From ");
    //IPAddress remote = Udp.remoteIP();
    //for (int i =0; i < 4; i++)
    //{
    //  Serial.print(remote[i], DEC);
    //  if (i < 3)
    //  {
    //    Serial.print(".");
    //  }
    //}
    //Serial.print(", port ");
    //Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    Serial.println(packetBuffer[0]);
  
    if(packetBuffer[0]=='1'){
      digitalWrite(led1,HIGH);
      //digitalWrite(led2,LOW);
    }else if(packetBuffer[0]=='2'){
       digitalWrite(led1,LOW);
       //digitalWrite(led2,HIGH);
       //delay(2000);
       //digitalWrite(led2,LOW);
    
  
  }

    // send a reply, to the IP address and port that sent us the packet we received
    //Udp.beginPacket(Udp.remoteIP(),Udp.remotePort());
    //Udp.write("WAHh BHurA");
    //Udp.endPacket();
  }
    Udp.beginPacket(ip,8032);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  delay(400);
  client.stop();
}




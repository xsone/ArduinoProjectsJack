/*
 Webclient voor:
 - Besturing CV-radiatoren per ruimte
 - Meten van T en RH per ruimte
 - Registreren van aanwezigheid persoon in ruimte
 - Rookmelder per ruimte
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created Jan. 2015, door Jack Cop
 
 */

/* LET op!!! geen digitale pinnen te gebruiken door combinatie LCD + Ethernet
According DFRobot site, the Ethernet Shield use digital pins 2, 10, 11, 12 & 13; 
it is compatible with the Arduino Ethernet library. 
While the LCD shield use digital pins 5, 6, 7, 8 & 9 and works with the LCB4Bit_Mod library, a modified version of LCD4bits.




//zoomkat 3-25-12
//submit box code
//for use with IDE 1.0
//open serial monitor to see what the arduino receives
//use the \ slash to escape the " in the html 
//address will look like http://192.168.1.102:84 when submited
//for use with W5100 based ethernet shields
//note that the below bug fix may be required
// http://code.google.com/p/arduino/issues/detail?id=605 

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 102 }; // ip in lan
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(84);; //server port

String readString; 
*/

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h> 

float Hum = 0.0;
float Temp = 12.3;
int CVstate = 1;

byte State = LOW;
boolean alreadyConnected = false; // whether or not the client was connected previously

char c;

//CV ketel
// Enter a MAC address and IP address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x00, 0x8E, 0x10 }; // Laatste cijfer MAC steeds 1 ophogen, kamer 1 0x11, 0x12 etc.
byte ip[]  = { 192, 168, 178, 80 };                  // IP-Adres van ethershield CV-ketel; kamers 81, 82 etc.
byte gateway[] = { 192, 168, 178, 1 };               // Gateway is router iP
byte subnet[]  = { 255, 255, 255, 0 };               // standdaard subnet 
unsigned int localPort = 8888;      

EthernetServer server(localPort);
EthernetClient client;

String httpData = "";

void setup() {
   Ethernet.begin(mac,ip,gateway,subnet); //vast IP adres
   server.begin();
  
  //Start Serial
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());
  //EthernetClient client = server.available();
  delay(1000);
}


void loop()
{
 // Stuurt berichten naar server
 
 if (!alreadyConnected) {
   EthernetClient client = server.available();
   client.flush();
   Serial.println("Reset ethershield");
   //client.println("Hello, client!");
   alreadyConnected = true;
  }
 
 
 EthernetClient client = server.available();
 if (client)
  {  
   Serial.println("new client");
   while (client.connected()) 
    {
     //Serial.println("client connected");
      if (client.available()) 
      {
        char c = client.read();
        httpData += c;
        //if (readString.length() < 100)
        // {
        //   readString += c;
        //Serial.println(httpData);
        //Serial.println("client.connected");
        
        //if (c == 'a')
        if ( httpData.substring(5,16) == "?cmd=ACTval")
         {
          Serial.println(httpData);
          client.println("HTTP/1.1 200 OK"); Serial.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");  // JSON response type
          client.println("Content-Type: text/html"); //eerder HTML
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          // open JSON   // example : {"res":"OK","temp":"25.0","pres":"1020","humi":"45"}
          client.print("{");
          client.print("\"res\":\"OK\"");
          client.print(",\"temp\":\"");
          client.print(Temp,1);
          //client.print(",\"setpoint\":\"");
          //client.print(SetPoint);
          //client.print(",\"cvstate\":\"");
          //client.print(CVstate);
          
                   
          //Udp.beginPacket(RemIP, RemPort);
          //Udp.write("CVaan");
          //Udp.endPacket();
          
          //client.print("Temp:     "); client.println(Temp);
          Serial.println(Temp, DEC);  
          //client.print("SetPoint: "); client.println(SetPoint); 
          //Serial.println(SetPoint, DEC); 
          //client.print("CVstate:  "); client.println(CVstate); Serial.println(CVstate, DEC);
          
          
          client.print("\"");
          client.println("}");
          client.println(); 
          c = '?';
          break;
         }         
      } // end available
    } // end while
   delay(1);  //was 200 delay voor browser
   Serial.println("client disonnecting");
   client.flush();
   client.stop();
  } //if client
}

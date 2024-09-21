#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


//char server[] = "www.linku.biz";    // name address for Google (using DNS)
//IPAddress server(192,168,178,144); //Rasp PI
IPAddress server(192,168,178,109); //PC

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,178,45);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Ethernet.init(10);
  Ethernet.begin(mac, ip);
  Serial.println(Ethernet.localIP());
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

//  // if you get a connection, report back via serial:
//  if (client.connect(server, 8000)) {
//    Serial.println("connected");
//    // Make a HTTP request:
//    //client.println("GET /bridge HTTP/1.1"); //werkt
//    client.println("GET / HTTP/1.1"); //werkt
//    client.println("Connection: close");
//    client.println();
//  } 
//  else {
//    // kf you didn't get a connection to the server:
//    Serial.println("connection failed");
//  }
}

void loop()
{
 // Wait until the client sends some data
 while(!client.available()){
   delay(1);
 }

 if (client.connect(server, 8000)) {
   Serial.println("connected with server");
   // Make a HTTP request:
   //client.println("GET /bridge HTTP/1.1"); //werkt
   client.println("GET / HTTP/1.1"); //werkt
   client.println("Connection: close");
   client.println();
//   //if there are incoming bytes available 
//   // from the server, read them and print them:
//   if (client.available()) {
//     char c = client.read();
//     Serial.print(c);
//   }
//   delay(5000);
 } 
 else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
 }
  
//   if there are incoming bytes available 
//   from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  delay(2000);

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}




/*

void loop() {

 
 // Read the first line of the request
 String req = client.readStringUntil('\r');
 Serial.println(req);
 client.flush();
while(client.available()>0)
{
char a=(client.read()-48)*100;
char b=(client.read()-48)*10;
char c=(client.read()-48);
}
p=(a+b+c);
setcolor(p);
   client.flush();
   String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nTHE PWM VALUE is  ";
   s += "</html>\n";
   client.print(P);
}

*/

// A simple web server that always just says "Hello World"

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

int led2 = 7;
int led1 = 6;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 };
byte ip[] = { 192, 168, 178, 15 };

EthernetServer server(8032);

void setup()
{
  Ethernet.begin(mac, ip);
  server.begin();
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  digitalWrite(led1, HIGH);
  delay(2000); 
  digitalWrite(led1, LOW);
  
  digitalWrite(led2, HIGH);
  delay(2000); 
  digitalWrite(led2, LOW);
}


void loop()
{
  EthernetClient client = server.available();
  //char* params;
  char params;
  if (params = client.read())
  {
    if (params == '1')
    {
          digitalWrite(led1, HIGH);
    }
    if (params == '2')
    {
          digitalWrite(led1, LOW);
         
    }
  
  //  if (strcmp(params, "?cmd=3") == 0)
  //  {
   //       digitalWrite(led2, HIGH);
   // }
  //  if (strcmp(params, "?cmd=4") == 0)
  //  {
  //        digitalWrite(led2, LOW);
         
   // }
   
   // e.respond();
   delay(1);
   client.stop();
   }
}

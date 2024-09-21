// A simple web server that always just says "Hello World"

#include "etherShield.h"
#include "ETHER_28J60.h"

#include "WProgram.h"
void setup();
void loop();
int led2 = 7;
int led1 = 6;


static uint8_t mac[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xBB, 0xAA};   // this just needs to be unique for your network,
                                                                // so unless you have more than one of these boards
                                                                // connected, you should be fine with this value.
                                                          
static uint8_t ip[4] = {192, 168, 178, 15};                       // the IP address for your board. Check your home hub
                                                                // to find an IP address not in use and pick that
                                                                // this or 10.0.0.15 are likely formats for an address
                                                                // that will work.
static uint16_t port = 80;                                      // Use port 80 - the standard for HTTP

ETHER_28J60 e;

void setup()
{
    e.setup(mac, ip, port);
   
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
   
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);

 
}

void loop()
{
  char* params;
  if (params = e.serviceRequest())
  {
    if (strcmp(params, "?cmd=1") == 0)
    {
          digitalWrite(led1, HIGH);
    }
    if (strcmp(params, "?cmd=2") == 0)
    {
          digitalWrite(led1, LOW);
         
    }
    if (strcmp(params, "?cmd=3") == 0)
    {
          digitalWrite(led2, HIGH);
    }
    if (strcmp(params, "?cmd=4") == 0)
    {
          digitalWrite(led2, LOW);
         
    }
   
    e.respond();
  }
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


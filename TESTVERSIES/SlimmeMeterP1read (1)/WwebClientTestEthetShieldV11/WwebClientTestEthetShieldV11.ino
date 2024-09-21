// (c) Jack 11/1/2014 LET OP!!

//To use this with a simpler library be sure to use: https://github.com/jcw/ethercard 
//Add the ", 10" to the following initialization line: if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0) 
//As the last parameter, this card uses this pin instead of the default 8. This is poorly documented but works great when done.

// Demo using DHCP and DNS to perform a web ether request.
// 2011-06-08 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];
static uint32_t timer;

const char website[] PROGMEM = "www.google.com";

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}

void setup () {
  Serial.begin(9600);
  Serial.println(F("\n[Arduion Mail Alert]"));

  if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  //if (!ether.dnsLookup(website))
  //  Serial.println("DNS failed");
    
  //ether.printIp("SRV: ", ether.hisip);
  //Serial.println(ether.myip);
  delay(2000);
  if (ether.dnsLookup("smtpcorp.com",2525))
   {
     Serial.println("connected met smtp2go server");
    //sendEmail(subject, message1, message2, tempC);
    
    ether.printIp("EHLO");
        Serial.println("EHLO");
    ether.printIp("AUTH LOGIN");
        Serial.println("AUTH LOGIN");
    ether.printIp("eHNvbmUyQGdtYWlsLmNvbQ==");
        Serial.println("xsone2");
    ether.printIp("c210cDJnbzEyMw==");
        Serial.println("gmail12345");
    ether.printIp("MAIL FROM:<xsone2@gmail.com>");
        Serial.println("MAIL FROM:<jack.cop@ziggo.nl>");
    ether.printIp("RCPT TO:<jack.cop@ziggo.nl>");
        Serial.println("RCPT TO:<jack.cop@ziggo.nl>");
    ether.printIp("DATA");
    ether.printIp("SUBJECT: Arduino Alarm...FIRE...");
    ether.printIp();
    ether.printIp("This is the body.");
    ether.printIp("This is another line of the body.");
    ether.printIp(".");
    Serial.println("Mail verstuurd");
  //etherether.print("MAIL FROM:<xsone2@gmail.com>\nRCPT TO:<xsone2@gmail.com>\nDATA\nFrom: \"Foo\" <xsone2@gmail.com>\r\nTo: xsone2@gmail.com\r\nSubject: MAIL NOTIFICATION\r\n");
  //etherether.print("Content-Type: text/html; name=\"Data_3.csv\"\r\nContent-Disposition: attachment; filename=\"Data_3.csv\"\r\nContent-Transfer-Encoding: base64\r\n\r\n");
    ether.printIp("\r\n.\r\nQUIT\n");
    ether.stop();
   }
  else Serial.println("connection met gmail failed");
}

void loop () {}

/*
void loop () {
  ether.packetLoop(ether.packetReceive());
  
  if (millis() > timer) {
    timer = millis() + 5000;
    Serial.println();
    Serial.print("<<< REQ ");
    ether.browseUrl(PSTR("/foo/"), "bar", website, my_callback);
  }
}
*/

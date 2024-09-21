/*

Tihomir Nedev
April 2014
Change the values for the network setting. 
Change the emails. NB! Some sorvers won't accept email for unedentified devices as Arduino.

Hardware: Arduino board + Ethernet shield


POP address is: pop.gmail.com (64.233.183.109) port number 995
SMTP address is: smtp.gmail.com (66.249.93.111) port number 465 

*/

#include <SPI.h>
#include <Ethernet.h>
#import <Client.h>
#import <Mail.h>
#import <SMTPClient.h>


// Set the panic button pin
byte panicButton = 2;
boolean statusCheck=false; 
// this must be unique
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x59, 0x67 };  
// change network settings to yours
IPAddress ip( 192, 168, 178, 29 );    
IPAddress gateway( 192, 168, 178, 1 );
IPAddress subnet( 255, 255, 255, 0 );

// change server to your email server ip or domain
// IPAddress server( 1, 2, 3, 4 );
char server[] = "smtp.ziggo.nl";
//char server[] = "smtp.gmail.com";


EthernetClient client;
//SmtpClient client(&ethClient, ip);
Mail mail;

void setup()
{
  Serial.begin(9600);
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  Ethernet.begin(mac, ip, gateway, gateway, subnet); 
  delay(2000);
  Serial.println(F("Ready."));
  pinMode(panicButton, INPUT);
  Serial.println(Ethernet.localIP());
}

void loop()
{


  if(digitalRead(panicButton)==HIGH && statusCheck==false)
  {
    Serial.println(F("Alarm..."));
    if(sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"));
		statusCheck = true;
  }
  else if (digitalRead(panicButton)==LOW)
  {
	statusCheck = false; 
  }
}

byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

  //if(client.connect(server,587)) { //gmail
  if(client.connect(server,25)) {  //ziggo
    Serial.println(F("connected"));
  } else {
    Serial.println(F("connection failed"));
    return 0;
  }

  if(!eRcv()) return 0;

// change to your public ip
  //client.println(F("HELO 217.120.27.59"));
  client.println(F("HELO 212.54.34.9"));
  
  if(!eRcv()) return 0;
  Serial.println(F("Sending From"));

// change to your email address (sender)
  client.println(F("MAIL From: jack.cop@ziggo.nl"));

  if(!eRcv()) return 0;

// change to recipient address
  Serial.println(F("Sending To"));
  client.println(F("RCPT To: jack.cop@ziggo.nl"));

  if(!eRcv()) return 0;

  Serial.println(F("Sending DATA"));
  client.println(F("DATA"));

  if(!eRcv()) return 0;

  Serial.println(F("Sending email"));

// change to recipient address
  client.println(F("To: You jack.cop@ziggo.nl"));

// change to your address
  client.println(F("From: Me jack.cop@ziggo.nl"));

  client.println(F("Subject: Panic Alarm!\r\n"));

  client.println(F("The panic button was pressed!!!"));

  client.println(F("."));

  if(!eRcv()) return 0;

  Serial.println(F("Sending QUIT"));
  client.println(F("QUIT"));

  if(!eRcv()) return 0;

  client.stop();

  Serial.println(F("disconnected"));

  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;

  while(!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }

  respCode = client.peek();

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  if(respCode >= '4')
  {
    efail();
    return 0;  
  }

  return 1;
}


void efail()
{
  byte thisByte = 0;
  int loopCount = 0;

  client.println(F("QUIT"));

  while(!client.available()) {
    delay(1);
    loopCount++;

    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }

  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }

  client.stop();

  Serial.println(F("disconnected"));
}

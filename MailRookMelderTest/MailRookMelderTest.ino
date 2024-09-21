#include <SPI.h>
#include <Ethernet.h>
#include <Client.h>
#include <Mail.h>
#include <SMTPClient.h>

EthernetClient ethClient;
SmtpClient client(&ethClient, "smtp.ziggo.nl", 25);

void setup() {
  Serial.begin(9600);

  byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x59, 0x67 };  
  // change network settings to yours
  IPAddress ip( 192, 168, 178, 29 );    
  IPAddress gateway( 192, 168, 178, 1 );
  IPAddress subnet( 255, 255, 255, 0 );
  delay(100);
 
  Serial.println();
  Serial.println("connected");
  Serial.println("preparing email");
  Mail mail;
  mail.from("Some Sender <jack.cop@ziggo.nl>");
  mail.replyTo("jack.cop@ziggo.nl");
  mail.to("Someone <jack.cop@ziggo.nl>");
  mail.to("Someone Else <jack.cop@ziggo.nl>");
  mail.cc("Another <jack.cop@ziggo.nl>");
  mail.bcc("Secret <jack.cop@ziggo.nl>");
  mail.subject("Hello there");
  mail.body("I can send email from an Arduino!");
  Serial.println("sending email");
  client.send(&mail);  
  Serial.println("email sent");
}
void loop() {
}

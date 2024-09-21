
/* Dit programma laat Arduino mail versturen via smtp2go
registratie op http://www.smtp2go.com/
  Settings SMTP2GO
  SMTP server: smtpcorp.com
  SMTP port: 2525
  SMTP Username: xsone2@gmail.com
  SMTP password: smtp2go123
Gecodeerde versie van Username en Password from http://base64-encoder-online.waraxe.us/ or any other base64 encoder you prefer.
*/



#include <SPI.h>
#include <Ethernet.h>

#define time 1000
#define emailInterval 600
#define smtp_port 2525

char message1[35] = "Test1";
char message2[35] = "Test2";
char subject[] = "ARDUINO: TEMPERATURE ALERT!!\0";
unsigned long lastMessage;
float tempC = 0.5;



byte MAC[] = { 0x90, 0xA2, 0xDA, 0x00, 0x8A, 0x13 }; // MAC-ADRES Ethershield zie onderkant
//byte server_mail[] = { "smtp.gmail.com" }; // Mail server address Gmail
//byte smtp[] = {186,47,174,186};
byte smtp[] = { 207,58,147,66 }; // I tried with two ip
//byte smtp[] = { "smtpcorp.com" }; // Mail server address Gmail

//IPAddress server( 74, 125, 65, 27 ); gmail server.
//byte server_mail[] = { 80, 12, 242, 10}; // Mail server address pour ORANGE
//EthernetClient client_mail(server_mail, 25);
EthernetServer server(80);
// byte ip[] = { "smtp.gmail.com" }; // hmailserver IP
//EthernetClient client_mail;
EthernetClient client;

int count = 0;


void setup() {
  Ethernet.begin(MAC); 
  Serial.begin(9600);			// voor debugging
  Serial.println("Mailer gestart...");
  Serial.println(Ethernet.localIP());
  delay(2000);
  //sendEmail(subject, message1, message2, tempC);
  
  if (client.connect("smtpcorp.com",2525))
//  if (client.connect("smtp.gmail.com",25))
   // if (client.connect(smtp,2525))
  //if (client.connect())
  //if (client_mail.connect(server_mail, 587))
   {
     
    Serial.println("connected met smtp2go server");
    //sendEmail(subject, message1, message2, tempC);
    
    client.println("EHLO");
        Serial.println("EHLO");
    client.println("AUTH LOGIN");
        Serial.println("AUTH LOGIN");
    client.println("eHNvbmUyQGdtYWlsLmNvbQ==");
        Serial.println("xsone2");
    client.println("c210cDJnbzEyMw==");
        Serial.println("gmail12345");
    client.println("MAIL FROM:<xsone2@gmail.com>");
        Serial.println("MAIL FROM:<jack.cop@ziggo.nl>");
    client.println("RCPT TO:<jack.cop@ziggo.nl>");
        Serial.println("RCPT TO:<jack.cop@ziggo.nl>");
    client.println("DATA");
    client.println("SUBJECT: Arduino Alarm...FIRE...");
    client.println();
    client.println("This is the body.");
    client.println("This is another line of the body.");
    client.println(".");
    Serial.println("Mail verstuurd");
  //client.print("MAIL FROM:<xsone2@gmail.com>\nRCPT TO:<xsone2@gmail.com>\nDATA\nFrom: \"Foo\" <xsone2@gmail.com>\r\nTo: xsone2@gmail.com\r\nSubject: MAIL NOTIFICATION\r\n");
  //client.print("Content-Type: text/html; name=\"Data_3.csv\"\r\nContent-Disposition: attachment; filename=\"Data_3.csv\"\r\nContent-Transfer-Encoding: base64\r\n\r\n");
    client.print("\r\n.\r\nQUIT\n");
    client.stop();
   }
  else Serial.println("connection met gmail failed");
}

/*
//////////////////////////////////////////////////////////////////////////
//FONCTION SEND EMAIL
void sendEmail(char subject[], char message1[], char message2[], float temp) 
 {
  Serial.println("connecting...");
  //if (client_mail.connect()) 
  //if (client_mail.connect(server_mail, 587))
  if (client.connect("smtp.gmail.com",587))
  {
    Serial.println("connected");
    client_mail.println("EHLO MYSERVER"); delay(time); // log in
    client_mail.println("AUTH LOGIN"); delay(time); // authorise
    // enter your username here
    //Dans le site web ci-dessous
    //http://www.motobit.com/util/base64-decoder-encoder.asp
    // ou http://textmechanic.com/Base64-Converter.html
    
    //CONNNEXION A ORANGE SMTP
    //client_mail.println("caFzLmNvbQaWNZXGluZWVsZWN0cm9uNAZW2FsydGhzd3"); 
    //Take your login and password of Orange smtp, and use a base64 encoder to copy/paste them below e.g. http://www.motobit.com/util/base64-decoder-encoder.asp
    //login in base 64
    //client_mail.println("amVyZW54Z3PPp29saWE="); 
    client_mail.println("eHNvbmUyQGdtYWlsLmNvbQ=="); 
    delay(time);
    // and password in base 64 here
    client_mail.println("Z21haWwxMjM0NQ=="); 
    //client_mail.println("ZnZJh4TYZ2ds"); 
    delay(time);
    
    //MESSAGE
    client_mail.println("MAIL FROM:<xsone2@gmail.com"); delay(time);
    //client_mail.println("RCPT TO:<jadiema@free.fr>"); delay(time);
    client_mail.println("RCPT TO:<xsone2@gmail.com>,<jack.cop@ziggo.nl>"); delay(time);
    
    //client_mail.println("RCPT TO:<jeremigrisolia@orange.fr>"); delay(time);
    client_mail.println("DATA"); delay(time);
    client_mail.println("From: <xsone2@gmail.com>"); delay(time);
    //client_mail.println("To: <jeremigrisolia@orange.fr>"); delay(time);
    client_mail.println("To: <xsone2@gmail.com>,<jack.cop@ziggo.nl>"); delay(time);
    //client_mail.println("To: <jadiema@free.fr>"); delay(time);
    
    client_mail.print("SUBJECT: ");
    //client_mail.println(subject); delay(time);
    client_mail.println(); delay(time);
    //client_mail.println(message1); delay(time);
    //client_mail.println(message2); delay(time);
    //client_mail.print("Temperature: ");
    
    //client_mail.println(temp); delay(time);
    client_mail.println("."); delay(time);
    client_mail.println("QUIT"); delay(time);
    Serial.println("Email sent.");
    lastMessage=millis();
  } 
 else Serial.println("connection failed");
}
*/

void loop() {
}

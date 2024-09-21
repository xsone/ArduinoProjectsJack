//Based on the Arduino Webserver example by David A. Mellis 
and Tom Igoe
//Based on Project 37, Project 46 & Project 48 from 'Beginning Arduino' by Mickael Roberts (Apress) 
//All programs merged by J. Grisolia (jgrisolia@free.fr)

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//DECLARATION GESTION EMAIL
#define time 1000
#define emailInterval 600
//#define HighThreshold 40 // Highest temperature allowed
#define HighThreshold 200 // Highest temperature allowed
#define LowThreshold -200 // Lowest temperature
char message1[35], message2[35];
char subject[] = "ARDUINO: TEMPERATURE ALERT!!\0";
unsigned long lastMessage;

//Un ping sur smtp.free.fr donne 212.27.48.4
//byte server[] = { 62, 234, 219, 95 }; // Mail server address. Change this to your own mail servers IP.
//byte server[] = { 212, 27, 48, 4 }; // Mail server address pour FREE
/*smtp.orange.fr
byte server[] = { 80, 12, 242, 141 }; // Mail server address pour ORANGE
Client client(server, 25);
*/
//smtp-msa.orange.fr
//byte server_mail[] = { 193, 252, 22, 72 }; // Mail server address pour ORANGE
//Client client_mail(server_mail, 587);

//Sometimes the smtp answer:
//Erreur obtenue
//421 mwinf5d30 ME Service refuse. Veuillez essayer plus tard. Service refused, please try later.  OFR404_106 [106]

//This one works for me: 
//smtpauth.orange.fr
byte server_mail[] = { 80, 12, 242, 10}; // Mail server address pour ORANGE
Client client_mail(server_mail, 25);
//Erreur obtenue
//421 mwinf5d30 ME Service refuse. Veuillez essayer plus tard. Service refused, please try later. OFR204_106 [106]

//Here's the right answer from the SMTPAUTH.ORANGE.FR
/*
High Threshhold Exceeded
connecting...
connected
Email sent.
220 mwinf5d14 ME ESMTP server ready
250-mwinf5d14 hello [90.38.125.47], pleased to meet you
250-HELP
250-AUTH LOGIN PLAIN
250-SIZE 44000000
250-ENHANCEDSTATUSCODES
250-8BITMIME
250 OK
334 VXNlcm5hbWU6
334 UGFzc3dvcmQ6
235 2.7.0 ... authentication succeeded
250 2.1.0 <jeremigrisolia@orange.fr> sender ok
250 2.1.5 <jadiema@free.fr> recipient ok
354 enter mail, end with "." on a line by itself
250 2.0.0 Vsou1g00F11Ua9W03soy7N mail accepted for delivery
221 2.0.0 mwinf5d14 ME closing connection
disconnecting.
*/



// GESTION DES CAPTEURS DE TEMPERATURE
//Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 12

//PhotoResistor Pin
int lightPin = 0; //the analog pin the photoresistor is 
                  //connected to
                  //the photoresistor is not calibrated to any units so
                  //this is simply a raw sensor value (relative light)
//LED Pin
int ledPin = 9;   //the pin the LED is connected to
                  //we are controlling brightness so 
                  //we use one of the PWM (pulse width
                  // modulation pins)



float tempC, tempF;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//The result of program #37 from Beginning Arduino (Projet 37_OneWire) gives me the ID of the two Dallas sensors:
/*
Requesting temperatures...DONE
Device Address: 28705A07030000CC Temp C: 23.50 Temp F: 74.30
Device Address: 28EC8E0703000041 Temp C: 23.44 Temp F: 74.19
*/
// arrays to hold device addresses – replace with your sensors addresses
DeviceAddress insideThermometer = { 0x28, 0x70, 0x5A, 0x07, 0x03, 0x00, 0x00, 0xCC };
DeviceAddress outsideThermometer = { 0x28, 0xEC, 0x8E, 0x07, 0x03, 0x00, 0x00, 0x41 };

/*
// arrays to hold device addresses
DeviceAddress insideThermometer = { 0x10, 0x7A, 0x3B, 0xA9, 0x01, 0x08, 0x00, 0xBF };
DeviceAddress outsideThermometer = { 0x10, 0xCD, 0x39, 0xA9, 0x01, 0x08, 0x00, 0xBE};
*/

byte mac[] = {0x48,0xC2,0xA1,0xF3,0x8D,0xB7};//my arbitrary MAC  adress
byte ip[] = {192,168,1,21}; //my arbitrary IP adress included between IP range of Livebox Orange
//byte mac[] = { 0x48, 0xC2, 0xA1, 0xF3, 0x8D, 0xB7 };
//byte ip[] = { 192,168,0, 104 };
// Start the server on port 80
Server server(80);
void setup()
{

// Begin ethernet and server
Ethernet.begin(mac, ip);
server.begin();
// Start up the sensors library
sensors.begin();
// set the resolution
sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
  lastMessage = 0;
  Serial.begin(9600);
  delay(1000);

}



void loop()
{
 int lightLevel = analogRead(lightPin); //Read the lightlevel
 lightLevel = constrain(map(lightLevel, 0, 1023, 0, 255), 0, 255); //adjust the value 0 to 900 to span 0 to 255 and make sure the value is between 0 and 255
 analogWrite(ledPin, lightLevel);  //write the value
 Serial.print("Light Level=");
 Serial.println(lightLevel);
  
sensors.requestTemperatures();
    sensors.requestTemperatures();
    getTemperature(insideThermometer);
    Serial.println(tempC);

//GESTION DES ENVOIS DE MAILS
// Is it too hot?
    if (tempC >= HighThreshold && (millis()>(lastMessage+(emailInterval*1000)))) {
    Serial.println("High Threshhold Exceeded");
    char message1[] = "Temperature Sensor\0";
    char message2[] = "High Threshold Exceeded\0";
    sendEmail(subject, message1, message2, tempC);
    } // too cold?
    else if (tempC<= LowThreshold && (millis()>(lastMessage+(emailInterval*1000)))) {
    Serial.println("Low Threshhold Exceeded");
    char message1[] = "Temperature Sensor\0";
    char message2[] = "Low Threshold Exceeded\0";
    sendEmail(subject, message1, message2, tempC);
    }
    if (client_mail.available()) {checkEmail();}

//GESTION DES ENVOIES DES VALEURS DES CAPTEURS
// listen for incoming clients
Client client = server.available();
if (client) {
// an http request ends with a blank line
boolean BlankLine = true;
while (client.connected()) {
if (client.available()) {
char c = client.read();
// If line is blank and end of line is newline character '\n' = end of HTTP request
  if (c == '\n' && BlankLine) {
      getTemperature(insideThermometer);
      client.println("HTTP/1.1 200 OK"); // Standard HTTP response
      client.println("Content-Type: text/html\n");
      client.println("<html><head><META HTTP-EQUIV=""refresh""CONTENT=""1"">\n");
      client.println("<title>Arduino Web Server by Jadiema</title></head>");
      client.println("<body>\n");
      client.println("<h1>Arduino Web Server by Jadiema</h1>");
      client.print("<h3>Internal Temperature:</h3>");
      client.print(tempC);
      client.print(" (C)");
      client.print("<br/>");
      /*
      client.println("Temp F:");
      client.println(tempF);
      client.println("<br/>");
      */
      getTemperature(outsideThermometer);
      client.print("<h3>External Temperature:</h3>");
      client.print(tempC);
      client.print(" (C)");
      client.print("<br/>");
      /*
      client.println("Temp F:");
      client.println(tempF);
      client.println("<br/>");
      client.println("<h3>Internal Temperature</h3>");
      */
      client.print("<h3>Light level:</h3>");
      client.print(lightLevel);
      client.print("<br/>");
      break;
    }//fin du if c==\n
      if (c == '\n') {
          // Starting a new line
          BlankLine = true;
      }//fin du if
      else if (c != '\r') {
        // Current line has< a character in it
        BlankLine = false; }//fin du elseif
     }//fin du if client available
  }//fin du while
    // Allow time for the browser to receive data
    delay(10);
    // Close connection
    client.stop();
  }//fin du if client
}//fin du void loop


////////////////////////////////////////////////////
// function to get the temperature for a device
void getTemperature(DeviceAddress deviceAddress)
{
tempC = sensors.getTempC(deviceAddress);
tempF = DallasTemperature::toFahrenheit(tempC);
}


//////////////////////////////////////////////////////////////////////////
//FONCTION SEND EMAIL
void sendEmail(char subject[], char message1[], char message2[], float temp) {
Serial.println("connecting...");
if (client_mail.connect()) {
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
client_mail.println("amVyZW54Z3PPp29saWE="); 
delay(time);
// and password in base 64 here
client_mail.println("dX34dH11Ng=="); 
//client_mail.println("ZnZJh4TYZ2ds"); 
delay(time);

//MESSAGE
client_mail.println("MAIL FROM:<jeremigrisolia@orange.fr>"); delay(time);
//client_mail.println("RCPT TO:<jadiema@free.fr>"); delay(time);
client_mail.println("RCPT TO:<jadiema@free.fr>,<robert.grisolia@free.fr>"); delay(time);

//client_mail.println("RCPT TO:<jeremigrisolia@orange.fr>"); delay(time);
client_mail.println("DATA"); delay(time);
client_mail.println("From: <jeremigrisolia@orange.fr>"); delay(time);
//client_mail.println("To: <jeremigrisolia@orange.fr>"); delay(time);
client_mail.println("To: <jadiema@free.fr>,<robert.grisolia@free.fr>"); delay(time);
//client_mail.println("To: <jadiema@free.fr>"); delay(time);

client_mail.print("SUBJECT: ");
client_mail.println(subject); delay(time);
client_mail.println(); delay(time);
client_mail.println(message1); delay(time);
client_mail.println(message2); delay(time);
client_mail.print("Temperature: ");

client_mail.println(temp); delay(time);
client_mail.println("."); delay(time);
client_mail.println("QUIT"); delay(time);
Serial.println("Email sent.");
lastMessage=millis();
} else {
Serial.println("connection failed");
}
}

//////////////////////////////////////////////////////////////////////////
void checkEmail() { // see if any data is available from client_mail
    while (client_mail.available()) {
    char c = client_mail.read();
    Serial.print(c);
    }
    if (!client_mail.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client_mail.stop();
    }
}



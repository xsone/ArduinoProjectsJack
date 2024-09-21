/*
 Web Client voorbveeld

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 22 Sept. 2022
 by Jack Cop
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal_I2C.h>
#include <TriacDimmer.h>




// this is factory config shield initialisation
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 178, 45);

// Initialize the Ethernet client library
const int serverPort = 8000;
EthernetServer serverPI(serverPort);

//Aansturen van regelkring, traploos of met relais?
String versie = "23/9/2022 (c) Jack v4.0";
String energieData = "";
String elecACTverbruik = "";
String elecACTgeleverd = "";


unsigned char sync = 8; //sync pin
unsigned char channel_1 = 9; // channel 1 pin

const int ZCP = 2; //zero cross detection triac
const int relPin = 3; //nummer van realis pin
const int dimPin = 8; //PWM poort van Mega

unsigned int dimValue = 0;
unsigned int mapValue = 0;
float brightness = 0;

unsigned int drempelWaarde = 100; // indien meer dan 100W wordt opgewekt dan gaat 
unsigned int maxLoad = 500; //maximale vermogen van verwarminsgelement of boiler of andere belasting

void setup() {
  //pinMode(ZCP, INPUT);
  pinMode(relPin, OUTPUT);
  //pinMode(dimPin, OUTPUT);
  //digitalWrite(dimPin, LOW);
  digitalWrite(relPin, HIGH);
  delay(1000);
  digitalWrite(relPin, LOW);
  delay(1000);

  TriacDimmer::begin();

  //attachInterrupt(digitalPinToInterrupt(2), ZeroCross, CHANGE);
  
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // try to congifure using IP address instead of DHCP:
  Ethernet.begin(mac, ip);
  
  // Listening for a TCP client Energylogger(from Arduino #1)
  serverPI.begin();
  Serial.print("serverPI is at ");
  Serial.println(Ethernet.localIP());
 
  //Initialiseren van LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(versie);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(Ethernet.localIP());
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
}

void loop() {
  EthernetClient client = serverPI.available();
  if (client) {
    Serial.println("new client");
    while (client.connected()) {
      if (client.available()) {
       energieData = client.readStringUntil('\r');
       int index = energieData.indexOf(';');
       int lengte = energieData.length();
       elecACTverbruik = energieData.substring(0, index);
       elecACTgeleverd = energieData.substring(index+1, lengte-1);
       lcd.setCursor(0,0);  
       lcd.print("                ");
       lcd.setCursor(0,0);
       lcd.print("GEB: " + elecACTverbruik + " Watt");
       lcd.setCursor(0,1);
       lcd.print("                ");
       lcd.setCursor(0,1);
       lcd.print("GEL: " + elecACTgeleverd + " Watt");
       
       
       //client.write("4567"); //t.b.v. android app.

       //float brightness = floatMap(elecACTverbruik.toFloat(), 0, 3000, 0, 1);
       float elecACTverbruikfloat = elecACTverbruik.toFloat();
       float elecACTgeleverdfloat = elecACTgeleverd.toFloat();
       Serial.print("Elec verbruik: ");
       Serial.println(elecACTverbruikfloat);
       Serial.print("Elec geleverd: ");
       Serial.println(elecACTgeleverdfloat);

       serverPI.println("GEB: " + elecACTverbruik + "  GEL: " + elecACTgeleverd); //t.b.v. android app. 
       
       //brightness = floatMap(elecACTgeleverdfloat, 100, 2000, 0, 1);
       if (elecACTgeleverdfloat > 0) brightness = elecACTgeleverdfloat/500;
       else brightness = brightness = floatMap(elecACTverbruik.toFloat(), 0, 2000, 0.25, 0.35); //elecACTverbruikfloat/2000;
       Serial.print("Bright: ");
       Serial.println(brightness);
       TriacDimmer::setBrightness(channel_1, brightness); 

       //float brightness = floatMap(elecACTgeleverd.toFloat(), 100, 2000, 0.25, 0.35);
       //Serial.println("{\"elecACTverbruik\":\"" + String(elecACTverbruik) + "\", \"elecACTgeleverd\":\"" + String(elecACTgeleverd) + "\"}");
       break;
      }
   } 
   delay(1);
   client.stop();
  } 
  if(elecACTgeleverd.toInt() >= drempelWaarde) digitalWrite(relPin, HIGH);
  if(elecACTgeleverd.toInt() < drempelWaarde) digitalWrite(relPin, LOW);

  

//  if (digitalRead(ZCP) == HIGH) {
//    ZeroCross();
////    //dimValue = 5000;
//    dimValue = map(elecACTverbruik.toInt(),100,3000,0,7400); 
//    Serial.println(dimValue); 
//  }
}

//Triac control
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//void ZeroCross(){
//  digitalWrite(dimPin, HIGH);
//  delayMicroseconds(dimValue);  
//  digitalWrite(dimPin, LOW);
//}

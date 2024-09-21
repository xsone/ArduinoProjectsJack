/*
 Web Client voorbveeld

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2020
 by Jack Cop
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>

// this is factory config shield initialisation
LiquidCrystal lcd(8,9,4,5,6,7); 

// below is for shield which has been modified (author's shield)
//LiquidCrystal lcd(8,3,4,5,6,7); 

// initialise the keypad
DFR_LCD_Keypad keypad(A0, &lcd);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 178, 45);

// Initialize the Ethernet client library
const int serverPort = 8000;
EthernetServer serverPI(serverPort);

//Aansturen van regelkring, traploos of met relais?
String energieData = "";
String elecACTverbruik = "";
String elecACTgeleverd = "";

const int ledPin =  4; // nummer van LED pin
const int relPin = 22; //nummer van realis pin
const int dimPin = 46; //PWM poort van Mega

int dimValue = 0;
int mapValue = 0;
int testValue = 0;

unsigned int drempelWaarde = 100; // indien meer dan 100W wordt opgewekt dan gaat 
unsigned int maxLoad = 500; //maximale vermogen van verwarminsgelement of boiler of andere belasting

void setup() {
  //Testen relais aansturing
  pinMode(ledPin, OUTPUT);
  pinMode(relPin, OUTPUT);
  pinMode(dimPin, OUTPUT);
  
  digitalWrite(ledPin, HIGH); //test connector
  digitalWrite(relPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  digitalWrite(relPin, LOW);
  delay(1000);
  
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // try to congifure using IP address instead of DHCP:
  Ethernet.begin(mac, ip);
  
  // Listening for a TCP client Energylogger(from Arduino #1)
//  Serial.print("Buffervat server op: ");
//  Serial.println(Ethernet.localIP());
//  // give the Ethernet shield a second to initialize:
  serverPI.begin();
 
  //Initialiseren van LCD
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(Ethernet.localIP());
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
}

void loop() {
  // Wait for a TCP client from PI #1:
  EthernetClient client = serverPI.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        energieData = client.readStringUntil('\r');
        int index = energieData.indexOf(';');
        int lengte = energieData.length();
        elecACTverbruik = energieData.substring(0, index);
        elecACTgeleverd = energieData.substring(index+1, lengte-1);
//        Serial.print("energieACTverbruik: ");
//        Serial.println(elecACTverbruik);
        lcd.setCursor(0,0);
        lcd.print("               ");
        lcd.setCursor(0,0);
        lcd.print("GEB: ");
        lcd.setCursor(5,0);
        lcd.print(elecACTverbruik);
//        Serial.print("energieACTgeleverd: ");
//        Serial.println(elecACTgeleverd);
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("GEL: ");
        lcd.setCursor(5,1);
        lcd.print(elecACTgeleverd);
        Serial.println("{\"elecACTverbruik\":\"" + String(elecACTverbruik) + "\", \"elecACTgeleverd\":\"" + String(elecACTgeleverd) + "\"}");
        break;
      }
   } 
   delay(1);
  } 
  if(elecACTgeleverd.toInt() >= drempelWaarde) digitalWrite(ledPin, HIGH);
  if(elecACTgeleverd.toInt() < drempelWaarde) digitalWrite(ledPin, LOW);
  //testMap();
}

//Testen traploze regeling
void testMap()
{
 for(int tel=0; tel < 11; tel++)
  {
   //map(value, fromLow, fromHigh, toLow, toHigh)
   mapValue = map(testValue, 0, maxLoad, 0, 255);
   analogWrite(ledPin, mapValue);
   Serial.print("testValue: "); Serial.println(testValue);
   Serial.print("mapValue: "); Serial.println(mapValue);
   testValue += 50;
   delay(1000);
  }
}  

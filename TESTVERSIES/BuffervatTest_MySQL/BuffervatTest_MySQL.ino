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
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


// this is factory config shield initialisation
LiquidCrystal lcd(8,9,4,5,6,7); 

// below is for shield which has been modified (author's shield)
//LiquidCrystal lcd(8,3,4,5,6,7); 

// initialise the keypad
DFR_LCD_Keypad keypad(A0, &lcd);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac_addr_buffervat[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip_addr_buffervat[] = { 192, 168, 178, 45 };
byte server_addr_nas[] = { 192, 168, 178, 20 };    

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
const int serverPort = 88;
EthernetServer TCPserver(serverPort);
EthernetClient client;
MySQL_Connection conn((Client *)&client);

//Mysql MariaDB login
char user[] = "Arduino";              // MySQL user login username
char password[] = "#@Xymox123";        // MySQL user login password
char default_db[] = "Energielogger";   //MySQL database name 

//char energieData[300];
String energieData = "";
unsigned int ElecACTverbruikt = 1111; //Meter reading Electrics - consumption actueel verbruikt
unsigned int ElecACTverbruiktOld = 0;
unsigned int ElecACTgeleverd = 0; 
unsigned int ElecACTgeleverdOld = 0;
const int ledPin =  4; // nummer van LED pin
const int relPin = 22; //nummer van realis pin
const int dimPin = 46; //PWM poort van Mega
int dimValue = 0;
int mapValue = 0;
int testValue = 0;
unsigned int drempelWaarde = 100; // indien meer dan 100W wordt opgewekt dan gaat 
//unsigned int maxLoad = 500; //maximale vermogen van verwarminsgelement of boiler of andere belasting
unsigned int maxLoad = 500; //maximale vermogen van verwarminsgelement of boiler of andere belasting


// Waarden naar Mysql-database
char INSERT_DATA[] = "INSERT INTO buffervat(ELECACTVERBRUIKT, ELECACTGELEVERD) VALUES(%d, %d);";
char query[128];

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(relPin, OUTPUT);
  pinMode(dimPin, OUTPUT);
  
  digitalWrite(ledPin, HIGH); //test connector
  digitalWrite(relPin, HIGH);
   delay(1000);
  digitalWrite(ledPin, LOW);
  digitalWrite(relPin, LOW);
   delay(1000);
  digitalWrite(ledPin, HIGH);
   delay(1000);
  digitalWrite(ledPin, LOW);
   delay(1000);
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // try to congifure using IP address instead of DHCP:
  Ethernet.begin(mac_addr_buffervat, ip_addr_buffervat);
  
  // Listening for a TCP client Energylogger(from Arduino #1)

  if(conn.connect(server_addr_nas, 3307, user, password, default_db)){
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    sprintf(query, INSERT_DATA, ElecACTverbruikt, ElecACTgeleverd);
    cur_mem->execute(query);
    delete cur_mem;
    //num_fails = 0;
    delay(500);
    Serial.println("Connection DB succes...");
   }
 
  Serial.print("Buffervat server op: ");
  Serial.println(Ethernet.localIP());
  // give the Ethernet shield a second to initialize:
  TCPserver.begin();
  //delay(1000);
 
  //Initialiseren van LCD
  lcd.begin(16,2);
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print(Ethernet.localIP());
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);

  //testMap();
}

void loop() {
  // Wait for a TCP client from Energylogger Arduino #1:
  EthernetClient client = TCPserver.available();
  if (client) {
    Serial.print("New client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
          char c = client.read();
          Serial.write(c);
          energieData += c;
        
// substring 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
//           # 0 0 . 0 0 0 ; 0 0 .  0  0  0  ;

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          //Serial.print("EnergieActueelVerbruikt: "); Serial.println(energieData.substring(1,7));
          ElecACTverbruikt = (int) (energieData.substring(1,7).toFloat() * 1000);
          lcd.setCursor(0,0);
          lcd.print("GEB: ");
          lcd.setCursor(5,0);
          lcd.print(energieData.substring(1,7));
          Serial.print("EnergieActueelGeleverd: ");
          Serial.println(energieData.substring(8,14));
          ElecACTgeleverd = (int) (energieData.substring(8,14).toFloat() * 1000);
          lcd.setCursor(0,1);
          lcd.print("LEV: ");
          lcd.setCursor(5,1);
          lcd.print(energieData.substring(8,14));
         } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
       }
    //Ethernet.maintain();
     }
  } 
    delay(1);
    client.stop();
    energieData = "";
    //lcd.clear();
    Serial.println("client disconnected");
  }  
  //if(ElecACTverbruikt >= drempelWaarde) digitalWrite(ledPin, HIGH);
  //if(ElecACTverbruikt < drempelWaarde) digitalWrite(ledPin, LOW);

  logVerbruik();
}

void logVerbruik()
{
  Serial.print("EnergieActueelGeleverd: "); Serial.println(ElecACTgeleverd);
  Serial.print("Drempelwaarde: "); Serial.println(drempelWaarde);
  if(ElecACTgeleverd >= drempelWaarde){
  //if(ElecACTverbruikt >= drempelWaarde){  
    Serial.println("Led ON"); 
    digitalWrite(ledPin, HIGH);
    digitalWrite(relPin, HIGH);
    mapValue = map(ElecACTgeleverd, 0, maxLoad, 0, 255);
    //mapValue = map(ElecACTverbruikt, 0, maxLoad, 0, 255); 
    analogWrite(dimPin, mapValue);
    Serial.print("ElecACTverbruikt: "); Serial.println(ElecACTverbruikt);
    Serial.print("mapValue: "); Serial.println(mapValue); 
  }
  if(ElecACTgeleverd < drempelWaarde){
    Serial.println("Led OFF"); 
    digitalWrite(ledPin, LOW);
    digitalWrite(relPin, LOW);
  }
  
  //if(ElecACTverbruikt < drempelWaarde) digitalWrite(ledPin, LOW);

  if (ElecACTverbruikt != ElecACTverbruiktOld || ElecACTgeleverd != ElecACTgeleverdOld){
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    sprintf(query, INSERT_DATA, ElecACTverbruikt, ElecACTgeleverd);
    cur_mem->execute(query);
    delete cur_mem;
    //num_fails = 0;
    delay(500); 
    ElecACTverbruiktOld = ElecACTverbruikt;
    ElecACTgeleverdOld = ElecACTgeleverd;
  } 
    
  
//  if (ElecACTgeleverd != ElecACTgeleverdOld){
//    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
//    sprintf(query, INSERT_DATA, ElecACTverbruikt, ElecACTgeleverd);
//    cur_mem->execute(query);
//    delete cur_mem;
//    //num_fails = 0;
//    delay(500); 
//  }  
}


void testMap()
{
 for(int tel=0; tel < 11; tel++)
  {
   mapValue = map(testValue, 0, maxLoad, 0, 255);
   analogWrite(dimPin, mapValue);
   Serial.print("testValue: "); Serial.println(testValue);
   Serial.print("mapValue: "); Serial.println(mapValue);
   testValue += 50;
   delay(5000);
  } 
}  

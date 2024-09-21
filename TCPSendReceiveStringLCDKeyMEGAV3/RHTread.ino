// Badkamer ventilator regeling met DHT library
// (c) Jack Cop 3/12/2014
// version 1.0

//#include "DHT.h"
//#define DHTPIN 24     // what pin we're connected to op MEGA

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 (besteld bij dealextreme.com
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor for normal 16mhz Arduino


//#define dht_dpin A0 //was A5
//#define dht_dpin 2 //was A5


//const int ledPin =  13;
//char HumiChar[10] = "0";
//char TempChar[10] = "0";
//char CalcChar[10] = "0"; //omrekenwaarde naar analog output
int CalcInt = 0;
int Teller = 0;
int Timer = 0;
int StopTime = 900; //stopt na 15 min.
int State = 0;
int MinHum = 60;
int Hum = 0;
float Temp = 0;




//void setup()
//{
 
 
 //pinMode(rxPin, INPUT);
 //pinMode(txPin, OUTPUT);
 //pinMode(ledPin, OUTPUT); 
 //digitalWrite(ledPin, LOW);

 //Serial.println("Humidity and temperature start...");
 //  mySerial.println("Humidity and temperature start...");
 //delay(500);
 //dht_dat[0] = 60;
 //CalcInt = 75;
//}



void RHTread()
{
  Hum = dht.readHumidity(); //lees RH uit in %, was orgineel een float waarde
  Temp = dht.readTemperature(); //lees temperatuur uit in C, was ook een float
     
  //CalcInt = (5.1*dht_dat[0])-153;  //30% = low (0); 80% = high (255); werkt goe
  //CalcInt = (25.5*dht_dat[0])-1785; //test in bereik van 70% tot 80% RH
  //CalcInt = (9*dht_dat[0])-465;  // <=60% = rust 3V (75); 95% = high 10V (255)
  //CalcInt = (6*Hum-285;  // <=60% = rust 3V (75); 90% = high 10V (255)
  //CalcInt = (18*Hum - 1455);  // <=85% = rust 3V (75); 95% = high 10V (255)
  //CalcInt = (9*Hum - 600);  // <=75% = rust 3V (75); 95% = high 10V (255)
   
  if (Teller == 20)  // eerst 20 metingen doen i.v.m. pendelen
   {
    CalcInt = (6*Hum - 285);  // <=75% = rust 3V (75); 95% = high 10V (255)
    //analogWrite(anout_pin, 255-CalcInt);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
    Teller = 0;
   } 
   Teller++;  
  
    
    //if (dht_dat[0] <= 60) CalcInt = 75; //minimum draaistand van 3V is 75bits
  if (CalcInt >= 255) CalcInt = 255;
  if (CalcInt <= 0) CalcInt = 0;

  if (Hum <= MinHum) //ventilator slaat af.
   {     
    CalcInt = 75;
    State = 0;
    Timer = 0;
    //digitalWrite(ledPin, LOW);
   } 
    
  if (Hum > MinHum) //ventilator slaat aan en timer gaat lopen
   {
    State = 1;
    Timer++;
    //digitalWrite(ledPin, HIGH);
   }
     
  if (Timer >= StopTime && State == 1)  //na een bepaalde periode "StopTime" wordt altijd gestopt en ventilator in laagste stand gezet.
   {
     CalcInt = 75; 
     //digitalWrite(ledPin, LOW);
     Timer = 0;
     State = 0;
   } 
  analogWrite(anout_pin, 255-CalcInt);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  DisplayData();
}


void DisplayData()
{
  Serial.print("Humidity: ");
  //Humidity = String (Hum);
  Serial.print(Hum);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  //Temperatuur = String (Temp);
  Serial.print(Temp);
  Serial.print(" *C\t");
  Serial.print(Timer,DEC);
  Serial.print("\t");
  Serial.println(CalcInt,DEC);
  delay(1000);  //delay van 1 sec.
}



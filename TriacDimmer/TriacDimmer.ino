// Arduino Triac Dimmer
// version 1
// sept. 2022 (c) Jack Cop

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

const int ZCP = 2;
unsigned int dim = 0;
const int dimPin = 8;
int data = 0;


void setup() {
 Serial.begin(9600);
 Serial.println("Triac Dimmer gestart...");
  
 pinMode(ZCP, INPUT);
 pinMode(dimPin, OUTPUT);
 digitalWrite(dimPin, LOW);

 lcd.init(); // initialize the lcd
 lcd.backlight();

 lcd.setCursor(0, 0);         // move cursor to   (0, 0)
 lcd.print("Arduino");        // print message at (0, 0)
 lcd.setCursor(2, 1);         // move cursor to   (2, 1)
 lcd.print("GetStarted.com"); // print message at (2, 1)
}

void loop() {
  
  
  if (digitalRead(ZCP) == HIGH) {
    ZeroCross();
    int data = analogRead(A0);
    Serial.print("potmeter value: ");
    Serial.println(data); // waarden van potmeter
    //Serial.print("Elec: ");
    //Serial.println(data);
    //delay(1000);
    //if(data >= 3000) data = 0;
    dim = map(data,240,1023,10,49); 
    Serial.print("Dim: ");
    Serial.println(dim); 
  }
}

void ZeroCross(){
  int dimming_time = (200 * dim);
  digitalWrite(dimPin, LOW);
  delayMicroseconds(dimming_time);
  //delayMicroseconds(10);  
  digitalWrite(dimPin, HIGH);
}

/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 Modified 16 Jun 2009
 By Tom Igoe
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */
// include the library code:
#include <LiquidCrystal.h>
#include <LCDKeypad.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LCDKeypad lcd;



int sensorPin = 3;    // select the input pin for the potentiometer
int ledPin = 13;     // select the pin for the LED
unsigned int SensorValue = 0;  // variable to store the value coming from the sensor
unsigned int oldSensorValue = 0;
unsigned int teller = 0;

void setup() 
{
  Serial.begin(9600);  
  pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2); 
  lcd.clear();
  lcd.print("...KWH teste...");
  lcd.setCursor(0,1);
  lcd.print("...IR-sensor...");
  delay(2000);
  lcd.clear();
}


void loop() 
{
  SensorValue=analogRead(sensorPin);
  //if(SensorValue>950&&SensorValue<1050) 
   //{
    //digitalWrite(ledPin, LOW);
   //} 
  Serial.print("SensorValue: ");
  Serial.println(SensorValue);
  
  //if(SensorValue>1000){Serial.print("ZWART: "); Serial.println(SensorValue);elay(150);digitalWrite(ledPin,LOW);}
 //if(SensorValue>=980&&SensorValue<1000)
 if (SensorValue >= (oldSensorValue + 5))
 //if(SensorValue>1000)
   {
    
   // Serial.print("SPIEGEL---");
    lcd.setCursor(0,0);
    //lcd.print("STREEP");
    lcd.print(teller++);
    lcd.setCursor(0,1);
    lcd.print(SensorValue);
    Serial.println("STREEP");
    //Serial.println(SensorValue);
 //   digitalWrite(ledPin,HIGH);
    
    //lcd.setCursor(0,1);
    //lcd.print("      ");
  }
  //else 
  // {
  
  //delay(500);
  //lcd.setCursor(0,0);
  //lcd.print("      ");
  lcd.setCursor(0,1);
  lcd.print(SensorValue);
  lcd.print("      ");
  //lcd.clear();
  // } 
  Serial.print("oldSensorValue: ");
  Serial.println(oldSensorValue);
  oldSensorValue = SensorValue; 
 
 
     
  //if(SensorValue>850&&SensorValue<950){Serial.print("SPIEGEL: ");Serial.println(SensorValue);digitalWrite(ledPin,HIGH);}
  //if(SensorValue>980&&SensorValue<1000){Serial.print("SPIEGEL: ");Serial.println(SensorValue);digitalWrite(ledPin,HIGH);}
 // else { Serial.print("STREEP----------------"); Serial.println(SensorValue);digitalWrite(ledPin, LOW);}
  //delay(50);
  //   if(SensorValue>950){Serial.print("STREEP: ");Serial.println(SensorValue);digitalWrite(ledPin,HIGH);}
}

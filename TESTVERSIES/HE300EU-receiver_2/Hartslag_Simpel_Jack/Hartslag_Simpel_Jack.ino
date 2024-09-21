int sensorPin = 0;
int heartBeat = 0;
int heartBeatTest = 0;

int sensorTemp = 0;
int sensorCal = 0;
int calVal = 0;

unsigned long timer;
unsigned long interval = 10000; // 10 seconder
unsigned long lastTime = 0;
boolean rising = false;
boolean falling = false;

void setup() {
 Serial.begin(9600);
 
 for (calVal=0; calVal<=5; calVal++){
  sensorTemp += analogRead(sensorPin);
  Serial.print("SensorTemp: ");
  Serial.println(sensorTemp);
  delay(1000);
 }
 sensorCal =  sensorTemp/calVal;
 Serial.print("Sensor calibratie waarde: ");
 Serial.println(sensorCal);
 delay(2000);
   
   //Calibratie: bepaal maximum sensorwaarde
   //Doe verschil van ongeveer 50
   
}

void loop ()
{
     if (analogRead(sensorPin)>= sensorCal && (rising == false)) {
        heartBeat++;
        heartBeatTest = 500; //laat in plotter puls zien
        rising = true;
     }   
     if (analogRead(sensorPin) < (sensorCal - 11) && (rising == true)) {
        rising = false;
        heartBeatTest = 0;
     }   
      //timer += millis();
      
      if(millis() - lastTime > interval) {
        lastTime = millis();  //timer die loopt tot interval
        Serial.print("BPM: ");
        Serial.println(heartBeat*6); //keer 6 voor omrekenen naar minuut
        heartBeat = 0; //na 1 minuut weer opnieuw beginnen
      }
     //Serial.print(",");                
     //Serial.print(heartBeatTest);      //test voor plotter puls
     //Serial.print(",");                //seperator voor 2 waarden in plotter
     //Serial.println(analogRead(sensorPin)); //laat signaal van sensor zien
     //delay(500);
} 


volatile long int ElecKwhWaarde = 0;
int ElecIntWaarde = 0;
int ElecSensorWaarde = 0;
int ZwartWaarde=0; //detectie zwarte vlakje KWh
int SensorLed = 13;


void setup()
{
  pinMode(13, OUTPUT);  //LED indicator KWh meter
  
  digitalWrite(SensorLed, LOW); //KWh meter
  
  Serial.begin(9600);			// voor debugging
  Serial.println("Starting KWh-sensor test...");
  
  digitalWrite(SensorLed, HIGH); //KWh meter
  delay(3000);
  digitalWrite(SensorLed, LOW); //KWh meter
  
  Serial.println("KWh-sensor programma gestart...");
}



// Uitlezen KWh sensor
void ElecRead()
{
 ElecSensorWaarde=analogRead(0);
 //if(ElecSensorWaarde>850&&ElecSensorWaarde<1000)
 if(ElecSensorWaarde>980&&ElecSensorWaarde<1000)
  {
   digitalWrite(SensorLed, HIGH);
   if(ZwartWaarde==1)
    {
     ElecIntWaarde++;  //bij "zien" van zwarte streepje
     ZwartWaarde=0;
     return;
    } 
  } 
 else
  {
   digitalWrite(SensorLed, LOW);
   ZwartWaarde=1;
   return;
  } 
 if(ElecIntWaarde==600) //600 omwentelingen = 1kWh
  {
   ElecKwhWaarde++;
   ElecIntWaarde=0;
   return;
  }
 if(ElecKwhWaarde==99999) ElecKwhWaarde=0; //KWHmeter Max. waarde bereikt
 return;
}

void loop()
{
 ElecRead();
 Serial.print("Teller: ");
 Serial.print(ElecIntWaarde, DEC);
 Serial.print("\tSensorWaarde: ");
 Serial.println(ElecSensorWaarde, DEC);
 //delay(500);
} 

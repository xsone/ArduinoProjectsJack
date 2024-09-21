//Voltcrdaf KWh meter op zonnepanelen.
volatile int state = LOW;
volatile unsigned long int ZonKWhWaarde = 0;
volatile unsigned long int ZonPulsWaarde = 0;
int InteruptTellerZon = 0;

//char ZonChar[10] = "0";
//String ZonChar = "0";
//char ZonKWhChar[10] = "0";


/*
  
}
  pinMode(13, OUTPUT);  //LED indicator KWh meter
  digitalWrite(13, LOW); //KWh meter
  delay(2000);
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);
}
*/

// Uitlezen Kwhsensor  
void ZonRead()
{
 ZonPulsWaarde++;
 digitalWrite(13, HIGH);   
 ZonPuls = String(ZonPulsWaarde);
 digitalWrite(13, LOW);
 
 //Laat waarden zien
 Serial.print("ZonPuls: ");
 Serial.println(ZonPuls);
 //lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("ZonPuls: ");
 lcd.print(ZonPuls);

 if(ZonPulsWaarde==1000) //1000 S0-pulsen is 1Kwh Zonneenergie
  {
   ZonKWhWaarde++;
   ZonKWh = String(ZonKWhWaarde);

   //Laat waarden zien
   Serial.print("ZonKWhTeller:  ");
   Serial.println(ZonKWh);
   lcd.setCursor(1,0);
   lcd.print("ZonKWh: ");
   lcd.print(ZonKWh);
  } 
 if(ZonPulsWaarde==999999) ZonPulsWaarde=0; //KWhmeter Max. waarde bereikt
}

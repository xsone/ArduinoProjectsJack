//Voltcrdaf KWh meter op zonnepanelen.
volatile unsigned long int ZonKWhWaarde = 0;
volatile unsigned long int ZonPulsWaarde = 0;

boolean solarInput = false;
boolean sensorIsOn = false;

int S0pin = 3;

// Uitlezen Kwhsensor via S0 output + komt aan digital input Arduino
void ZonRead()
{ 
 solarInput = digitalRead(S0pin); // rising edge?
 if(solarInput && sensorIsOn == false) sensorIsOn=true; // falling edge?
 if(!solarInput && sensorIsOn == true)
  {
   ZonPulsWaarde++;
   sensorIsOn = false;
   Serial.print("ZonPuls: ");
   Serial.println(ZonPulsWaarde, DEC);
  }  
 if(ZonPulsWaarde==1000) //1000 S0-pulsen is 1Kwh Zonneenergie
  {
   ZonKWhWaarde++;
   ZonKWh = String(ZonKWhWaarde);
   Serial.print("ZonKWhTeller:  ");
   Serial.println(ZonKWh);
   ZonPulsWaarde = 0;
  } 
 if(ZonPulsWaarde==999999) 
  {
   ZonKWhWaarde = 0; //KWhmeter Max. waarde bereikt 
   ZonPulsWaarde = 0;
  } 
} 

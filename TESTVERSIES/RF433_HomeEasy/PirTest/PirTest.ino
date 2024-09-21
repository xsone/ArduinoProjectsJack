/* Arduino Night Alarm with Warning light
* T.K.Hareendran
* www.electroschematics.com/
*/
#define NOMOVE 10 //testen met 5 sec.
#define WELMOVE 5 //testen met 10 sec.
#define NALOOP 35 //testen met 5 sec. + 25 sec delay voordat weer aan kan.
#define PIRDELAY 25 //vertraging na uitschakelen PIR-sensor

int TimerAan = 0;
int TimerUit = 0;
int AanTeller = 0;
int UitTeller = 0;
const int pirPin = 3;
const int stateLed = 13;
byte pirState = LOW;  // status van pir-bewegingssensor


void setup() {
    pinMode(pirPin, INPUT); // Sensor pin as input
    pinMode(stateLed, OUTPUT); // Sensor pin as input
    digitalWrite(stateLed, HIGH);
    delay(1000);
    digitalWrite(stateLed, LOW);
    delay(1000);
    Serial.begin(9600);
    Serial.println("...Start ventilator...");
    Serial.println();
}

//Bewegingssensor
void loop()
{
 if(digitalRead(pirPin) == LOW)  //Wel Move
  {
   digitalWrite(stateLed, HIGH);
   Serial.print("Vent AAN: ");
   Serial.println(TimerAan, DEC);
   TimerUit = 0;
   AanTeller++;
   TimerAan++;
  } 

 if(digitalRead(pirPin) == HIGH)  //No Move
  {
   digitalWrite(stateLed, LOW);
   Serial.print("Vent UIT: ");
   Serial.println(TimerUit, DEC);
   TimerUit++;
  } 

 if (TimerAan > PIRDELAY)
  {
    AanTeller = 1;
  }

 if (AanTeller == 1) //vertraagt inschakelen
  {
   digitalWrite(stateLed, HIGH);
   Serial.print("Vent AAN: ");
   Serial.println(TimerAan, DEC); 
  }
  

 if (TimerUit > NALOOP + PIRDELAY)
  {
   digitalWrite(stateLed, LOW);
   Serial.print("Vent UIT: ");
   Serial.println(TimerUit, DEC);
   TimerUit = 0;
   TimerAan = 0;
   AanTeller = 0;
  }

 Serial.print("Timer AAN: ");
 Serial.println(TimerAan, DEC);



/* 
  if(digitalRead(pirPin) == HIGH && pirState == LOW) 
   {
    pirState = HIGH;
    digitalWrite(stateLed, LOW); //geinverteerd door transistor
    //Timer = 0;
   }
  if(digitalRead(pirPin) == LOW && pirState == HIGH)
   {
    pirState = LOW;
    digitalWrite(stateLed, HIGH);
    //Timer++;
   }
*/

 /*
 if( TimerUit >= NOMOVE ) //No Move
  {
   digitalWrite(stateLed, LOW); //geinverteerd door transistor
   pirState = HIGH;
   //Serial.print("pirState0");
   Serial.println("...Vent UIT (Naloop?) ...");
   TimerAan = 0;
   TimerUit = 0;
  } 

 if( TimerAan >= WELMOVE ) //Wel Move start douchebeurt
  {
   digitalWrite(stateLed, HIGH); //geinverteerd door transistor
   //digitalWrite(motorLed, HIGH); //Motor aan
   pirState = HIGH;
   //Serial.print("pirState0");
   Serial.println("...Motor AAN douchbeurt start...");
  } 
*/

/* 
 if(digitalRead(pirPin) == LOW && pirState == HIGH)  //Wel Move
  {
   digitalWrite(stateLed, HIGH);
   pirState = LOW;
   TimerUit = 0;
   Serial.println();
   //Serial.println("...Vent AAN...");
   Serial.print("TimerAan: ");
   AanTeller++;
   if (AanTeller > 1)
    {
     Serial.println();
     Serial.println("...Vent AAN...");
     AanTeller = 0;
    }
   //TimerAan++;
  } 

 if(digitalRead(pirPin) == HIGH && pirState == LOW)  //No Move
  {
   digitalWrite(stateLed, LOW);
   pirState = HIGH;
   TimerAan = 0;

   /*
   if (AanTeller > 5)
    {
      UitTeller++;
      if (UitTeller > 5)
       {
        Serial.println();
        Serial.println("...Vent UIT...");
        AanTeller = 0;
       }
    }
   */ 
  // Serial.print("TimerUit: ");
   //TimerUit++;
 // } 

 

/*
 if (pirState == LOW)
  {
  
   Serial.print(TimerAan++, DEC);
  }

 if (pirState == HIGH)
  {
  
   Serial.print(TimerUit++, DEC);
  }
 */ 
 /*
 if(digitalRead(pirPin) == LOW && Timer >= TIMER2) 
  {
   digitalWrite(stateLed, HIGH);
   pirState == LOW;
   Serial.println("Naloop...");
   //Timer = 0;
   //NaLoop(kwartier)
  }
*/
//  if (Timer >= TIMER1) Serial.print("Timer: "); Serial.println(Timer, DEC); //HalfSpeed
//  if (Timer >= TIMER2) Serial.print("Timer: "); Serial.println(Timer, DEC); //FullSpeed
   
 //digitalWrite(stateLed, HIGH);
 //delay(1000);
 //digitalWrite(stateLed, LOW);
 
 //Serial.print("pirState: ");
 //Serial.println(pirState, DEC);
/*
 Serial.print("TimerAan: ");
 Serial.println(TimerAan, DEC);
 Serial.print("TimerUit: ");
 Serial.println(TimerUit, DEC);
*/ 
 
 delay(1000); //timer van 1 sec.
}

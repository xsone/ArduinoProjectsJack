// Sensor reading
#include "WProgram.h"
void setup();
void loop();
volatile long int GasIntWaarde=0;
volatile long int GasM3Waarde=0;
int InteruptTellerGas = 0;
int InteruptTellerWater = 0;


// Uitlezen Gassensor  
/*
void GasRead()
{
  if(InteruptTellerGas==2)
     {
       GasIntWaarde++;
       digitalWrite(13, HIGH);
       delay(200);
       digitalWrite(13, LOW);
       InteruptTellerGas=0;
       if(GasIntWaarde>1000) //1000 pulsen = 1M3
        {
         GasM3Waarde++;
         GasIntWaarde=0;
        } 
     }
  else InteruptTellerGas++; 
  digitalWrite(13, LOW);
}
*/


void setup()
{
  pinMode(3, INPUT);    //sensorinput Gas INT1
  pinMode(13, OUTPUT);  //testLED output op pin 13
  //attachInterrupt(1, GasRead, CHANGE); //interrupt digital pin 3
}

void loop() {
  if(digitalRead(3)==LOW){
       digitalWrite(13, HIGH);
       delay(200);
       digitalWrite(13, LOW);
 }
  
}


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}


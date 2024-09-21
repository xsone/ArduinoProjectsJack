#include <AltSoftSerial.h>
#include <SPI.h>

// AltSoftSerial always uses these pins:
//
// Board          Transmit  Receive   PWM Unusable
// -----          --------  -------   ------------
// Teensy 2.0         9        10       (none)
// Teensy++ 2.0      25         4       26, 27
// Arduino Uno        9         8         10
// Arduino Mega      46        48       44, 45
// Wiring-S           5         6          4
// Sanguino          13        14         12

AltSoftSerial altSerial;

int msg[1];
const uint64_t pipe = 0xE8E8F0F0E1LL;

const int requestPin =  4;         
char input; // incoming serial data (byte)
bool readnextLine = false;
#define BUFSIZE 75
char buffer[BUFSIZE]; //Buffer for serial data to find \n .
int bufpos = 0;
long mEVLT = 0; //Meter reading Electrics - consumption low tariff
long mEVHT = 0; //Meter reading Electrics - consumption high tariff
long mEAV = 0;  //Meter reading Electrics - Actual consumption
long mEAT = 0;  //Meter reading Electrics - Actual generated electricity (Solar panels)
long mG = 0;   //Meter reading Gas
int tarief = 3; //Meter actual rate

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Serial oke");
  altSerial.begin(115200);
  delay(1000);
  Serial.println("AltSerial oke");

}


void loop() {
  long tl = 0;
  long tld =0;
  

  if (altSerial.available()) {
    input = altSerial.read();
    char inChar = (char)input;
    // Fill buffer up to and including a new line (\n)
    buffer[bufpos] = input&127;
    bufpos++;

    if (input == '\n') { // We received a new line (data up to \n)


      // 1-0:1.8.2 = Electricity consumption high tariff (DSMR v4.0)
      if (sscanf(buffer,"1-0:1.8.2(%ld%.%ld%*s" , &tl, &tld) >0 ) {
        tl *= 1000;
        tl += tld;
        mEVHT = tl;    
        //mEVHT = tl * 1000 + tld;
        if (mEVHT > 0) {

          //mEVHT = 0;
        }
      }

        if (sscanf(buffer,"0-0:96.14.0(%ld.%ld*s" , &tl) >0  )   //)
        {
          tarief = tl;
        }


      if (sscanf(buffer,"1-0:1.8.1(%ld.%ld" ,&tl, &tld)==2){
        tl *= 1000;
        tl += tld;
        mEVLT = tl;

        if (mEVLT > 0) {


        }
      }

      // 1-0:1.7.0 = Electricity consumption actual usage (DSMR v4.0)
      if (sscanf(buffer,"1-0:1.7.0(%ld.%ld" ,&tl , &tld) == 2)
      { 
        mEAV = (tl*1000)+tld;
        if (mEAV > 0) {

        }
      }

      // 0-1:24.2.1 = Gas (DSMR v4.0) on Kaifa MA105 meter
      if (strncmp(buffer, "0-1:24.2.1", strlen("0-1:24.2.1")) == 0) {
        if (sscanf(strrchr(buffer, '(') + 1, "%d.%d", &tl, &tld) == 2) {
          mG = (tl*1000)+tld; 

        }
      }

      // Empty buffer again (whole array)
      for (int i=0; i<75; i++)
      { 
        buffer[i] = 0;
      }
      bufpos = 0;

    }
     
    if (input == '!') {   //uitroepteken geeft einde van telegram aan, dus we gaan data versturen
      //SendNrf();
      printData();
      mEVHT=0;
      mEVLT = 0;
      mEAV = 0;
      mG = 0;
      tarief=3;
      //client.stop();
    } //Einde vraagteken detectie   

  } //Einde 'if AltSerial.available'

}
void printData(){
  Serial.println("********* BEGIN LOOP *********");  
  Serial.print("Elektra - meterstand verbruik HOOG tarief (Wh): ");
  Serial.println(mEVHT);  
  Serial.print("Elektra - meterstand verbruik LAAG tarief (Wh): ");
  Serial.println(mEVLT);  
  Serial.print("Elektra - actueel verbruik (W): ");
  Serial.println(mEAV);
  Serial.print("Gas - meterstand (liters): ");
  Serial.println(mG);

  Serial.print("Huidige tarief: ");
  if (tarief==1){
    Serial.println ("dal");
  }
  else {
    Serial.println("hoog");
  }
//  Serial.println(tarief);
  Serial.println("");
}

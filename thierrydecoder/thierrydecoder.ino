#include <VirtualWire.h>

// Jaspers Thierry Mugler Decoder
// Code ported from http://roel.reijerse.net/thierry/
// Hardware: - Arduino Uno
//           - Thierry Mugler wireless weather station
//           - 433MHz receiver (SZSAW CZS-3) on digital pin 2

// (c) nov 2015 test jack

#include "thierrydecoder.h"
#include <VirtualWire.h>

const int led_pin = 13;
const int receive_pin = 11; //waas 12

volatile unsigned long tstart;   
volatile unsigned long tlength; 
volatile unsigned char Td, Tt, TT;//temperature
volatile unsigned char hh, hH;	  //relative humidity 
volatile int ContentHandled;
unsigned char thnbuf,nzeroes;
char thbuffer[40];
enum State state; 

void setup()
{ 
  Serial.begin(9600);
  Serial.print("Start Thierry (action) weerstation decoder");
  delay(1000);
  
  vw_set_rx_pin(receive_pin);
  //vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(4000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
  pinMode(led_pin, OUTPUT);
  
  //attachInterrupt(0,isr_pinchange,CHANGE);  //interrupt pin0=dig pin2
}

void isr_pinchange()        // capture interrupt
{
  digitalWrite(led_pin, HIGH); //jcp
  
  if(digitalRead(2)==HIGH)  // rising edge detected
  {
    Serial.println("INT detected");
    tstart=micros();        // store time since start of program
  }
  else                      // falling edge detected
  {    
    tlength=micros()-tstart;// measure time between rising and falling edge
    handlePulse(tlength);
  }
}

void loop()
{
  if (ContentHandled==1)
  {
    ContentHandled=0;
    Serial.print("Temp:");
    Serial.print(TT);
    Serial.print(Tt);
    Serial.print(".");
    Serial.print(Td);
    Serial.print("dC RH:");
    Serial.print(hH);
    Serial.print(hh);
    Serial.println("%");  
    digitalWrite(led_pin, LOW); //jcp
  }
}

void handlePulse(int len)//feds a pulse into the finite state machine
{
  char c = decodePulseLength(len);//gives interpretation to pulse length
  switch (state)
  {		
  case Idle:
    if (c == '0')
      setState(Preamble);
    break;
  case Preamble:
    if (c == 'S' && nzeroes >= 6) // 6 PREAMBLE_ONES
      setState(Content);
    else
      if (c == '0')
        nzeroes++;
      else
        state = Idle;
    break;
  case Content:
    if (c == '0' || c == '1')
    {
      thbuffer[thnbuf++] = c;
      if (thnbuf == 40)
      {
        handleContent();
        setState(Idle);
      }
    }
    else
    {
      setState(Idle);
    }
    break;
  default:
    state=Idle;
    break;
  }
}

char decodePulseLength(int l) //gives interpretation to pulse length
{
  if (l >= S_MIN && l < S_MAX)
  {
    return 'S';
  }
  if (l >= Z_MIN && l < Z_MAX)
  {
    return '0';
  }
  if (l >= O_MIN && l < O_MAX)
  {
    return '1';
  }
  else{
    return 'u';
  }
}

//state transition logic
void setState(enum State newstate)
{
  switch (newstate)
  {
  case Preamble:
    nzeroes = 1;
    break;
  case Content:
    thnbuf=0;
    break;
  default:
    break;		
  }
  state = newstate;
}

void handleContent(){//does something with thbuffer which should contain 40 '0'/'1' chars now.
  // after reset of the transmitter it transmits 5 times with 20s interval:
  // 1111 dddd tttt TTT0 hhhh HHHH S000 CCCC CCCC 1111
  // after that it transmits with 1 minute interval:
  // 1111 dddd tttt TTT0 hhhh HHHH S010 CCCC CCCC 1111
  // 0    4    8    12   16   20   24   28   32   36
  //Serial.println(&thbuffer[0]); //used only for debugging
  if (memcmp("1111",&thbuffer[0],4)!=0 || memcmp("1111",&thbuffer[36],4)!=0)//check beginning and end of string for  "1111"
    return;
  Td = dec(&thbuffer[4],4);  //temperature,  10^-1
  Tt = dec(&thbuffer[8],4);  //              10^0
  TT = dec(&thbuffer[12],3); //		     10^1
  hh = dec(&thbuffer[16],4); //humidity,     10^0
  hH = dec(&thbuffer[20],4); //		     10^1
  if (Td > 9 || Tt > 9 || TT > 9 || hh > 9 || hH > 9) //sanity check on values
    return;
  ContentHandled=1; // tell main loop content is handled
}

unsigned char dec(const char* s, unsigned char n)//returns the value of a 10101 string at s of length n
{
  unsigned char v=0;
  for(unsigned char i=0;i<n;i++)
    if (s[i]=='1')
      v+=1<<i;
  return v;
}

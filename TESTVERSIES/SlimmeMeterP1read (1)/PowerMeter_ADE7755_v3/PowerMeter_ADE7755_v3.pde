//#include "S0Sensor.h"
#include <Time.h>
//#include <arduino.h>

// START -- Powermeter section
// ============================

unsigned long PulseTimeNow = 0; // Time stamp (in millisecons) for pulse triggering the interrupt
unsigned long PulseTimeLast = 0; // Time stamp of the previous pulse
unsigned long PulseTimeInterval = 0; // Time interval since last pulse
unsigned long PulsesCumulatedTime = 0; // Time Interval since last power computation 
unsigned long PulsesNbr = 0;           // Number of pulses since last power computation 
unsigned long MinPulseTimeInterval = 5000000; //2* 1000000.0; // minimaal 10sec tot 20sec was 1000000000;
unsigned long PowerSamples;

unsigned long Frequency = 0;
unsigned long Power = 0;
float PowerNow = 0;
float PowerLast = 0;
boolean bPowerSign = 0;

// Misc.
// -----
int FirstLoop = 0;
int old_value=0;  
int Timer = 1;

// END -- PowerMeter section

int TRUE = 1;
int FALSE = 0;
int Samples = 1;
byte pin = 2;
bool sensorIsOn;

long todayCnt;                               // today's pulse count
int  ppu;                                    // the pulses per unit (kWh or m3) for this counter
long lastMillis;                             // the time of the last pulse (ms)
long pulseLength; 


volatile unsigned int ledPin = 13;
volatile int state = LOW;



// **********************
// -- SETUP
// **********************
void setup()
{
 
 Serial.begin(9600);
 Serial.println("\n\nArduGrid V1 - MercinatLabs (30 Nov 2011)");
 pinMode(pin, INPUT_PULLUP);
 
 // interrupt 1 on digital pin 3 for PowerMeter pulses
// attachInterrupt(0, PowerMeterPulse, RISING);
 PulseTimeLast = micros();
}

void loop()
{
 Serial.println(todayCnt++, DEC);
} 


void PowerMeterLoop()
{
    // read the digital input
    bool solarInput = digitalRead(pin);
    // rising edge?
    if(solarInput && sensorIsOn==false)
    {
        sensorIsOn=true;
    }
    // falling edge?
    if(!solarInput && sensorIsOn==true)
    {
        // store the time between the last two pulses
        pulseLength = millis() - lastMillis;
        //store the time for the next pulse
        lastMillis = millis();
        // update counters
        todayCnt++;
        sensorIsOn=false;
    }
}





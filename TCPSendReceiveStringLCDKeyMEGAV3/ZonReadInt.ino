//Voltcraft KWh meter op zonnepanelen via S0 ingang
// S0 geeft 2000 pulsen per KWh af.
// 1 puls is dus 1000W / 2000puls = 0.5 W per puls 

//Program
void ZonReadInt()
{
 ZonKWH = readKWH();
 ZonSUM = readKWH1();
 
 Serial.print("KWH: ");    
 Serial.print(ZonKWH, 4);
 Serial.print("    ");
 Serial.println(ZonSUM, 4);

 //delay(1000);
}


// The interrupt routine
void Kirq()
{
  rpk++;                    // just increment raw pulse counter.
  if (rpk > 1000000000)     // reset pulse counter after 10e9 pulse = 500.000 KW 
  {
    if (false == CS_KWH)    // in critical section?  // assumption IRQ-call is handled atomic on arduino.
    {
      rpk -= rpk_old;
      rpk_old = 0;
    }
  }
}

// returns KWH's since last reset
float readKWH1()
{
  //return rpk/2000.0;       // one pulse = 0.5 watt.
  return rpk/1000.0;
}

// returns KWH's since last call
float readKWH()
{
  CS_KWH = true;           // Start Critical Section - prevent interrupt Kirq() from changing rpk & rpk_old ;
      long t = rpk;            // store the raw pulse counter in a temp var.
      long k = t - rpk_old;    // subtract last measure to get delta
      rpk_old = t;             // remember old value
  CS_KWH = false;          // End Critical Section
  return k/2000.0;         // return delta, one pulse = 0.5 watt.
}


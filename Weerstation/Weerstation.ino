#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
RH_ASK driver;

#define _Whole_Data_length    116
#define _Array_dimension      2000
#define debug 0

int pin = 11;
byte tempten;
byte tempone;
byte tempcom;
byte humiten;
byte humione;
byte valuepos=0;
byte bitpos=0;
byte bitcounter=1;
byte array_pos=0;
byte value;
unsigned long duration;
int whole_array[_Whole_Data_length];   

void setup()
{
  #ifdef RH_HAVE_SERIAL
    Serial.begin(9600);    // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
  ;
#endif
}


// Werkt met FS1000 receiver van Thierry Mugler
// https://forum.arduino.cc/t/433-mhz-funkthermometer-decodieren/67111/22

void loop()
{
  duration = pulseIn(pin, HIGH);
  if ((duration > 600) && (duration < 1000)) {
    //if (debug) Serial.println("1");
    whole_array[array_pos] = 1;    
    array_pos++;
  }
  if ((duration > 1200)&&(duration < 4000)){
    //if (debug) Serial.println("0");
    whole_array[array_pos] = 0;   
    array_pos++;
  }
  
  if (array_pos>=_Whole_Data_length) {
        for (int valuepos = 22; valuepos <= 25; valuepos++) {
          bitWrite(tempten, bitpos, whole_array[valuepos]);
          bitpos++;
        }
        bitpos=0;
 
        for (int valuepos=26;valuepos<=29;valuepos++) {
          bitWrite(tempone, bitpos, whole_array[valuepos]);
          bitpos++;
        }
        bitpos=0;

        for (int valuepos=30;valuepos<=33;valuepos++) {
          bitWrite(tempcom, bitpos, whole_array[valuepos]);
          bitpos++;
        }
        bitpos=0;

        for (int valuepos=34;valuepos<=37;valuepos++) {
          bitWrite(humiten, bitpos, whole_array[valuepos]);
          bitpos++;
        }
        bitpos=0;        

        for (int valuepos=38;valuepos<=41;valuepos++) {
          bitWrite(humione, bitpos, whole_array[valuepos]);
          bitpos++;
        }
        bitpos=0;        

        Serial.print("Het is ");   
        Serial.print(tempcom,DEC);    
        Serial.print(tempone,DEC);
        Serial.print(",");
        Serial.print(tempten,DEC);
        Serial.print(" Graden Celsius bij ");
        Serial.print(humione,DEC);    
        Serial.print(humiten,DEC);
        Serial.println(" Procent Luchtvochtigheid.");        
        array_pos=0;
  }
}


/*
#define _Whole_Data_length    116
#define debug 0

byte pin = 2;
byte whole_array[_Whole_Data_length];   
byte value;
byte array_pos=0;
unsigned long duration;

void setup()
{
    pinMode(pin, INPUT);
    Serial.begin(9600);
    Serial.println("Give it to me...");
    delay(2000);
}

/*
void loop()
{
  duration = pulseIn(pin, HIGH);
  if ((duration>600)&&(duration<1000)) {
    if (debug) Serial.print("1");
    whole_array[array_pos] = 1;    
    array_pos++;
  }
  if ((duration>1200)&&(duration<4000)){
    if (debug) Serial.print("0");
    whole_array[array_pos] = 0;   
    array_pos++;
  }
  
  if (array_pos>=_Whole_Data_length) {
        if (debug) Serial.print(";");   
        Serial.print(createValue(30),DEC);    
        Serial.print(createValue(26),DEC);
        Serial.print(",");
        Serial.print(createValue(22),DEC);
        Serial.print(";");    
        Serial.print(createValue(38),DEC);    
        Serial.println(createValue(34),DEC);
        array_pos=0;
  }
}

byte createValue(byte start)
{
    byte bitpos=0;
    for (byte valuepos=start;valuepos<=start+3;valuepos++) {
          bitWrite(value, bitpos, whole_array[valuepos]);
          bitpos++;
    }
    return value;
}
*/

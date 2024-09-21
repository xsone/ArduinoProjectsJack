//#define dht_dpin = 0 //A0
byte bGlobalErr;
byte dht_dat[5];

int ledPin = 9;      // LED connected to digital pin 9
int analogPin = 3;   // potentiometer connected to analog pin 3; sensor hangt aan analog A0
int dht_dpin = 2;    // Digital input gebruiken
int input_val = 0;   // variable to store the read value
int output_val = 0;


void setup()

{
  pinMode(ledPin, OUTPUT);   // sets the pin as output

  Serial.begin(9600);
  InitDHT();
  //setTime(12,0,0,1,1,13); //0:00uur 1/1/2013
  //Serial.println("Humidity and temperature");
  

}



void loop()

{
  ReadDHT();
  switch (bGlobalErr)
  {
   case 0:
    
    Serial.print(dht_dat[0], DEC);
    Serial.print(".");
    Serial.print(dht_dat[1], DEC);
    Serial.print("%  ");
    
    Serial.print(dht_dat[2], DEC);
    Serial.print(".");
    Serial.print(dht_dat[3], DEC);
    Serial.println("C  ");
   break;
   case 1:
    Serial.println("Error 1: DHT start condition 1 not met.");
   break;
   case 2:
    Serial.println("Error 2: DHT start condition 2 not met.");
   break;
   case 3:
    Serial.println("Error 3: DHT checksum error.");
   break;
   default:
    Serial.println("Error: Unrecognized code encountered.");
   break;
 }
  
  
  
  
  
  
  input_val = analogRead(analogPin);   // read the input pin was val
  //input_val = dht_dat[0];
  output_val = input_val/4;
  
  
  
  
  
  analogWrite(ledPin, 255-output_val);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255

  Serial.print("input: ");
  Serial.println(input_val, DEC);
  
  Serial.print("output: ");
  Serial.println(255-output_val, DEC);
  
  delay(1000);
}



/*

void loop()
{
  ReadDHT();
  switch (bGlobalErr)
  {
   case 0:
    
    Serial.print(dht_dat[0], DEC);
    Serial.print(".");
    Serial.print(dht_dat[1], DEC);
    Serial.print("%  ");
    
    Serial.print(dht_dat[2], DEC);
    Serial.print(".");
    Serial.print(dht_dat[3], DEC);
    Serial.println("C  ");
   break;
   case 1:
    Serial.println("Error 1: DHT start condition 1 not met.");
   break;
   case 2:
    Serial.println("Error 2: DHT start condition 2 not met.");
   break;
   case 3:
    Serial.println("Error 3: DHT checksum error.");
   break;
   default:
    Serial.println("Error: Unrecognized code encountered.");
   break;
 }
 delay(1000); // elk kwartier
}
*/
 

void InitDHT()
{
 pinMode(dht_dpin,OUTPUT);
 digitalWrite(dht_dpin,HIGH);
}


 
void ReadDHT()
{
 bGlobalErr=0;
 byte dht_in;
 byte i;
 digitalWrite(dht_dpin,LOW);
 delay(20);
 digitalWrite(dht_dpin,HIGH);
 delayMicroseconds(40);
 pinMode(dht_dpin,INPUT);
 //delayMicroseconds(40);
 dht_in=digitalRead(dht_dpin);

 if(dht_in)
  {
   bGlobalErr=1;
   return;
  }
  delayMicroseconds(80);
  dht_in=digitalRead(dht_dpin);
 if(!dht_in)
  {
   bGlobalErr=2;
   return;
  }
  delayMicroseconds(80);
 for (i=0; i<5; i++) dht_dat[i] = read_dht_dat();
 pinMode(dht_dpin,OUTPUT);
 digitalWrite(dht_dpin,HIGH);
 
 byte dht_check_sum = dht_dat[0]+dht_dat[1]+dht_dat[2]+dht_dat[3];
  if(dht_dat[4]!= dht_check_sum) {bGlobalErr=3;}
};

 

 byte read_dht_dat()
 {
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++)
   {
    while(digitalRead(dht_dpin)==LOW);
    delayMicroseconds(30);
    if (digitalRead(dht_dpin)==HIGH)
    result |=(1<<(7-i));
    while (digitalRead(dht_dpin)==HIGH);
   }
  return result;
 }
 

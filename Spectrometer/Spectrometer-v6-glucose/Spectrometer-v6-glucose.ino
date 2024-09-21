//Versie 5.0 is is ter voorbereiding op de Android app

// variable declaration
int count = 0;
int val = 0;
int analogPin = 1; // sensor output connected to Analog Pin 1 hier komt een photo transistor aan te zitten
unsigned int sensorValue = 0; // variable to store the ADC value of the photodiode

char inChar = ' ';
String StofRead; //aantal stoffen
String StofSoort[5];

int nrSamples = 7;
int nrTest = 50;
int ir = 3;  // IR led on output pin 3 was pin 8
int red = 4; // red led on output pin 4 was pin 9
int orange = 5; // orange led on output pin 5 was pin 10
int yellow = 6; // yellow led on output pin 6 was pin 11
int blue = 7; // blue led on output pin 7 was pin 12

int pinArray[] = {3, 4, 5, 6, 7}; // pin address array
int col = 0; // for next loop coler
int row = 0;
unsigned int sampleValue[5]; // sample value array
unsigned int fileValue[5][5];

unsigned int Marge = 10; //was 5 hoeveel wijkt gemeten waarde af van opgeslagen waarde
int hitTeller = 0;
String Sample = "";

// initialize settings
void setup() 
{  
 Serial.begin(9600);//set baud rate
 Serial.print(0);  // To freeze the lower limit
 Serial.print(" ");
 Serial.print(300);  // To freeze the upper limit
 Serial.println(" ");

 pinMode(ir, OUTPUT); // set IR led pin as output
 pinMode(red, OUTPUT); // set red led pin as output
 pinMode(orange, OUTPUT); // set orange led pin as output
 pinMode(yellow, OUTPUT); // set yellow led pin as output
 pinMode(blue, OUTPUT); // set blue led pin as output

 // set pins voor output
 for (col=0 ;col < 5; col++) 
  {
   pinMode(pinArray[col], OUTPUT);
  }

  // Print startup text
 delay(100);
 //Serial.println("Spectrometer; (c) Jack; Rev. 5.0 okt Android app 2015");
 delay(200); // keep the welcome screen on for 3 seconds
}

// program starts here
void loop() 
{
 //Serial.println("..'l'..Lees");
 //Serial.println("..'s'..stability test");
 //Serial.println("..'c'..continue 50x");
 //Serial.println();
 //Serial.println("...Start...");
 //Serial.println();
 
 Continue();

 // wait button press
 /*
 for (;;)
  {  
   if (Serial.available() > 0)
    {
     inChar = Serial.read();
     if (inChar == 'l') Lees();
     if (inChar == 's') Stabiliteit();
     if (inChar == 'c') Continue();
    } 
  }
  */ 
}


//Stabiliteitstest
void Stabiliteit()
{
  Serial.println();
  Serial.println("Testen hoe stabiel meetwaarden blijven bij 5 metingen...");
  Serial.println();
     
  for (count=0; count < nrTest; count++)
   { 
   for (col=0; col < 5; col++) 
    { 
     digitalWrite(pinArray[col], HIGH);  //turn the led on
     delay(10); // wait for stabilization
     sensorValue = analogRead(analogPin);  // read the value from the sensor
     Sample = (String(sensorValue) + ',' + Sample);
     Serial.print(sensorValue); 
     Serial.print("-");  //send as a ascii encoded number
     if (col == 0) Serial.print("IR   ");
     if (col == 1) Serial.print("Red   ");
     if (col == 2) Serial.print("Orange    ");
     if (col == 3) Serial.print("Yellow  ");
     if (col == 4) Serial.print("Blue "); 
     digitalWrite(pinArray[col], LOW); // turn off ledr
    }
   Serial.println(count, DEC);
  }
}

//Continue meting
void Continue()
{
  for (count=0; count < nrTest; count++)
   { 
   for (col=0; col < 5; col++) 
    { 
     digitalWrite(pinArray[col], HIGH);  //turn the led on
     delay(10); // wait for stabilization
     //Sample = (String(sensorValue) + ',' + Sample);
     //Serial.print(",");
     //Serial.println(sensorValue);
//     Serial.print("-");  //send as a ascii encoded number
     //if (col == 0) Serial.println(sensorValue);
     //if (col == 1) Serial.println(sensorValue);
     //if (col == 2) Serial.println(sensorValue);
     //if (col == 3) Serial.println(sensorValue);
     //if (col == 4) Serial.println(sensorValue); 
     sensorValue = analogRead(analogPin);  // read the value from the sensor
     if (sensorValue < 350) Serial.println(sensorValue);
     digitalWrite(pinArray[col], LOW); // turn off ledr
    }
//   Serial.println(count, DEC);
  }
}

//Opslaan op SD card
void Lees()
{
  Sample = ""; //buffer leeg
  StofRead = "";
  
  //Serial.println();
  //Serial.println("...Uitlezen Spectro sensor...");
  for (col=0; col < 5; col++) 
   { 
    digitalWrite(pinArray[col], HIGH);  //turn the led on
    delay(100); // wait for stabilization
    sensorValue = analogRead(analogPin);  // read the value from the sensor
    Sample = (Sample + ',' + String(sensorValue));
    //Serial.print(sensorValue); 
    //Serial.print("-");  //send as a ascii encoded number
    //if (col == 0) Serial.print("IR   ");
    //if (col == 1) Serial.print("Red   ");
    //if (col == 2) Serial.print("Orange    ");
    //if (col == 3) Serial.print("Yellow  ");
    //if (col == 4) Serial.print("Blue "); 
    digitalWrite(pinArray[col], LOW); // turn off ledr
   }
  delay(500);
  Serial.println();
  Serial.flush();
  Serial.print(Sample);
/*
  Serial.println();
  Serial.println("Voer Sample-naam in en sluit af met Enter...");
  Serial.flush();
  for(;;)
   {
    if(Serial.available() > 0)
     {
      StofRead = Serial.readStringUntil('\n'); 
      break;
     } 
   } 
  delay(100);
  Serial.println();
  //Serial.println(Sample);
  Serial.println(StofRead + ':' + Sample);
*/  
  Sample = ""; //buffer leeg
  StofRead = "";
}


// Spectrometer_led10
// clear the LCD
//void clearLCD() {
//  Serial.print(12, BYTE);
//  delay(1);
//}
// start a new line
//void newLine() {
//  Serial.print(10, BYTE);
//}  

//Versie 4.0 is inclusie saving op microSD shield Sparkfun Chip Select is daar pin nr 8, vandaar omgezet naar

// '/n' moet worden 13
// '/r' moet worden 10

#include <SD.h>

File myFile;
char FileNaam[] = "spectro.txt"; // moet in te lezen naam wordem

// variable declaration
int count = 0;
int val = 0;
int analogPin = 1; // sensor output connected to Analog Pin 1 hier komt een photo transistor aan te zitten
unsigned int sensorValue = 0; // variable to store the ADC value of the photodiode

char inChar = ' ';
String StofRead; //aantal stoffen
String StofSoort[5];

int nrSamples = 7;
int nrTest = 10;
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

// standaard waarden diverse stoffen
int niks[] = {965,987,937,990,992}; 
int niksMarge = 5;

int plastic[] = {948,988,938,992,992}; 
int plasticMarge = 5;

int papier[] = {1003,1018,1020,1021,1021}; 
int papierMarge = 5;

unsigned int Marge = 10; //was 5 hoeveel wijkt gemeten waarde af van opgeslagen waarde
int hitTeller = 0;
String Sample = "";
String oldSensorValue = "START: ";


// initialize settings
void setup() 
{  
 Serial.begin(9600);//set baud rate

 pinMode(ir, OUTPUT); // set IR led pin as output
 pinMode(red, OUTPUT); // set red led pin as output
 pinMode(orange, OUTPUT); // set orange led pin as output
 pinMode(yellow, OUTPUT); // set yellow led pin as output
 pinMode(blue, OUTPUT); // set blue led pin as output

 Serial.print("Initializing SD card...");
 //pinMode(10, OUTPUT);
 if (!SD.begin(8)) {
  Serial.println("initialization SD card failed!");
  return;
 }
 Serial.println("initialization SD card done.");
 // set pins voor output
 for (col=0 ;col < 5; col++) 
  {
   pinMode(pinArray[col], OUTPUT);
  }

  // Print startup text
 delay(100);
 Serial.println("Spectrometer; (c) Jack; Rev. 1.20 june 2015");
 delay(200); // keep the welcome screen on for 3 seconds
}

// program starts here
void loop() 
{
 Serial.println("...Leer Mode (pin 2).");
 Serial.println("...Herken Mode (pin 3)...");
 delay(50);
 Serial.println("..'r'..Lees");
 Serial.println("..'h'..Herken");
 Serial.println("..'s'..Opslaan");
 Serial.println("..'t'..stability test");
 Serial.println();
 Serial.println("...Start...");
 Serial.println();

 // wait button press
 for (;;)
  {  
   if (Serial.available() > 0)
    {
     inChar = Serial.read();
     if (inChar == 'r') Lees();
     if (inChar == 'h') Herken();
     if (inChar == 's') Opslaan();
     //if (inChar == 't') Stabiliteit();
    } 
  } 
}


//Stabiliteitstest
void Stabiliteit()
{
  Serial.println();
  Serial.println("Testen hoe stabiel meetwaarden blijven...");
  Serial.println();
  Serial.println("Voer het aantal testen in en sluit af met Enter...");

  Serial.flush();
  for(;;)
   {
    if(Serial.available() > 0)
     {
      String temp = Serial.readStringUntil('\n'); 
      nrTest = temp.toInt();
      break;
     } 
   } 
  
   
  for (count=0; count < nrTest; count++)
   { 
   for (col=0; col < 5; col++) 
    { 
     digitalWrite(pinArray[col], HIGH);  //turn the led on
     delay(100); // wait for stabilization
     sensorValue = analogRead(analogPin);  // read the value from the sensor
     Sample = (Sample + ',' + String(sensorValue) );
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


//Teruglezen uit SD card  
void Lees()
{
 //row = 0;
 Serial.println("...Teruglezen uit File SD-card...");
 Serial.println();
 
 myFile = SD.open(FileNaam, FILE_READ);
 if (myFile) 
  {
   Serial.println(FileNaam);
   //while ( myFile.available() ) //orgineel
   // { 
     for (row=0; row < nrSamples; row++)
      {
       StofSoort[row] = myFile.readStringUntil(':'); //werkt niet
       Serial.println(StofSoort[row]); // is incl EOL
       for (col=0; col < 5 ;col++) 
        { 
         fileValue[row][col] = myFile.parseInt();  // read the value from the sensor 
         Serial.print(fileValue[row][col], DEC); 
         Serial.print(' ');
        } // end for
       //Serial.println();
      } // end for
  // } // end while
  } // end if
 else Serial.println("error opening file.txt");
 myFile.close();
 Serial.println();
 Serial.println("EOF...break");
}

// Herken functie
void Herken()
{
 Lees();
 Serial.println();
 Serial.println();
 Serial.println();
 Serial.println("...Herken Mode...");
 for (row=0; row < nrSamples; row++)
  {
   Serial.println();
   Serial.print("Stofnummer: ");
   Serial.println(row,DEC);
   hitTeller = 0;
   for (col=0; col < 5; col++) 
    { 
     digitalWrite(pinArray[col], HIGH); //turn on led
     delay(100); // wait for led to stabilize
     sensorValue = analogRead(analogPin);  // read the value from the sensor
     sampleValue[col] = sensorValue; // store value in matrix
     digitalWrite(pinArray[col], LOW); // turn off led
     //if ( (sampleValue[col] >= (fileValue[row][col] - Marge)) && (sampleValue[col] <= (fileValue[row][col] + Marge)) )
     if ( (sampleValue[col] - fileValue[row][col]) < Marge )
      {
       hitTeller++;
       //Serial.print("Verschil: ");
       //Serial.println( (sampleValue[col] - fileValue[row][col]), DEC);
       Serial.print ("hitTeller: ");
       Serial.println(hitTeller);
      } //endif 
     Serial.print("Meet <> File : ");
     Serial.print(sampleValue[col], DEC);
     Serial.print("...");
     Serial.println(fileValue[row][col], DEC);
     if(hitTeller == 4)
      {
       Serial.print("Stof herkend: ");
       Serial.println(StofSoort[row]);
       //  Serial.print ("hitTeller: ");
     //  Serial.println(hitTeller);
       /*
       for (col=0; col < 5; col++)
        {
         Serial.print("Meet <> File..Hit!! : ");
         Serial.print(sampleValue[col], DEC);
         Serial.print("...");
         Serial.println(fileValue[row][col], DEC); 
        }
       */ 
       break; //exit for loop
      } //endif hitTeller 
     } //endfor 
    } //endfor
 Serial.println();
 Serial.println("...Einde Herken...");
 Serial.println();
}

//Opslaan op SD card
void Opslaan()
{
  Serial.println();
  Serial.println("Definitief Opslaan referentie?...");
  for (col=0; col < 5; col++) 
   { 
    digitalWrite(pinArray[col], HIGH);  //turn the led on
    delay(100); // wait for stabilization
    sensorValue = analogRead(analogPin);  // read the value from the sensor
    Sample = (Sample + ',' + String(sensorValue) );
    Serial.print(sensorValue); 
    Serial.print("-");  //send as a ascii encoded number
    if (col == 0) Serial.print("IR   ");
    if (col == 1) Serial.print("Red   ");
    if (col == 2) Serial.print("Orange    ");
    if (col == 3) Serial.print("Yellow  ");
    if (col == 4) Serial.print("Blue "); 
    digitalWrite(pinArray[col], LOW); // turn off ledr
   }
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
  Serial.println(Sample);
  Serial.println(StofRead + ':' + Sample);
  //SD.remove(FileNaam);
  myFile = SD.open(FileNaam, FILE_WRITE); // FILE_ATACH
  
  if (myFile) 
   {
    Serial.println(FileNaam);
   
    //while ( myFile.available() ) //orgineel
    // {
    //  String fileInhoud = myFile.readStringUntil('#'); //werkt niet
    //  Serial.println(fileInhoud);
    //  fileInhoud.replace("#", "$");
    //  Serial.println(fileInhoud);
    // } 
   
   myFile.println(StofRead + ':' + Sample);
   myFile.close();
  }  
  else Serial.println("error opening file.txt"); 
  Serial.println();
  Serial.println("EOF...break");
  Sample = ""; //buffer leeg
  StofRead = "";
}

#include <SD.h>

File myFile;

char inChar;
String Stof;
int StofNr;
String StofRead;
//char FileName[10] = "";
char FileNaam[] = "spectro.txt"; // moet in te lezen naam wordem

int count = 0; // for next loop counter
int row = 0;
int col = 0;
int sampleValue[5]; // sample value array
int fileValue[5][5];
int lineValue[5];


void setup() 
{  
 Serial.begin(9600);//set baud rate
 Serial.print("Initializing SD card...");
 if (!SD.begin(8)) {
    Serial.println("initialization SD card failed!");
    return;
  }
 Serial.println("initialization SD card done.");
 Serial.println("FileReadTest; (c) Jack; Rev. 1.20 sept 2015");
}

void loop() 
{
  Serial.println("...Lees Mode r");
  delay(50);
  Serial.println("...Start...");
  Serial.println();

  // wait button press
  for (;;)
  {  
    /*
   LeerState = digitalRead(inPinLeer); // test for red button press
     HerkenState = digitalRead(inPinHerken); // test for black button press  
     
     if (HerkenState == LOW) Herken(); // red button pressed
     else digitalWrite(inPinHerken, HIGH); 
     if (LeerState == LOW) Leer();  // black button pressed goto learn; // jump to learn subroutine    
     else digitalWrite(inPinLeer, HIGH); 
     */


    if (Serial.available() > 0)
    {
      inChar = Serial.read();
      //if (inChar == 'l') Leer();
      //if (inChar == 'h') Herken();
      //if (inChar == 's') Opslaan();
      if (inChar == 'r') Lees();
    } 
  } 
}


void Lees()
{
  Serial.println("...Teruglezen uit File SD-card...");
  int pos = 0;
  int regel = 0;
  
  myFile = SD.open(FileNaam, FILE_READ);
  if (myFile) 
   {
    Serial.println(FileNaam);
    
   while (myFile.available())
    { 
      //Serial.println(myFile.read());

        //for (row=0; row < 5; row++)
        
        // {
          
          inChar = myFile.read();
          
          if(inChar == '=')
           {
            StofRead = myFile.readStringUntil(':');
            Serial.println(StofRead);
            StofRead = "";
           } 
          
          if(inChar == ';') //werkt niet
           {
            //for (col=0; col < 5 ;col++) 
            // { 
              //fileValue[row][col] = myFile.parseInt();  // read the value from the sensor 
              //fileValue[row][col] = col;
              //Serial.print(myFile.parseInt(), DEC); 
              Serial.println("; found");
              //if(myFile.read() == ';') {Serial.println("EOL"); break;}
            // } 
           }
          //myFile.readln();
          //Serial.println("next line");
          Serial.print(inChar);
        // }
        
        //Serial.println("EOF...break");
        //break;
        //Serial.write(myFile.read());
        //StofRead = myFile.readString();
        //Serial.println(StofRead);
        //StofRead = "";
        //break;
        
      // } //end while file
       //Serial.println();
       //Serial.println("EOL");
       //if(inChar == ';') break;
       //return;
      //} 
     //Serial.println();
    // Serial.println("EOF");
     //return;
    } // end while
   } // end if
  else Serial.println("error opening file.txt");
  myFile.close();
}

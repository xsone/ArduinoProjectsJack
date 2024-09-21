
// Spectrometer_led10
// clear the LCD
void clearLCD() {
  Serial.print(12, BYTE);
  delay(1);
}
// start a new line
void newLine() {
  Serial.print(10, BYTE);
}  
// variable declaration
int inPinRed = 3; // red button on pin 3
int inPinBlack = 2; // black button on pin 2
int redState; // is red button pressed
int blackState; // is black button pressed
int val = 0;
int analogPin = 1; // sensor output connected to Analog Pin 1
int sensorValue = 0; // variable to store the ADC value of the photodiode
int red = 6; // red led on output pin 6
int yellow = 7; // yellow led on output pin 7
int ir = 8;  // IR led on output pin 8
int blue = 9; // blue led on output pin 9
int orange = 10; // orange led on output pin 10
int pinArray[] = {6, 7, 8, 9, 10}; // pin address array
int count = 0; // for next loop counter
int sampleValue[5]; // sample value array
int windex[] = {38,1019,564,321,315}; // database values for windex
int windexValue; // windex value 
int windexTemp = 0; // windex value - sensor value for ID
int motoroil[] = {31,1020,787,283,193};  
int motoroilValue;
int motoroilTemp = 0;
int coffee[] = {38,993,41,351,286}; 
int coffeeValue;
int coffeeTemp = 0;
int softsoap[] = {31,1020,708,379,58};
int softsoapValue;
int softsoapTemp = 0;
int water[] = {33,1017,582,327,45};
int waterValue;
int waterTemp = 0;
int mouthwash[] = {46,1020,621,292,568};  
int mouthwashValue;
int mouthwashTemp = 0;
int tea[] = {33,1010,335,317,48};  
int teaValue;
int teaTemp = 0;
int rainx[] = {32,1015,402,305,49};  
int rainxValue;
int rainxTemp = 0;
int nosample[] = {46,488,30,44,325};  
int nosampleValue;
int nosampleTemp = 0;
int none[] = {46,488,30,44,325}; // no sample
int noneValue;
int noneTemp = 0;
void setup() // initialize settings
{  
  beginSerial(19200);  //set serial port to 19.2k baud
  pinMode(inPinRed, INPUT); // set red switch pin as input
  pinMode(inPinBlack, INPUT); // set black switch pin as input
  pinMode(red, OUTPUT); // set red led pin as output
  pinMode(yellow, OUTPUT); // set yellow led pin as output
  pinMode(ir, OUTPUT); // set IR led pin as output
  pinMode(blue, OUTPUT); // set blue led pin as output
  pinMode(orange, OUTPUT); // set orange led pin as output
// actually set the pins for output
  for (count=0;count<4;count++) {
    pinMode(pinArray[count], OUTPUT);
  }
// Print startup text
delay(100);
// nex long line is for initializing the display when necessary
//Serial.print(26,BYTE), Serial.print(26,BYTE), Serial.print(32,BYTE),Serial.print
(32,BYTE),Serial.print(32,BYTE),Serial.print(32,BYTE),Serial.print
(32,BYTE),Serial.print(32,BYTE),Serial.print(26,BYTE),Serial.print(26,BYTE); //
reboot display


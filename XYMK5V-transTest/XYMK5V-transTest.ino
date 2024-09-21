
 #define rfReceivePin 0  //RF Receiver pin = Analog pin 0
 #define ledPin 13        //Onboard LED = digital pin 13

 unsigned int data = 0;   // variable used to store received data
 //const unsigned int upperThreshold = 70;  //upper threshold value
 //const unsigned int lowerThreshold = 60;  //lower threshold value
 const unsigned int upperThreshold = 70;  //upper threshold value
 const unsigned int lowerThreshold = 700;  //lower threshold value

 void setup(){
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600);
 }

 void loop(){
   data=analogRead(rfReceivePin);    //listen for data on Analog pin 0
   delay(200);
   if (data < 700 && data > 0) Serial.println(data);

/*
    if(data>upperThreshold){
     digitalWrite(ledPin, LOW);   //If a LOW signal is received, turn LED OFF
     Serial.println(data);
   }
   
   if(data<lowerThreshold){
     digitalWrite(ledPin, HIGH);   //If a HIGH signal is received, turn LED ON
     Serial.println(data);
   }
 */  
   
 }

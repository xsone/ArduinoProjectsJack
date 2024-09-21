//Momentary Button Module o.b.v. KY004
//Use the LED on digital pin 13, and connect the button module input to digital 3, and when button is pressed, the LED will turn on, otherwise, it will turn off.


int Led=13;

int buttonpin=2;

int val;

void setup()

{

pinMode(Led,OUTPUT);

pinMode(buttonpin,INPUT);

}

void loop()

{

val=digitalRead(buttonpin);

if(val==HIGH)

{

digitalWrite(Led,HIGH);

}

else

{

digitalWrite(Led,LOW);

}

}

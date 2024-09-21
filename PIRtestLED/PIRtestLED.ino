//Connect PIR sensor to pin 9
//Connect what ever you want to go high to pin 13
int val = 0;//sets value variable to 0
void setup() {
pinMode (13,OUTPUT);//pin 13 is the led pin
pinMode (3, INPUT);//pin 9 reads the high signal from the PIR
}
void loop () {
val = digitalRead(3);//Pin 9 is initiallly 0
digitalWrite(13,val);//pin thirteen is equal to whatever pin 9 is
if (val) digitalWrite(13,LOW);//led turns off
}

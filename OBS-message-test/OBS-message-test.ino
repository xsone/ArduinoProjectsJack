byte message[] = {0x41, 0x1C, 0x33, 0x14};
byte temp;
int i = 0;
int tel = 0;
unsigned int Speed;

void setup(){
  Serial.begin(9600);
  Serial.println("OBD-scanner v1.0 start..");
}

void loop(){

/*  
  message[2] = 0x25;
  message[3] = 0x12;
  for( i = 0; i < sizeof(message); i++ )
   {
    Serial.print(message[i],HEX);
    Serial.print(" ");
   }
  Serial.println(); 
  delay(1000);
  message[2] = 0x33;
  message[3] = 0x14;
  for( i = 0; i < sizeof(message); i++ )
   {
    Serial.print(message[i],HEX);
    Serial.print(" ");
   }
 */
  Serial.println("41 0C 0F A0"); //010C RPM 1000 RPM 0X0FA0 = 4000 RPM
  delay(1000);
  Serial.println("41 0D FF"); //010D 0xFF = 255 KMH
  delay(1000);
  Serial.println("41 05 64"); //0105 0x64 = 100; 100-40 = 60 C
  delay(1000);
  Serial.println("DP auto,iso15765-4(can11/500)>"); 
  delay(1000);
  Serial.println("41 0C 07 D0"); //010C RPM 1000 RPM 0X07D0 = 2000 RPM
  delay(1000);
  Serial.println("41 0D 96"); //010D 0x96 = 150 KMH
  delay(1000);
  Serial.println("41 05 5A"); //0105 0x5A = 100; 90-40 = 50 C
  delay(1000);
}

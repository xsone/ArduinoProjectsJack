
void setup()  
{
  Serial.begin(38400);
  Serial1.begin(38400);
  Serial.print("start...");
}

void loop()                     // run over and over again
{

  if (Serial1.available()) {
      Serial.print((char)Serial1.read());
  }
  //if (Serial.available()) {
  //    Serial1.print((char)Serial.read());
 // }
}

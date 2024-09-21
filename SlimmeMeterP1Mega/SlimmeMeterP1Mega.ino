char c;
 
void setup() {
    Serial.begin(115200);
    Serial.println("Serial oke");
    Serial1.begin(115200);
    Serial.println("Serial1 oke");    
}
 
void loop() {
    if (Serial1.available()) {
        c = Serial1.read();

   
        // --- 7 bits instelling ---
        c &= ~(1 << 7);
        char inChar = (char)c;
        if (inChar == '\n') { 
         Serial.println("REGEL: ");
        }
    Serial.print(c);
    }
}

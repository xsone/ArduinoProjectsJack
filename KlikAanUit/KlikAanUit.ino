//Test set up for Arduino met Rx 433 ontvanger tbv KaKu
int  ir_pin = 3;                //Rx ingang pin 3 
int led_pin = 13;               //"Klaar om te ontvangen" flag, not needed but nice
unsigned int a ;
int v= 0;
int b= 0;
int c= 0;
unsigned int time1 =0;
unsigned int time =0;
unsigned long code = 0;
unsigned int  codea = 0;
char lettercode; 
unsigned int mystatus[16];
unsigned long data[110];

int samples[16] ={
  0,0x100,0x400,0x500,0x1000,0x1100,0x1400,0x1500,0x4000,0x4100,0x4400,0x4500,0x5000,0x5100,0x5400,0x5500};
char mijnstring[6] ={
  0,0x20,0x0,0x20,0x3F,0xA,}; //Chr 1 Letter, chr 3 cijfer, char 5 status A Aan U uit

void setup() {
  pinMode(led_pin, OUTPUT);     //Klaar om te ontvangen
  pinMode(ir_pin, INPUT);
  digitalWrite(led_pin, LOW);       //bog niet klaar
  Serial.begin(9600);
}

void loop() {
  int key = getIRKey();         //Haal de code
  delay(80);  
}

int getIRKey() {
  int z;
  digitalWrite(led_pin, HIGH);     //Ok, ik ben klaar voor ontvangst
 do{ 
 a = pulseIn(ir_pin, LOW);
 } 
while(a >10000);  //Wait for repeat
 b = getdata(); 
  if (b ==0){ //geen errors
  digitalWrite(led_pin, LOW);
  decode(16); //decodeer 16 bits
  codea = codea <<3; // bit shift 3 naar links
  if  (codea == 0xA000){ //kijk hier naar de bits die altijd aanstaan
    decode(0);  //decode 8 bits vanaf bit 0
    convert(codea); //converteer naar letter
    mijnstring[0] =lettercode; //eerste command
    decode(8); //decode 8 bits vanaf bit 
    convert(codea);
    mijnstring[2]=lettercode; //2e command
    mijnprint(); 
    decode2(22); //command aan-uit
  }
  else {
    
    // ik heb signalen gekregen maar niet voor mij bestemd zijn
    // worden deze herhaald??
    time = millis() - time1;
    if (time > 35000){
      v++;
      Serial.print("niet gevonden ");
      Serial.println(v);
      Serial.print("tussenpauze  ");
      Serial.println(time,DEC);
    }
    time1 =millis();
  } 
  }
  else{
    // bit errors
 //    Serial.print("error  ");
 //     Serial.print(b,HEX);
  }
    
}

int getdata(){
  int error=0;
  for (int i =0 ; i <25; i++)
  {
    data[i] = pulseIn(ir_pin,HIGH);
    if (data[i] < 320 ){ // pulse is te kort
      error = i;
      break;
    }
  }
  return error;
}
void decode(int stcode) {
  // converteer 8 samples naar letter
  codea = 0;
  for(int i=stcode ;i < stcode + 8;i++) {
    codea = codea >> 1;     
    if (data[i] < 1000)
    {      
      // nothing at the moment
    }
    else {
      codea = codea + 0x8000;
    }
  }
}

void decode2(int stcode) {
  mijnstring[4]=0x3F;
  codea = 0;
  for(int i=stcode ;i < stcode+1;i++) {
    codea = codea >> 1;     
    if (data[i] < 1000)
    {      
      mijnstring[4]=0x55;
      status(20);
    }
    else {
      codea = codea + 0x8000;
      mijnstring[4]=0x41;
      status(21);
    }
  }
}

char convert(int mijnnummer){
  for (int i =0; i<16; i++){
    if (samples[i]== mijnnummer){
      lettercode = i +0x41;
    }
  }
}
void mijnprint(){
  for (int i =0; i<6; i++){
    Serial.print(mijnstring[i]);
  }
}

void status(int x){
  int z;
  c=01; 
  b =(mijnstring[2]-0x41);
  z =(mijnstring[0]-0x41);
  c =  c << b;
  if(x ==21){
    mystatus[z] = mystatus[z]| c;   
  }
  else {
    c = ~c;
    mystatus[z] = mystatus[z] & c;
  }
  Serial.println(mystatus[z],BIN);
}

void printstatus(){
  for (int i =0; i <16; i++)
    Serial.println(mystatus[i],BIN);
}

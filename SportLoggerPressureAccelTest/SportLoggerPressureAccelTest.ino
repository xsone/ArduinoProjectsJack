//Test voor GIT

/*  Meet de versnelling in XYZ postie met ADXL accelerometer; tevens de kracht m.b.v. druksensor
De sensors zijn aangesloten op Arduino Nano met Bluetooth interface*/

#define DELAY 250 //delay in ms
#define HEADER "##"

const int blinkPin = 13;          // The on-board Arduino LED, close to PIN 13.
const int ForcePinLeft = A5;
const int ForcePinRight = A6;
const int VCCPin = A4;
const int xPin = A3;                  // x-axis of the accelerometer
const int yPin = A2;                  // y-axis
const int zPin = A1;                  // z-axis (only on 3-axis models)
const int GNDPin = A0;

int xData = 0;
int yData = 0;
int zData = 0;

int xAng = 0;
int yAng = 0;
int zAng = 0;

int ForceValueLeft = 0;
int ForceValueRight = 0;

int minVal = 300;
int maxVal = 360;

int tominVal = -90;
int tomaxVal = 90;


void setup() {   
  Serial.begin(9600);          // For Serial Monitor
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(ForcePinLeft, INPUT);   
  pinMode(ForcePinRight, INPUT);  
  analogReference(DEFAULT);  
  pinMode(A0, OUTPUT);
  pinMode(A4, OUTPUT);
  digitalWrite(A4, HIGH);
  digitalWrite(A0, LOW);
}

void loop() {
  //analogReference(EXTERNAL);
  int xData = analogRead(xPin);
  yData = analogRead(yPin);
  zData = analogRead(zPin);
  int accData= sqrt((xData*xData)+(yData*yData)+(zData*zData));

  //bereken de hoek in graden bereik van 0 tot 360 was 180; map(value, fromLow, fromHigh, toLow, toHigh)
  xAng = map(xData, minVal, maxVal, tominVal, tomaxVal);
  //if(xAng < -90 || xAng > 90) xAng = 0;
  yAng = map(yData, minVal, maxVal, tominVal, tomaxVal);
  //if(yAng < -90 || yAng > 90) yAng = 0;
  zAng = map(zData, minVal, maxVal, tominVal, tomaxVal);
  //if(zAng < -90 || zAng > 90) zAng = 0;
 
  ForceValueLeft = analogRead(ForcePinLeft);
  ForceValueRight = analogRead(ForcePinRight);
  //delay(DELAY);
  Serial.println(String(HEADER)+','+String(ForceValueLeft)+','+String(ForceValueRight)+','+ String(xAng)+','+String(yAng)+','+String(zAng)+','+String(accData)+',');
//  Serial.print("Z-as: ");
//  Serial.print(String(zAng));
//  Serial.print(",");
//  Serial.print("Y-as: ");
//  Serial.println(String(yAng));
  delay(DELAY);                    // considered best practice in a simple sketch.
}

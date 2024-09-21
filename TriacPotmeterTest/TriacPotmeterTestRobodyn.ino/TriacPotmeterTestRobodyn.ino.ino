#include <TriacDimmer.h>

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

unsigned char sync = 8; //sync pin
unsigned char channel_1 = 9; // channel 1 pin

//TriacDimmer::setBrightness(channel_1, 0.5);

void setup() {
  // initialize the dimmer library.
  TriacDimmer::begin();
  Serial.begin(9600);
}

void loop() {
  // read the input on analog pin A0:
  int analogValue = analogRead(A0);
  //Serial.println(analogValue);
  float brightness = floatMap(analogValue, 0, 3000, 0, 1);
  Serial.println(brightness);
  // set channel 1 to the brightness value:
  TriacDimmer::setBrightness(channel_1, brightness); 
}

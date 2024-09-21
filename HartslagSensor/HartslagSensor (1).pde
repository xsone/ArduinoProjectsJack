//Includes

// Pins
const int ledPin = 13;
const int hartPin = 0;
const int tempPin = 1;
const int verschilPin = 2;

// Hardslag LED
int ledState = LOW;
long ledOnMillis = 0;
long ledOnInterval = 50;

// Hardslag detectie variabelen
int newHeartReading = 0;
int lastHeartReading = 0;
int verschil = 0;
int metingen[8] = {0,0,0,0,0,0,0,0};
int historySize = 8;
int recentTotal = 0;
int index = 0;
boolean highChange = false;
int minimaalVerschil = 5;

// Hardslag timing
long lastHeartbeatTime = 0;
long debounceDelay = 150;
int currentHeartrate = 0;
int lastHeartrate = 0;

// Debugging string
String debugOutput = "";

void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); 
}

void loop() 
{
  // LED UIT
  digitalWrite(ledPin, LOW);
  // Lees de sensorgegevens uit
  newHeartReading = analogRead(hartPin);
  minimaalVerschil = analogRead(verschilPin);
  minimaalVerschil = 2;//(minimaalVerschil-403)/100;
  
  //Bereken verschil tussen nieuwe en laatste meting
  verschil = newHeartReading - lastHeartReading;
  lastHeartReading = newHeartReading;
  
  // Find new recent total
  recentTotal = recentTotal - metingen[index] + verschil;
  // replace indexed recent value
  metingen[index] = verschil;
  // index + 1
  index = (index + 1) % historySize;
  
  if (recentTotal >= minimaalVerschil) 
  {
    // Mogelijke hartslag bekijk de timing
    if (millis() - lastHeartbeatTime >= debounceDelay)
    {
		// Hartslag
		digitalWrite(ledPin, HIGH);
		currentHeartrate = 60000 / (millis() - lastHeartbeatTime);
    		lastHeartbeatTime = millis();
    		if (currentHeartrate <= 200)// && currentHeartrate > 20) 
    		{  
		   LCD_CMD(0x80);
                   Serial.print("Heartrate=>" + String(currentHeartrate) + "  ");
    		} 
                   LCD_CMD(0xC0);
                   Serial.print("Hand Temp=>" + String(recentTotal));
    
    }
  }
  delay(10); 
}
void LCD_CMD(int Command)
{
  Serial.print(0xFE, BYTE);
  Serial.print(Command, BYTE); 
}

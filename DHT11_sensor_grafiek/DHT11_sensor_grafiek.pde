import processing.serial.*;
 
Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph 
 
//Variables to draw a continuous line.
int lastxPos=1;
int lastheight=0;
float val;
 
void setup () {
  // set the window size:
  size(800, 600);        
 
  // List all the available serial ports
  println(Serial.list());
  // Check the listed serial ports in your machine
  // and use the correct index number in Serial.list()[].
 
  myPort = new Serial(this, Serial.list()[3], 9600);  //
 
  // A serialEvent() is generated when a newline character is received :
  //myPort.bufferUntil('\n');
  background(0);      // set inital background:
}
void draw () {
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.read();          // read it and store it in val
    // Convert the values to set the radius
 
  }
  // everything happens in the serialEvent()
}
 
void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');
  if (inString != null) {
    inString = trim(inString);                // trim off whitespaces.
    float inByte = float(inString);           // convert to a number.
    inByte = map(inByte, 0, 100, 0, height); //map to the screen height.
 
    //Drawing a line from Last inByte to the new one.
    stroke(127,34,255);     //stroke color
    strokeWeight(2);        //stroke wider
    line(lastxPos, lastheight, xPos, height - inByte); 
    lastxPos= xPos;
    lastheight= int(height-inByte);
 
    // at the edge of the window, go back to the beginning:
    if (xPos >= width) {
      xPos = 0;
      lastxPos= 0;
      background(0);  //Clear the screen.
    } 
    else {
      // increment the horizontal position:
      xPos++;
    }
  }
}

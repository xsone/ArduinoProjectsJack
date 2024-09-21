/* 
This program is free software; you can redistribute it and/or modify 
it under the terms of the GNU License V2.                            
This program is distributed in the hope that it will be useful,      
but WITHOUT ANY WARRANTY; without even the implied warranty of       
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        
GNU General Public License, version 2 for more details               

File:Seismic_processing
By SensorFace <http://www.sensorface.nl>
Created 9 Sept 2014

Reads JSON from the Arduino feed and displays the data in a graph
*/

import processing.serial.*;
Serial myPort;        // The serial port
float xPos = 0;       // horizontal position of the graph
String val;

float xPrev = 0;
float yPrev = 0;
float zPrev = 0; 
 
void setup () {
  //size(1920, 900);        // window size
  size(1024, 800);        // window size
 
  // List all the available serial ports
  //println(Serial.list());
 
  String portName = Serial.list()[0];
  //myPort = new Serial(this, portName, 9600);
  myPort = new Serial(this, "COM9", 9600);
 
  background(#000000); 
  
  myPort.clear();
  myPort.write('A');
}


void draw () {
   
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.readStringUntil(10);         // read it till LF and store it in val
    //print(val[0]);
    if(val != null) val = trim(val);
  }
  
  if(val != null && val.length() > 0){
    if (!val.substring(0,1).equals("{")){
      //print(val.substring(0,1));
      //print(val.substring(0,2));
      //print(val.substring(0,3));
      val = "";
      myPort.clear();
      return;
    }
    
    int xSpeed = 2;
    JSONObject json = new JSONObject().parse(val);

    println(val);
    //float x = json.getFloat("Humi")+height/2;
    //float y = json.getFloat("Temp")+height/2;
    //float z = json.getFloat("Sens")+height/2;
   
    float x = json.getFloat("x")+height/2;
    float y = json.getFloat("y")+height/2;
    float z = json.getFloat("z")+height/2;
    
    //println(x);
    //println(y);
    //println(z);
    
    // println(z);
    strokeWeight(1);
    stroke(#FFFFFF); 
    line(xPos+1, 0, xPos+1, height);
    stroke(#000000); 
    strokeWeight(xSpeed+1);
    line(xPos-2, 0, xPos-2, height);
    strokeWeight(4);
    stroke(#FF0000);
    line(xPos-xSpeed, xPrev, xPos, height - x);
    stroke(#00FF00);
    line(xPos-xSpeed, yPrev, xPos, height - y);
    stroke(#0000FF);
    line(xPos-xSpeed, zPrev, xPos, height - z);
   
    if (xPos >= width) {
      xPos = 0;    
      //background(#000000);
    } else {   
      xPos+=xSpeed;
      xPrev = height - x;
      yPrev = height - y;
      zPrev = height - z;
    }
  }
  myPort.write('A');
} 

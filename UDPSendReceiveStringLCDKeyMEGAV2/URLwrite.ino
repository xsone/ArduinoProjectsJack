
//float TestTemp = 16.6;
//float SetPoint = 20.1;

//void URLwrite(String Kamer, String CVstate, String Humidity, String Temperature, String Move, String Smoke)
void URLwrite()
{
  EthernetClient client = CVserver.available();
  if (client) {
    boolean currentLineIsBlank = true;
    Serial.println("We WEER a new client");
    
    // an http request ends with a blank line
    while (client.connected()) {
       if (client.available()) {
        char c = client.read();
        httpData += c;
        Serial.println(httpData);
        //Serial.println(httpData.substring(0,4));
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        //if (c == '\n' && currentLineIsBlank) {
        if (c == '\n' ) {  
        //if (httpData.substring(5,15) == "?cmd=CVweb"){
          //if (httpData.substring(0,4) == "GET"){  
        //if (httpData == "\r\n") {
  
          // send a standard http response header
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/html");
              //client.println("Connection: close");  // the connection will be closed after completion of the response
    	      client.println("Refresh: 5");  // refresh the page automatically every 5 sec
              client.println();
              client.println("<!DOCTYPE HTML>");
              client.println("<html>");
              client.println("<meta http-equiv=refresh content=5>");
              client.println("<body bgcolor=\"#E0FFFF\">");
              client.println("<center><p><h1><align=center>Energylogger & Heatingcontrol Webserver  v1.4  </h1></p> ");
              client.println("<center><p><h1><align=center>");
              client.println(String(tijdNu));
              client.println("</h1></p> ");
              client.println("<DIV style=text-align: center;>");
              client.println("<TABLE style=margin: 0px auto; border=5>");
              client.println("<COL CLASS=kolom1><COL CLASS=kolom2><COL CLASS=kolom3><COL CLASS=kolom4><COL CLASS=kolom5><COL CLASS=kolom6><COL CLASS=kolom7><COL CLASS=kolom8>");
              //client.println("<TR BGCOLOR=\"#99CCFF\"><TH CLASS=cel1 links>KAMER</TH><TH CLASS=cel2> CV-STATE </TH><TH CLASS=cel3>HUMIDITY</TH><TH CLASS=cel4>TEMPERATURE</TH><TH CLASS=cel5>MOVE</TH><TH CLASS=cel6>SMOKE</TH></TR>");
              client.println("<TR BGCOLOR=\"#99CCFF\"><TH CLASS=cel1 links> KAMER </TH><TH CLASS=cel2> TEMPERATURE </TH><TH CLASS=cel3> SETPOINT </TH><TH CLASS=cel4> HUMIDITY </TH><TH CLASS=cel5> CV-STATE </TH><TH CLASS=cel6> MOVE </TH><TH CLASS=cel7> SMOKE </TH></TR>");
              // Volgorde strings Kamer[5], Temperature[4], Setpoint[4], Humidity[4], CVstate[1], Move[1], Smoke[1] totale lengte: 20 chars
              client.println("<TR ALIGN=center><TD>" + String(Kamer) + "</TD><TD>" + String(Temperature) + " &#8451" + "</TD><TD>" + String(Setpoint) + " &#8451" + "</TD><TD>" + String(Humidity) + " % RH" + "</TD><TD>" +  String(CVstate) +"</TD><TD>" +  String(Move) + "</TD><TD>" + String(Smoke) + "</TD></TR>");
              client.println("<TR ALIGN=center><TD>" + String(Kamer) + "</TD><TD>" + String(Temperature) + " &#8451" + "</TD><TD>" + String(Setpoint) + " &#8451" + "</TD><TD>" + String(Humidity) + " % RH" + "</TD><TD>" +  String(CVstate) +"</TD><TD>" +  String(Move) + "</TD><TD>" + String(Smoke) + "</TD></TR>");
              client.println("</TABLE></DIV><br>");
              /* 
              //Naar MySQL database op webserver          
              client.println("<form action=http://192.168.2.10/phpMyAdmin/mysqlvul.php method=get>");
              //client.println("<form action=http://77.169.66.57/phpMyAdmin/mysqlvul.php method=get>");
              client.println("<input type=hidden name=GAS value=");
              //client.print(GasIntWaarde);
              client.print(">");
              client.println("<input type=hidden name=WATER value=");
              //client.println(WaterIntWaarde);
              client.print(">");
              client.println("<input type=hidden name=ELEC value=");
              //client.println(ElecKwhWaarde);
              client.print(">");
              client.print("</font></h2><br>");          
              //client.print("  M&#179</font></h2><br>");
              //lcd.clear();
              //lcd.printIn("Gas: ");
              //lcd.printIn(GasCharWaarde); 
              */       
         
              //client.println("<input type=hidden name=cmd value=1>");
              client.println("<input type=submit value=\"Actual values to database\" value=1></form>");
              client.println("</center><br><p> (c) jcp-2015 <a href=\"http://217.120.27.59>www.sens4x.com<a>");
              client.println("</body></html>");
              client.println();
              client.println();
              break;
        } //if currline blank
        //if (c == '\n') currentLineIsBlank = true;
        //else if (c != '\r') currentLineIsBlank = false; 
         
         
         if (httpData.substring(5,16) == "?cmd=Setpnt") 
           { 
            Serial.print("HttpData test: "); 
            Serial.println(httpData);
            SetpointAndroid = "11"; //test
            //SetpointAndroid = httpData.substring(16,19);
            Serial.print("SetpointAndroid test: ");
            Serial.println(SetpointAndroid);
            SetpointAndroidChanged = true;
             
            client.println("HTTP/1.1 200 OK");
            //client.println("Content-Type: application/json");  // JSON response type
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            //client.print("{\"res\":\"OK\",\"cvstate\":\""); client.print(CVstate);
            //client.println("\"}");
            client.println();
            Serial.println(httpData);
            //digitalWrite(Relay, HIGH); 
            client.println();
            //httpData = "";
            break;
          }
         
         
          
          //if (CVstate = "AAN") digitalWrite(Relay, HIGH); 
          //if (CVstate = "UIT") digitalWrite(Relay, LOW); 
          //via Mobiele app 
          //Serial.println(httpData);
          if (httpData.substring(5,15) == "?cmd=CVaan") 
           { 
            
            CVstate = "AAN";
            client.println("HTTP/1.1 200 OK");
            //client.println("Content-Type: application/json");  // JSON response type
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            //client.print("{\"res\":\"OK\",\"cvstate\":\""); client.print(CVstate);
            //client.println("\"}");
            client.println();
            Serial.println(httpData);
            digitalWrite(Relay, HIGH); 
            client.println();
            //httpData = "";
            break;
          }
          
          if (httpData.substring(5,15) == "?cmd=CVuit") 
           { 
            
            CVstate = "UIT";
            client.println("HTTP/1.1 200 OK");
            //client.println("Content-Type: application/json");  // JSON response type
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            //client.print("{\"res\":\"OK\",\"cvstate\":\""); client.print(CVstate);
            //client.println("\"}");
            client.println();
            Serial.println(httpData);
            digitalWrite(Relay, LOW); 
            client.println();
            //httpData = "";
            break;
          }
          //if ( httpData.substring(5,15) == "?cmd=CVuit") { Serial.println("CV UIT"); CVstate = "CVuit"; digitalWrite(Relay, LOW); client.println();httpData = "";break;}
          
          //Actuele waarden via JSON
          if ( (httpData.substring(5,16) == "?cmd=ACTval"))
           {
             //Serial.println("JSON code...");
             Serial.println(httpData);
             client.println("HTTP/1.1 200 OK"); Serial.println("HTTP/1.1 200 OK");
             client.println("Content-Type: application/json");  // JSON response type
             client.println("Content-Type: text/html"); //eerder HTML
             //client.println("Content-Type: application/json;charset=utf-8");  // JSON response type
             //client.println("Server: CVserver");
             client.println("Connection: close");  // the connection will be closed after completion of the response
             client.println();
             //client.println("{\"temp\":\"55.2\"}"); // test werkt goed
             client.print("{\"res\":\"OK\",\"kamer\":\""); client.print(Kamer);
             client.print("\",\"temperature\":\""); client.print(Temperature);
             client.print("\",\"setpoint\":\""); client.print(Setpoint);
             client.print("\",\"humidity\":\""); client.print(Humidity);
             client.print("\",\"move\":\""); client.print(Move);
             client.println("\"}");
             client.println();
             
             //Volgorde strings Kamer[5], Temperature[4], Setpoint[4], Humidity[4], CVstate[1], Move[1], Smoke[1] totale lengte: 20 chars
             
            // open JSON   // example : {"res":"OK","temp":"25.0","pres":"1020","humi":"45"}
            // open JSON   // example : ("{\"res":\"OK",\"temp":\"25.0",\"pres":\"1020",\"humi":\"45"}\")
            //client.print("{");
            //client.print("\"res\":\"OK\"");
            //client.print(",\"temp\":\"");
            
            
            //client.print("{\"res\":\"OK\",\"temp\":\"");
            //client.print(Temperature);
            //client.print(TestTemp,1);
            //client.print("\"");
            //client.println("}");
            
            //Serial.println(TestTemp,1); //Serial.println(Temperature);
            
            // zie ook code CVRadiator V3
            //client.print(",\"setpoint\":\"");
            //client.print(SetPoint);
            //client.print(",\"cvstate\":\"");
            //client.print(CVstate);
            //client.print("Temp:     "); client.println(Temp);
            // Serial.println(Temp, DEC);  
            //client.print("SetPoint: "); client.println(SetPoint); 
            //Serial.println(SetPoint, DEC); 
            //client.print("CVstate:  "); client.println(CVstate); Serial.println(CVstate, DEC);
            
            //client.print("\"");
            //client.println("}");
            client.println();
            //c = '?';
            //client.flush();
            //client.stop();
            break;
           }         
          //httpData = "";    // finished with request, empty string
          //break;
       // } //if currline blank
       // if (c == '\n') currentLineIsBlank = true;
       // else if (c != '\r') currentLineIsBlank = false;
     
       } // if client avail
   
    } // end while
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    Serial.println("client disonnecting");
    client.flush();
    client.stop();
    httpData = "";
   } // if client
  //else client.flush(); // als client wegvalt
  //CVreadUDP();
}  
  
  
  
 /* 
  
  client.println("HTTP/1.1 200 OK");
 //client.println("Content-Type: application/json");  // JSON response type
  client.println("Content-Type: text/html"); //eerder HTML
  //client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println();
       // open JSON
       // example : {"res":"OK","temp":"25.0","pres":"1020","humi":"45"}
  client.print(Message);
  client.println();
  delay(1);
  client.stop(); 
}

*/

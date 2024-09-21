


void URLwrite(String CVstate, String Humidity, String Temperatuur, String ZonPuls, String ZonKWh)
{
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Got a client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //client.println("<meta http-equiv=refresh content=2>");
          client.println("<body bgcolor=\"#E0FFFF\">");
          client.println("<center><p><h1><align=center>Energylogger & Heatingcontrol Webserver  v1.3  </h1></p> ");
          client.println("<table border=5>");
          client.println("<COL CLASS=kolom1><COL CLASS=kolom2><COL CLASS=kolom3><COL CLASS=kolom4>");
          client.println("<TH CLASS=cel1 links>KAMER</TH><TH CLASS=cel2>CV-STATE</TH><TH CLASS=cel3>HUM</TH><TH CLASS=cel4>TEMP</TH>");
          client.println("<TR><TD CLASS=links>JACK</TD><TD>");
          client.println("CV status: " + String(CVstate));
          client.println("</TD><TD>");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("</TD><TD>");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("</TD><TD>?</TD><TD>");
          client.println("<TR><TD CLASS=links>MAURITS</TD><TD>");
          client.println("CV status: " + String(CVstate));
          client.println("</TD><TD>");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("</TD><TD>");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("</TD><TD>?</TD><TD>");
          client.println("<TR><TD CLASS=links>WOONKAMER</TD><TD>");
          client.println("CV status: " + String(CVstate));
          client.println("</TD><TD>");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("</TD><TD>");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("</TD><TD>?</TD><TD>");         
          client.println("</TD><TD>");
          //client.println(WaterLtrWaarde);
          client.println("</TD><TD>C</TD><TD>");
          client.println("<TR><TD CLASS=links>ZON PULS</TD><TD>");
          client.println("Zonpuls: " + String(ZonPuls) + " pls");
          //client.println(ElecIntWaarde);
          client.println("</TD><TD>");
          client.println("ZonKWh: " + String(ZonKWh)+ " KWh");
          client.println("</TD><TD>KWh</TD><TD>");
          client.println("<TR><TD CLASS=links>SOLAR</TD><TD>0</TD><TD>0</TD><TD>KWh</TD></TR>");
          client.println("<TR><TD CLASS=links>WIND</TD><TD>0</TD><TD>0</TD><TD>KWh</TD></TR>");
          client.println("</TABLE><br>");

/*
          client.print("<h2> GAS  : <font color=\"#00FF00\">");
          client.print(GasIntWaarde/2); //interrupt=CHANGE: dus 2 pulsen is 1x spiegeltje langs. 1000 pulsen = 1M3
          client.print("  Pulses</font></h2><br>");
          client.print("<h2> WATER: <font color=\"#00FF00\">");
          client.print(WaterIntWaarde/2); //interrupt=CHANGE: dus 2 pulsen is 1x spiegeltje langs 
          client.print("  Pulses</font></h2><br>");
          //client.print("  M&#179</font></h2><br>");
          client.print("<h2> ELEC : <font color=\"#00FF00\">");
          client.print(ElecIntWaarde);
          client.print("  Pulses</font></h2><br>");    
*/          
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
          
          /*
          //client.print("  M&#179</font></h2><br>");
          //lcd.clear();
          //lcd.printIn("Gas: ");
          //lcd.printIn(GasCharWaarde); 
              
          */       
        //client.println("<input type=hidden name=cmd value=1>");
        client.println("<input type=submit value=\"Actual values to database\" value=1></form>");
        //client.println("</center><br><p> (c) jcp-2015 <a href=\"http://217.120.27.59">www.jacks.com<a>");
        client.println("</center><br><p> (c) jcp-2015 <a href=\"http://217.120.27.59">"http://217.120.27.59<a>");
        client.println("</body></html>");
        
          
          
          
          /*
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("<body bgcolor=\"#E0FFFF\">");
          //client.println("<center><p><h1>CV + Zon + Douche Webserver  v1.2<align=center></h1></p> ");
          client.println("<H1>CV + Zon + Douche Webserver  v1.2<align=center></H1>");
          */
          client.println();
          // print the current readings, in HTML format:
          client.println("<br />");
          client.println("CV status: " + String(CVstate));
          client.println("<br />");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("<br />");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("<br />");
          client.println("Zonpuls: " + String(ZonPuls) + " p");
          client.println("<br />");
          client.println("ZonKWh: " + String(ZonKWh)+ " KWh");
          client.println("<br />");
          client.println();
          client.println();
          client.println("Kamer Jack");
          client.println("<br />");
          client.println("Humidity: " + String(Humidity) + " % RH");
          client.println("<br />");
          client.println("Temperature: " + String(Temperatuur) + " C");
          client.println("<br />");
          client.println("Beweging: " + String(Beweging) + " C");
          client.println("<br />");
          client.println("Rook: " + String(Rook) + " C");
          client.println("<br />");
          client.println("Koolmonox: " + String(Koolmonox) + " C");
          client.println("<br />"); 
          client.println("</html>");    
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
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

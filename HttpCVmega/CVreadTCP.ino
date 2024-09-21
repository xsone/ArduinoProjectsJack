// buffers for receiving and sending data
//boolean alreadyConnected = false; // whether or not the client was connected previously
//String httpData;

// read the packet into packetBufffer

/*

void CVreadTCP()
{
  // when the client sends the first byte, say hello:
  // wachten of CVclient (per kamer) zich aandient
EthernetClient client = arduinoCVserver.available();
  
  if (client) 
  {
    boolean currentLineIsBlank = true;
//    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true;
    }

    if (client.available() > 0) 
     {
      char c = client.read();
      httpData += c;
      //Serial.print(httpData);
      if (c == '\n' && currentLineIsBlank) 
       {
        
        if ( httpData == "CVaan") { CVstate = "CVaan"; digitalWrite(Relay, HIGH); }
        if ( httpData == "CVuit") { CVstate = "CVuit"; digitalWrite(Relay, LOW);  }
        Serial.println(httpData);
        Serial.println(CVstate);        
        httpData = "";    // finished with request, empty string
        //break;
       }
     if (c == '\n') currentLineIsBlank = true;
     else if (c != '\r') currentLineIsBlank = false;
    } //end client avail
  } // end if client
}    

*/

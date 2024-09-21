// buffers for receiving and sending data
char udpData [UDP_TX_PACKET_MAX_SIZE];
char udpReply[] = "acknowledged";    // a oke string to send back

// lees het UDP-pakketje into packetBufffer
void CVreadUDP()
{
  // if there's data available, read a packet
 int packetSize = Udp.parsePacket();
 //Udp.parsePacket();
 if(packetSize) 
  {
   Serial.print("Received packet of size "); Serial.println(packetSize);
   Udp.read(udpData,UDP_TX_PACKET_MAX_SIZE);
   Serial.print("UdpData: "); Serial.println(udpData);
   Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
   Udp.write(udpReply);
   Udp.endPacket();
   
   /*
   // Zo ziet de opbouw van het bericht vanaf de radiator eruit
   // Kamer[5], Temperature[4], Setpoint[2], Humidity[4], CVstate[1], Move[1], Smoke[1] totale lengte: 17 chars
   // Kamer[0-4], Temperature[4-8], Setpoint[8-10], Humidity[10-14], CVstate[14], Move[15], Smoke[16] totale lengte: 17 chars
   // String Buffer = Kamer + String(Temp,1) + String(SetPoint) + String(Hum,1) + CVstate + Move + Smoke;
   */
   
   String Buffer = String(udpData);
   Kamer = String(Buffer.substring(0,4));
   Temperature = String(Buffer.substring(4,8));
   //Setpoint = String(Buffer.substring(8,10));
   SetpointArduino = String(Buffer.substring(8,10));
   Humidity  = String(Buffer.substring(10,14));// oud
   
   if (SetpointArduino != SetpointArduinoOld) { SetpointArduinoChanged = true;}
   //else Setpoint = SetpointArduino;
   //if (SetpointAndroid != Setpoint) { Setpoint = SetpointAndroid;}
   //else Setpoint = SetpointArduino;
   
   if (Buffer[14] == 'H') { CVstate = "AAN"; digitalWrite(Relay, HIGH);}
   if (Buffer[14] == 'L') { CVstate = "UIT"; digitalWrite(Relay, LOW); }
   if (Buffer[15] == 'H') { Move = "AAN"; digitalWrite(TestLed, HIGH);}
   if (Buffer[15] == 'L') { Move = "UIT"; digitalWrite(TestLed, LOW); }
   if (Buffer[16] == 'H') { Smoke = "AAN"; }
   if (Buffer[16] == 'L') { Smoke = "UIT"; }
   Serial.print("Buffer: "); Serial.println(Buffer);
   Serial.print("Setpoint: "); Serial.println(Setpoint);
   Serial.print("SetpointArduino: "); Serial.println(SetpointArduino);
   Serial.print("SetpointAndroid: "); Serial.println(SetpointAndroid);
   SetpointArduinoOld = SetpointArduino;
  }
 delay(10); // tijd om bericht te verwerken
 //delay(1000); // tijd om bericht te verwerken
}    

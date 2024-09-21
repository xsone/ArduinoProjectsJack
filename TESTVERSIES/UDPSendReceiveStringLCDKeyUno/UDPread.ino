// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back


// read the packet into packetBufffer
void CVreadUDP()
{
  // if there's data available, read a packet
 if (Udp.parsePacket())
  {
   IPAddress remote = Udp.remoteIP();
   Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
   Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
   Udp.write(ReplyBuffer);
   Udp.endPacket();
  }
 if ( strncmp(packetBuffer, "CVaan", 5) == 0)
  {
   Serial.println("CV staat AAN");
   //URLwrite(Message);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("CV staat AAN");
   digitalWrite(Relay, HIGH);
   CVstate = "CV staat AAN";
  } 
 if ( strncmp(packetBuffer,"CVuit", 5) == 0)
  { 
   Serial.println("CV staat UIT");
   //URLwrite(Message);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("CV staat UIT");
   digitalWrite(Relay, LOW);
   CVstate = "CV staat UIT";
  }
}    

// buffers for receiving and sending data
char UDPpacketBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
//char ReplyBuffer[] = "acknowledged";       // a string to send back
char ReplyBuffer[] = "ackno";       // a string to send back


// read the packet into packetBufffer
void CVreadUDP()
{
  // if there's data available, read a packet
 Udp.parsePacket();
 IPAddress remote = Udp.remoteIP();
 Udp.read(UDPpacketBuffer,UDP_TX_PACKET_MAX_SIZE);
 Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
 Udp.write(ReplyBuffer);
 Udp.endPacket();
 if ( strncmp(UDPpacketBuffer, "CVaan", 5) == 0) { CVstate = "CVaan"; digitalWrite(Relay, HIGH); }
 if ( strncmp(UDPpacketBuffer, "CVuit", 5) == 0) { CVstate = "CVuit"; digitalWrite(Relay, LOW);  }
 Serial.print("packetBuffer :");
 Serial.println(CVstate);
 Serial.println(UDPpacketBuffer);
 //CVstate = UDPpacketBuffer;
 UDPpacketBuffer[0] = 0;
}    

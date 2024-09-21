//#include <Ethernet.h>
//#include <EthernetUdp.h>
//#include <Time.h>

#define TZ_OFFSET -3600L  // time zone offset - set deze offset in seconds to.v. je local time //Zomertijd dan -2hrs (-7200L sec); Wintertijd dan -1hrs (-3600L sec)


//Server server(80);
//unsigned int localPort = 8888;
//IPAddress timeServer(132, 163, 4, 101); // time-c.timefreq.bldrdoc.gov NTP server werkt!
IPAddress timeServer(131, 211, 8, 244); // pool.ntp.org NTP server in Ziggo router werkt soms! Onbetrouwbaar

const long NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message
byte NTPpacketBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// Tijd instellingen
//time_t t = 0UL; 
//String tijdNu;
//String tijdLog = "21:00:00";


//NTP externe code
unsigned long getNtpTime()
{
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() ) {
    // We've received a packet, read the data from it
    Udp.read(NTPpacketBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(NTPpacketBuffer[40], NTPpacketBuffer[41]);
    unsigned long lowWord = word(NTPpacketBuffer[42], NTPpacketBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    
    // now convert NTP time into Unix time
    // starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;  
    return epoch - TZ_OFFSET;
  }
  return 0; // return 0 if u`````  qqnable to get the time
}

// send an NTP request to the time server at the given address 
unsigned long sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(NTPpacketBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  NTPpacketBuffer[0] = 0b11100011;   // LI, Version, Mode
  NTPpacketBuffer[1] = 0;     // Stratum, or type of clock
  NTPpacketBuffer[2] = 6;     // Polling Interval
  NTPpacketBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  NTPpacketBuffer[12]  = 49; 
  NTPpacketBuffer[13]  = 0x4E;
  NTPpacketBuffer[14]  = 49;
  NTPpacketBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp: 		   
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(NTPpacketBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}

// Vertaal naar tijdwaarde
String getTimeString() {  
  // gives back hh:mm:ss  
  time_t t = now();  
  String s = "";  
  if (hour(t) <10) s = s + "0";  
  s = s + hour(t) + ":"; 
  if (minute(t) <10) s = s + "0";  
  s = s + minute(t) + ":";  
  if (second(t) <10) s = s + "0";  
  s = s + second(t);  
  return(s);  
}  

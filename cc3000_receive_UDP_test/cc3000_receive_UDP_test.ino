
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include "utility/socket.h"
#include <string.h>
#include "utility/debug.h"


//-----------------CC3000 and network info plus interrupt and control pins--------//
#define ADAFRUIT_CC3000_IRQ 3//  3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT 5 //5 previously pin 5 in all examples - 5 clashes with octows2811 library pins
#define ADAFRUIT_CC3000_CS   10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIV2);
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
Adafruit_CC3000_Client client;


#define WLAN_SSID   "TRAFFICBOX2"        // cannot be longer than 32 characters!
#define WLAN_PASS   "Z3zhhjyS2nbm"


//network location
byte mac[] = {0x08, 0x00, 0x28, 0x57, 0xAC, 0x96};//set you mac address (you can find it using buildtest example)
//unsigned long ip = 0xc0a8010a;//comment out if using static IP method

//-------for wifi connection timeout error------//
const unsigned long
  dhcpTimeout     = 60L * 1000L, // Max time to wait for address from DHCP
  connectTimeout  = 15L * 1000L; // Max time to wait for server connection
uint32_t t;

//int socket ( long domain , long type , long protocol );
/* domain selects the protocol family which will be used for communication. AF_INET is supported. */
/* type specifies the communication semantics. SOCK_STREAM and SOCK_DGRAM, SOCK_RAW are supported. */
/* protocol specifies a particular protocol to be used with the socket. IPPROTO_TCP, IPPROTO_UDP or IPPROTO_RAW are supported. */
/* On success the return value is the socket handle that is used for consequent socket operations. On error, -1 is returned. */

//int recvfrom ( long sd , void * buf , long len , long flags , sockaddr * from , socklen_t * fromlen );
//recvfrom() will write the senders IP and port number into the from structure.//
/* sd specifies the socket handle. */
/* buf points to the buffer where the message should be stored. */
/* len specifies the length in bytes of the buffer pointed to by the buffer argument. Range: 1-1460 bytes. */
/* flag specifies the type of message reception. On this version, this parameter is not supported. */
/* len specifies the length in bytes of the buffer pointed to by the buffer argument. Range: 1-1460 bytes. */
/* from is a pointer to an address structure indicating the source address: sockaddr. */
/* On success the return return the number of bytes received. On error, -1 is returned. 0 if timeout occurred. */

// useful reading for sockets and UDP http://en.wikipedia.org/wiki/Berkeley_sockets 

//-------------------for UDP socket, bind and receive--------//
unsigned long port = 8888;
long sockfd;
unsigned long localPort;
sockaddr_in  socketAddr, from;
socklen_t sockLen;
unsigned char buff[64];

void setup() {  
  Serial.begin(9600);
//------ Initialise the CC3000 module----//
  cc3000.begin();
  memset(&socketAddr, 0, sizeof(sockaddr_in));

 /*-------------setting a static IP Address--------------------  */
//unsigned long IPAdd[1] = {192.168.178.82};//use this to set it to static//192.168.1.10
//unsigned long SubNetMask[1] = {255.255.255.0};
//unsigned long dfGW[1] = {192.168.178.1};
//unsigned long DNSServer[1] = {192.168.178.1};

//   if (!cc3000.setStaticIPAddress(IPAdd, SubNetMask, dfGW, DNSServer))
  
  uint32_t ipAddress = cc3000.IP2U32(192, 168, 178, 82);
  uint32_t netMask = cc3000.IP2U32(255, 255, 255, 0);
  uint32_t defaultGateway = cc3000.IP2U32(192, 168, 178, 1);
  uint32_t dns = cc3000.IP2U32(192, 168, 178, 1);
  if (!cc3000.setStaticIPAddress(ipAddress, netMask, defaultGateway, dns)) {
  {
      Serial.println(F("Failed to set static IP"));
      while(true);
    }
  } 



//requires changes to cc3000.cpp and .h files for static IP to work!!!!!!//
//for info on setting up the cc3000 - http://learn.adafruit.com/downloads/pdf/adafruit-cc3000-wifi.pdf
//for changes to files for setting a static address se http://forums.adafruit.com/viewtopic.php?f=31&t=47004
/*-------------------- End setting static IP Address---------------------*/

//------ Connect to  WiFi network
cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  Serial.println("Connected to WiFi network!"); 
//------ Check DHCP
  Serial.print(F("Requesting address from DHCP server..."));
  for( t=millis(); !cc3000.checkDHCP() && ((millis() - t) < dhcpTimeout); delay(1000));
  if(cc3000.checkDHCP()) {
    Serial.println(F("OK"));
  } else {
    Serial.println(F("failed"));
    return;
  }
/* Wait for DHCP to complete */
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }  

/* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

//--------connect the UDP port-----//
  int  startTime = millis();
    do {
      client = cc3000.connectUDP(ipAddress, port);
      
    } while((!client.connected()) &&
            ((millis() - startTime) < connectTimeout));

    if(client.connected()) {
      Serial.print(F("connected to UDP port  ")); 
      Serial.println(port);
    }

//----------set up the socket------//
  socketInit();
}
//------------end of setup---------------------//

void loop() {
//------get the received data and print to serial------//  
  grabInput();
    }


/**************************************************************************/
/*!
    @brief  Displays the driver mode (tiny of normal), and the buffer
            size if tiny mode is not being used

    @note   The buffer size and driver mode are defined in cc3000_common.h
*/
/**************************************************************************/
void displayDriverMode(void)
{
  #ifdef CC3000_TINY_DRIVER
    Serial.println(F("CC3000 is configure in 'Tiny' mode"));
  #else
    Serial.print(F("RX Buffer : "));
    Serial.print(CC3000_RX_BUFFER_SIZE);
    Serial.println(F(" bytes"));
    Serial.print(F("TX Buffer : "));
    Serial.print(CC3000_TX_BUFFER_SIZE);
    Serial.println(F(" bytes"));
  #endif
}

/**************************************************************************/
/*!
    @brief  Tries to read the CC3000's internal firmware patch ID
*/
/**************************************************************************/
uint16_t checkFirmwareVersion(void)
{
  uint8_t major, minor;
  uint16_t version;
  
#ifndef CC3000_TINY_DRIVER  
  if(!cc3000.getFirmwareVersion(&major, &minor))
  {
    Serial.println(F("Unable to retrieve the firmware version!\r\n"));
    version = 0;
  }
  else
  {
    Serial.print(F("Firmware V. : "));
    Serial.print(major); Serial.print(F(".")); Serial.println(minor);
    version = major; version <<= 8; version |= minor;
  }
#endif
  return version;
}

/**************************************************************************/
/*!
    @brief  Tries to read the 6-byte MAC address of the CC3000 module
*/
/**************************************************************************/
void displayMACAddress(void)
{
  uint8_t macAddress[6];
  
  if(!cc3000.getMacAddress(macAddress))
  {
    Serial.println(F("Unable to retrieve MAC Address!\r\n"));
  }
  else
  {
    Serial.print(F("MAC Address : "));
    cc3000.printHex((byte*)&macAddress, 6);
  }
}


/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

/**************************************************************************/
/*!
    @brief  Begins an SSID scan and prints out all the visible networks
*/
/**************************************************************************/

void listSSIDResults(void)
{
  uint8_t valid, rssi, sec, index;
  char ssidname[33]; 

  index = cc3000.startSSIDscan();

  Serial.print(F("Networks found: ")); Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);
    
    Serial.print(F("SSID Name    : ")); Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}

//==========test to get input from cc3000 and print to serial=================//
void grabInput()
{
  Serial.println("I'm in grabInput");
  
//------check client socket open----//
    if(client.connected()) {
      Serial.print(F("connected to UDP port  ")); 
      Serial.println(port);
    } 
//-------end of socketcheck------//

  int inData;
  Serial.print("inData start value = ");Serial.println(inData);
  sockLen = sizeof(sockaddr_in);
 Serial.println("receive test ...");
 inData = recvfrom(sockfd, buff, sizeof(buff), 0, (sockaddr*)&from, &sockLen);
  
 //-----print buffer contents------//
  for(int i=0;i<sizeof(buff);i++){
   Serial.print(buff[i]);}
   Serial.println("");

   if(inData > 0){
     Serial.println("in");}
       else { Serial.println("nothing received");
       }
}

void socketInit(){
   //-----set a socket and check for it, then bind it-----//
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0){
        Serial.println("socket failure");}
      else {
        Serial.print ("Sockfd = ");
        Serial.println (sockfd);      }
        
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_addr.s_addr = 0;
    socketAddr.sin_port = htons(port/*localPort*/);
    bind(sockfd, (sockaddr*)&socketAddr, sizeof(sockaddr_in));
    Serial.print("check for sockfd number = ");Serial.println(sockfd);
    Serial.print("check for port number = ");Serial.println(localPort);
    Serial.println(", end of socketInit();");
  }
  
  

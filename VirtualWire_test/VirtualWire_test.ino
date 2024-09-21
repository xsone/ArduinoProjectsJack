#include <VirtualWire.h>

uint8_t currentPacket = 0; 
uint8_t counter = 0; 

void setup() {
  Serial.begin(9600);
  Serial.println("Start Virtual Wire Test...");
  
  /*
   * Set TX pin -> Connect FS1000A here
   */
  vw_set_tx_pin(12);
  /*
   * Set RX pin -> Connect XY-MK-5V here
   */
  vw_set_rx_pin(11);

  /*
   * Set transmission speed. Reciver and trasmitter have to use the same speed
   */
  vw_setup(1000);
  /*
   * Start to listen. Required on receiver side
   */
  vw_rx_start();
}

void transmit() {
 const char *msg = "hello";
 vw_send((uint8_t *)msg, strlen(msg));
 delay(400);
}

void receive() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) {
    int i;
    // Message with a good checksum received, dump HEX
    Serial.print("Got: ");
    for (i = 0; i < buflen; i++)
      {
        Serial.print(buf[i], HEX);
        Serial.print(" ");
      }
    Serial.println("");
//    /*
//     * Recived packet, if checksum is correct, is avalable in buf variable
//     * Send it over Serial, or do whatever you want with it
//     */
//    currentPacket = (uint8_t) buf[0];
//    Serial.println(currentPacket);
  }
}
  
void loop(){
  transmit();
  receive();
}

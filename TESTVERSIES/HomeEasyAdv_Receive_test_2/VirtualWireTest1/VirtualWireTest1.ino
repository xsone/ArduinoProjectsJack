//simple Tx on pin D12
//Written By : Mohannad Rawashdeh
// 3:00pm , 13/6/2013
//http://www.genotronex.com/
//..................................
#include <VirtualWire.h>
void setup()
{
    Serial.begin(9600);
    Serial.println("Start 433MHz uitlezen...");
    //vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_rx_pin(11);
    vw_setup(2000);  // Bits per sec
    pinMode(13, OUTPUT);

    vw_rx_start();       // Start the receiver PLL running
}
    void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    vw_get_message(buf, &buflen); // Non-blocking
    Serial.print(buf[0]);
    Serial.print(buf[1]);
    Serial.println(buf[2]);
    delay(1000);
}

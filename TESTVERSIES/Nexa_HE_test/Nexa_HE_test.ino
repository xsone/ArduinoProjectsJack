#include <NexaCtrl.h>

#define TX_PIN 13
#define RX_PIN 8

// Your remote id which can be it can be "sniffed" with the 
// reciever example at http://playground.arduino.cc/Code/HomeEasy
const static unsigned long controller_id = 55779;
unsigned int device = 0;

NexaCtrl nexaCtrl(TX_PIN, RX_PIN);

void setup()
{
    nexaCtrl.DeviceDim(controller_id, device, 0);
    delay(3000);
    nexaCtrl.DeviceDim(controller_id, device, 15);
    delay(3000);
    nexaCtrl.DeviceDim(controller_id, device, 0);

    delay(3000);

    nexaCtrl.DeviceOn(controller_id, device);
    delay(3000);
    nexaCtrl.DeviceOff(controller_id, device);

    delay(3000);

    nexaCtrl.GroupOff(controller_id);
    delay(3000);
    nexaCtrl.GroupOn(controller_id);
}

void loop()
{
}

/****************************************************************************
CAN Read Demo for the SparkFun CAN Bus Shield. 

Written by Stephen McCoy. 
Original tutorial available here: http://www.instructables.com/id/CAN-Bus-Sniffing-and-Broadcasting-with-Arduino
Used with permission 2016. License CC By SA. 

Distributed as-is; no warranty is given.
*************************************************************************/

#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>

char UserInput;
int data;
char buffer[456];  //Data will be temporarily stored to this buffer before being written to the file

//********************************Setup Loop*********************************//

void setup() {
  Serial.begin(9600); // For debug use
  Serial.println("CAN Read - Testing receival of CAN Bus message v1.1");  
  delay(1000);
  
  if(Canbus.init(CANSPEED_500))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
    
  delay(1000);
}

//********************************Main Loop*********************************//

void loop(){

  tCAN message;

 //VW CAN-bus moet een trigger hebben om het lezen te starten
 Canbus.ecu_req(PID_REQUEST, buffer);
 //Canbus.ecu_req(ENGINE_COOLANT_TEMP, buffer);
 //Serial.print("Engine Coolant Temp: ");
 //Serial.println(buffer);
 //delay(500);

if (mcp2515_check_message()) 
	{
    if (mcp2515_get_message(&message)) 
	{
        //if(message.id == 0x620 and message.data[2] == 0xFF)  //uncomment when you want to filter
             //{
               
               Serial.print("ID: ");
               Serial.print(message.id,HEX);
               Serial.print(", ");
               Serial.print("Data: ");
               Serial.print(message.header.length,DEC);
               for(int i=0;i<message.header.length;i++) 
                {	
                  Serial.print(message.data[i],HEX);
                  Serial.print(" ");
                }
               Serial.println("");
             //}
           }}

}

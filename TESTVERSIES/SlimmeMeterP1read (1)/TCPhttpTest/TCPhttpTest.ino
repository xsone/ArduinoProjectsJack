
/*
    Ethernet TCP client/server communications test/proof of concept
    Programmed by Michael E. Landon on 09/20/2012
    (With parts borrowed/stolen from Nick Gammon)
    
    This is a primitive example of using the Arduino Uno w/Ethernet Shield
    with outputs controlled and monitored through a TCP connection.
    
    Reading an output's state:
      Format:    rx    (where x= output to read. 0-7 valid in this test)
      
    Writing to an output:
      Format:    wx:y  (where x = output to write to. 0-7 valid in this test)
                       (y = boolian state to write. (0 or 1))  
    
    Output 0 = pin 2, Output 1 = pin 3, etc. 
    Only 8 contiguous outputs were available for this test, hence only 8 addressed.
    
    Posted in the Arduino Forums to assist those who like me needed to see
    how it was done!
   
*/

#include <SPI.h>
#include <Ethernet.h>


// Set values below to match your network needs:
byte mac[] = {0x54, 0x52, 0x49, 0x41, 0x44, 0x00};   // MAC Address
byte ip[] = {10, 0, 0, 47};                          // Network Address
byte gateway[] = {10, 0, 0, 1};                      // Gateway Address
byte subnet[] = {255, 255, 255, 0};                  // Subnet Mask
EthernetServer server(23);                           // Set Server Port
EthernetClient client;                               // Define client
const int MAX_LEN = 10;                              // Maximum length of a command


void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);          // Start the Ethernet connection
  server.begin();                                    // Begin acting like a server
  for(int x = 2; x < 9; x++)                         // Set up pins 2-9
  {                                                  // (we will call them 0-7 in our commands)
    pinMode(x, OUTPUT);                              // Make them all outputs
  }                                                  // This is the maximum we can do with an UNO
}                                                    // A MEGA could easily handle more

void loop()
{
  char command[MAX_LEN];        // Set up the command buffer
  static int c = 0;             // Set up a counter (static so it does not get overwritten outside of this loop
  char inByte;                  // Set up a character buffer to grab the input

  client = server.available();  // Check for server availability
  if(client)
  {
    inByte = client.read();     // Read the character in the buffer
    switch (inByte)             // check for certain types
    {
    case 13:                    // carriage return: process the command
      command[c] = 0;           // make the current position a terminating null byte
      DoCommand(command);	// Process the command
      c = 0;                    // reset counter for next time
      break;                    // exit when done
    case 10:			// linefeed: Ignore it
      break;                    // and skip to the end
    default:                    // any character other than a CR or LF 
      if (c < (MAX_LEN - 1))	// put in the command string (until it's full)
      {
        command[c] = inByte;    // command concactination
        c++;                    // Increment our counter
      }
    }                           // end of switch
  }                             // end of if statement
}                               // end of main loop

void DoCommand(char * cmd)	// Command received: process it!
{
  switch (cmd[0])		// Check first character of the command:
  {
  case 'r':			// r = read pin's state
    ReadPins(cmd);		// Proceed to read the pin
    break;
  case 'w':			// w = write a state to a pin
    WritePins(cmd);		// Proceed to write to the pin
    break;
  default:
    client.println("ERROR_01"); // ERROR_01 = bad command - only 'r' and 'w' are acceptable
  }
}

void ReadPins(char * cmd)	// Read a pin state
{
  //Serial.println("Reading");	//debug	
  byte x;					
  x = cmd[1] - 46;		// We get an ASCII character of 0-7, which has a value in the range of 48-55
                                // by subtracting 46, we get a value of 2-9, which represent the pins used as outputs 0-7 for this test.
  if((x > 9) || (x < 2))	// If the result is outside this range, we throw an error.
  {
    client.println("ERROR_02"); // ERROR_02 = invalid pin number
    return;			// and exit before we do any further harm.
  }
  client.print("Output ");	// send out the TCP connection a message the output in question
  client.print(x - 2);
  if(digitalRead(x))
  {
    client.println(" is on.");  // is currently on..
  }
  else
  {
    client.println(" is off."); // or off (as the case may be)
  }
}				// done with reading

void WritePins(char * cmd)	// Write a pin state
{	
  byte x;
  x = cmd[1] - 46;		// We get an ASCII character of 0-7, which is a value in the range of 48-55
                                // by subtracting 46, we get a value of 2-9, which represent our 0-7 outputs for this test.
  if((x > 9) || (x < 2))	// If the result is outside this range, we throw an error.
  {
    client.println("ERROR_02"); // ERROR_02 = invalid pin number
    return;			// and exit before we do any further harm.
  }
  if(cmd[2] != ':')		// If the third character is not a colon, we throw an error.
  {
    client.println("ERROR_03"); // ERROR_03 = unknown separator 
    return;			// and exit before we do any further harm.
  }
  switch (cmd[3])		// check the 4th character
  {
  case '0':			// 0 = turn the output off
    digitalWrite(x, LOW);	// Write to the pin to turn it off
    client.print("Output ");	// Send out the TCP connection a message the output in question
    client.print(x - 2);
    client.println(" is off.");	// is now turned off.
    break;
  case '1':			// 1 = turn the output on
    digitalWrite(x, HIGH);	// Write to the pin to turn it on
    client.print("Output ");	// Send out the TCP connection a message the output in question
    client.print(x - 2);
    client.println(" is on.");	// is now turned on.
    break;
  default:
    client.println("ERROR_04");	// ERROR_04 = Only Boolean states allowed in this test.
  }
}


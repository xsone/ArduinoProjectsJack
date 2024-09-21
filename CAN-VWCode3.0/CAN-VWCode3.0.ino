//##############################################################################################################  

//Volkswagen CAN BUS Gaming
//Test Sketch v3.0
//(C) by Leon Bataille 2015-2016
//Hackaday Project Page: https://hackaday.io/project/6288-volkswagen-can-bus-gaming

//Sketch information #DO NOT REMOVE!
String game = "VW Static v3.0";
String vendor = "Leon Bataille";
//END Sketch information

//For individual setup scroll down to the section MAIN CONFIGURATION

//##############################################################################################################  

//Libraries
#include <mcp_can.h>            //CAN Bus Shield Compatibility Library
#include <SPI.h>                //CAN Bus Shield SPI Pin Library
#include <Servo.h>              //Library for Controlling Servos
#include <Wire.h>               //Extension Library for measuring current
#include <LiquidCrystal_I2C.h>  //Libraries for controlling LC Displays
#include <LCD.h>                
#include <LiquidCrystal.h>

//Definition
#define lo8(x) ((int)(x)&0xff)
#define hi8(x) ((int)(x)>>8)

//Variables (Dashboard LEDs)
int high_beam = 13;
int fog_beam = 12;
int park_brake = 11;

//Variables (VW Dashboard Adapter)
int LEFT_INDICATOR  = A0;
int RIGHT_INDICATOR = A1;
int PARKING_BREAK   = A2;
int FUEL_WARNING    = A3;

//Variables (Dashboard Functions)
int turning_lights = 3;
boolean turning_lights_blinking = true;
int turning_lights_counter = 0;
int temp_turning_lights = 0;
boolean backlight = false;
int oilpswitch = 8;
int pack_counter = 0;
boolean add_distance = false;
int drive_mode = 0;
int distance_multiplier = 2;
boolean oil_pressure_simulation = true;
boolean door_open = false;
boolean clutch_control = false;
int temp_clutch_control = 0;
boolean check_lamp = false;
int temp_check_lamp = 0;
boolean trunklid_open = false;
int temp_trunklid_open = 0;
boolean battery_warning = false;
int temp_battery_warning = 0;
boolean keybattery_warning = false;
int temp_keybattery_warning = 0;
int lightmode = 0;
boolean seat_belt = false;
int temp_seat_belt = 0;
int engine_control = 0;
int temp_dpf_warning = 0;
boolean dpf_warning = false;
boolean light_fog = false;
int temp_light_fog = 0;
boolean light_highbeam = false;
int temp_light_highbeam = 0;
boolean signal_abs = false;
boolean signal_offroad = false;
boolean signal_handbrake = false;
boolean signal_lowtirepressure = false;
boolean signal_dieselpreheat = false;
boolean signal_watertemp = false;
int temp_signal_abs = 0;
int temp_signal_offroad = 0;
int temp_signal_handbrake = 0;
int temp_signal_lowtirepressure = 0;
int temp_signal_dieselpreheat = 0;
int temp_signal_watertemp = 0;

//Variables (Speed and RPM)
int speed = 0;
byte speedL = 0;
byte speedH = 0;

int rpm = 0;
short tempRPM = 0;
byte rpmL = 0;
byte rpmH = 0;

int distance_adder = 0;
int distance_counter = 0;

//Variables LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlightpin, polarity

//Variables CAN BUS
/* the cs pin of the version after v1.1 is default to D9
v0.9b and v1.0 is default D10 */
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);  // Set CS pin

//Setup Configuration
void setup()
{
  //Initialize LCD
  lcd.begin(16,2);
  lcd.backlight();
  showLCD("VW Dashboard","by Leon Bataille");
  delay(2000);
  
  showLCD("VW Dashboard","booting...");
  
  //Define Dashboard LEDs as Outputs
  pinMode(high_beam, OUTPUT);
  pinMode(fog_beam, OUTPUT); 
  pinMode(park_brake, OUTPUT);
  pinMode(oilpswitch, OUTPUT);
  
  //Visual Start Sequence on Dashboard
  digitalWrite(high_beam, HIGH);
  digitalWrite(fog_beam, HIGH);
  digitalWrite(park_brake, LOW);
  digitalWrite(PARKING_BREAK, HIGH);
  delay(500);               // wait for a second
  digitalWrite(high_beam, LOW);
  digitalWrite(FUEL_WARNING, HIGH);
  digitalWrite(PARKING_BREAK, LOW);
  delay(500);
  digitalWrite(FUEL_WARNING, LOW);
  digitalWrite(LEFT_INDICATOR, HIGH);
  digitalWrite(RIGHT_INDICATOR, HIGH);
  digitalWrite(fog_beam, LOW);
  delay(500);
  digitalWrite(LEFT_INDICATOR, LOW);
  digitalWrite(RIGHT_INDICATOR, LOW);
  
  //Begin with Serial Connection
  Serial.begin(115200);
  
  //Show Sketch information on LCD
  showLCD(game,"by "+vendor);
  
  //Begin with CAN Bus Initialization
START_INIT:

    if(CAN_OK == CAN.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}


//Reload LC Display
void showLCD(String line1,String line2)
{  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}


//Send CAN Command (short version)
void CanSend(short address, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h)
{
  unsigned char DataToSend[8] = {a,b,c,d,e,f,g,h};
  CAN.sendMsgBuf(address, 0, 8, DataToSend);
}

//fuel gauge (0-100%)
void Fuel(int amount)
{
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
 
  if (amount >= 90)
  {
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    return;
  }
  if (amount >= 75)
  {
    pinMode(3, OUTPUT);
    digitalWrite(3, LOW);
    return;
  }
  if (amount >= 50)
  {
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
    return;
  }
  if (amount >= 25)
  {
    pinMode(5, OUTPUT);
    digitalWrite(5, LOW);
    return;
  }
  if (amount >= 10)
  {
    pinMode(6, OUTPUT);
    digitalWrite(6, LOW);
    return;
  }
  if (amount >= 0)
  {
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
    return;
  }
}

//Loop
void loop()
{
//##############################################################################################################  
  
  //MAIN SETUP FOR OPERATION

  //Set constant speed and RPM to show on Dashboard
  speed = 0;                        //Set the speed in km/h
  rpm = 800;                        //Set the rev counter
  Fuel(30);                          //Set the fuel gauge in percent
  
  //Set Dashboard Functions
  backlight = true;                  //Turn the automatic dashboard backlight on or off
  turning_lights = 0;                //Turning Lights: 0 = none, 1 = left, 2 = right, 3 = both
  turning_lights_blinking = false;   //Choose the mode of the turning lights (blinking or just shining)
  add_distance = false;              //Dashboard counts the kilometers (can't be undone)
  distance_multiplier = 2;           //Sets the refresh rate of the odometer (Standard 2)
  signal_abs = false;                //Shows ABS Signal on dashboard
  signal_offroad = false;            //Simulates Offroad drive mode
  signal_handbrake = false;          //Enables handbrake
  signal_lowtirepressure = false;    //Simulates low tire pressure
  oil_pressure_simulation = true;    //Set this to true if dashboard starts to beep
  door_open = false;                 //Simulate open doors
  clutch_control = false;            //Displays the Message "Kupplung" (German for Clutch) on the dashboard's LCD
  check_lamp = false;                //Show 'Check Lamp' Signal on dashboard. B00010000 = on, B00000 = off
  trunklid_open = false;             //Simulate open trunk lid (Kofferraumklappe). B00100000 = open, B00000 = closed
  battery_warning = false;           //Show Battery Warning.
  keybattery_warning = false;        //Show message 'Key Battery Low' on Display. But just after first start of dashboard.
  light_fog = false;                 //Enable Fog Light
  light_highbeam = false;            //Enable High Beam Light
  seat_belt = false;                 //Switch Seat Betl warning light.
  signal_dieselpreheat = false;      //Simualtes Diesel Preheating
  signal_watertemp = false;          //Simualtes high water temperature
  dpf_warning = false;               //Shows the Diesel particle filter warning signal.
  
  
//##############################################################################################################  
  
  //Conversion Speed
  speed = speed / 0.0075; //KMH=1.12 MPH=0.62
  
  //Conversion Low and High Bytes
  speedL = lo8(speed);
  speedH = hi8(speed);
  
  tempRPM = rpm*4;
  rpmL = lo8(tempRPM);
  rpmH = hi8(tempRPM);
  
  if (add_distance == true)
  { 
    distance_adder = speed * distance_multiplier;
    distance_counter += distance_adder;
    if (distance_counter > distance_adder) { distance_counter = 0; }
  }
  
  if ( oil_pressure_simulation == true)
  {
  //Set Oil Pressure Switch
    if (rpm > 1500) {
      digitalWrite(oilpswitch, LOW);
      }
    else {
      digitalWrite(oilpswitch, HIGH);
      }
  }
 
  /*Send CAN BUS Data*/
  
  //Immobilizer
  CanSend(0x3D0, 0, 0x80, 0, 0, 0, 0, 0, 0);
  
  //Engine on and ESP enabled
  CanSend(0xDA0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  
  //Enable Cruise Control
  //CanSend(0x289, 0x00, B00000001, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  
  //Dashboard Functions
  
  //Turning Light blinking
  if (turning_lights_blinking == true)
  {
    if (turning_lights_counter == 15)
    {
      temp_turning_lights = turning_lights;
      turning_lights_counter = turning_lights_counter + 1;
    }
    if (turning_lights_counter == 30)
    {
      temp_turning_lights = 0;
      turning_lights_counter = 0;
    }
    else
    {
      turning_lights_counter = turning_lights_counter + 1;
    }
  }
  else
  {
    temp_turning_lights = turning_lights;
  }
  
  //Check main settings
  
  //DPF
  if (dpf_warning == true) temp_dpf_warning = B00000010;
  else temp_dpf_warning = B00000000;
  
  //Seat Belt
  if (seat_belt == true) temp_seat_belt = B00000100;
  else temp_seat_belt = B00000000;
  
  //Battery Warning
  if (battery_warning == true) temp_battery_warning = B10000000;
  else temp_battery_warning = B00000000;
  
  //Trunk Lid (Kofferraumklappe)
  if (trunklid_open == true) temp_trunklid_open = B00100000;
  else temp_trunklid_open = B00000000;
  
  //Check Lamp Signal
  if (check_lamp == true) temp_check_lamp = B00010000;
  else temp_check_lamp = B00000000;
  
  //Clutch Text on LCD
  if (clutch_control == true) temp_clutch_control = B00000001;
  else temp_clutch_control = B00000000;
  
  //Warning for low key battery
  if (keybattery_warning == true) temp_keybattery_warning = B10000000;
  else temp_keybattery_warning = B00000000;
  
  //Lightmode Selection (Fog Light and/or High Beam)
  if (light_highbeam == true) temp_light_highbeam = B01000000;
  else temp_light_highbeam = B00000000;
  if (light_fog == true) temp_light_fog = B00100000;
  else temp_light_fog = B00000000;
  lightmode = temp_light_highbeam + temp_light_fog;
  
  //Engine Options (Water Temperature, Diesel Preheater)
  if (signal_dieselpreheat == true) temp_signal_dieselpreheat = B00000010;
  else temp_signal_dieselpreheat = B00000000;
  if (signal_watertemp == true) temp_signal_watertemp = B00010000;
  else temp_signal_watertemp = B00000000;
  engine_control = temp_signal_dieselpreheat + temp_signal_watertemp;
  
  //Drivemode Selection (ABS, Offroad, Low Tire Pressure, handbrake)
  if (signal_abs == true) temp_signal_abs = B0001;
  else temp_signal_abs = B0000;
  if (signal_offroad == true) temp_signal_offroad = B0010;
  else temp_signal_offroad = B0000;
  if (signal_handbrake == true) temp_signal_handbrake = B0100;
  else temp_signal_handbrake = B0000;
  if (signal_lowtirepressure == true) temp_signal_lowtirepressure = B1000;
  else temp_signal_lowtirepressure = B0000;
  
  drive_mode = temp_signal_abs + temp_signal_offroad + temp_signal_handbrake + temp_signal_lowtirepressure;
  
  
  //Send CAN data every 200ms
  pack_counter++;
  if (pack_counter == 20)
  {
    //Turning Lights 2
    CanSend(0x470, temp_battery_warning + temp_turning_lights, temp_trunklid_open + door_open, backlight, 0x00, temp_check_lamp + temp_clutch_control, temp_keybattery_warning, 0x00, lightmode);
    
    //Diesel engine
    CanSend(0x480, 0x00, engine_control, 0x00, 0x00, 0x00, temp_dpf_warning, 0x00, 0x00);
    
    ///Engine
    //CanSend(0x388, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    
    
    //Cruise Control
    //CanSend(0x289, 0x00, B00000101, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    
    pack_counter = 0;
  }
  
  //Motorspeed
  CanSend(0x320, 0x00, (speedL * 100), (speedH * 100), 0x00, 0x00, 0x00, 0x00, 0x00);
  
  //RPM
  CanSend(0x280, 0x49, 0x0E, rpmL, rpmH, 0x0E, 0x00, 0x1B, 0x0E);
    
  //Speed
  CanSend(0x5A0, 0xFF, speedL, speedH, drive_mode, 0x00, lo8(distance_counter), hi8(distance_counter), 0xad);
    
  //ABS
  CanSend(0x1A0, 0x18, speedL, speedH, 0x00, 0xfe, 0xfe, 0x00, 0xff);
  
  //Airbag
  CanSend(0x050, 0x00, 0x80, temp_seat_belt, 0x00, 0x00, 0x00, 0x00, 0x00);
    
  //Wait a time
  delay(20);
}

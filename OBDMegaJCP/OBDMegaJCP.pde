/*
 Arduino Mega + ELM327 communication
 ELM327 Pin18 > Mega Pin19 (RxD119)
 ELM327 Pin19 > Mega Pin18 (TxD118)
 
 created 30 Dec. 2010
 by Jack Cop
 
 */

/*
static int reset_chip_proc(int msg, DIALOG *d, int c);
static int options_proc(int msg, DIALOG *d, int c);
static int page_flipper_proc(int msg, DIALOG *d, int c);
static int sensor_proc(int msg, DIALOG *d, int c);
static int toggle_proc(int msg, DIALOG *d, int c);
static int toggle_all_proc(int msg, DIALOG *d, int c);
static int status_proc(int msg, DIALOG *d, int c);
static int page_number_proc(int msg, DIALOG *d, int c);
static int inst_refresh_rate_proc(int msg, DIALOG *d, int c);
static int avg_refresh_rate_proc(int msg, DIALOG *d, int c);
static int page_updn_handler_proc(int msg, DIALOG *d, int c);
*/

#define STRLEN  40
#define NUL     '\0'
#define CR      '\r'  // carriage return = 0x0d = 13
#define PROMPT  '>'
#define DATA    1     // data with no cr/prompt

char str[STRLEN];

typedef struct
{
   void (*formula)(int raw_data, char *buf);
   char label[32];
   char screen_buf[64];
   char pid[3];
   int enabled;
   int bytes; // number of data bytes expected from vehicle
} SENSOR;

static void load_sensor_states();
static void save_sensor_states();
static void fill_sensors(int page_number);


/* Sensor formules:
static void engine_rpm_formula(int data, char *buf);
static void vehicle_speed_formula(int data, char *buf);
static void engine_load_formula(int data, char *buf);
static void coolant_temp_formula(int data, char *buf);
*/

// variabelen
//static int device_connected = FALSE;
//static int reset_hardware = FALSE;
static int num_of_sensors = 0;
static int num_of_disabled_sensors = 0;
static int sensors_on_page = 0;
static int current_page = 0;

static float inst_refresh_rate = -1; // instantaneous refresh rate
static float avg_refresh_rate = -1;  // average refresh rate

volatile int refresh_time; // time between sensor updates

static SENSOR sensors[] =
{
   // formula                        // label            //screen_buffer  //pid  //enabled // bytes
   { engine_rpm_formula,            "Engine RPM:",                     "", "0C",      1,    2 },
   { vehicle_speed_formula,         "Vehicle Speed:",                  "", "0D",      1,    1 },
   { engine_load_formula,           "Calculated Load Value:",          "", "04",      1,    1 },
   { coolant_temp_formula,          "Coolant Temperature:",            "", "05",      1,    1 },
   { NULL,                          "",                                "", "",        0,    0 }
};



void engine_rpm_formula(int data, char *buf)
{
  sprintf(buf, "%i r/min", (int)((float)data/4));
  Serial1.print(buf);
}

void vehicle_speed_formula(int data, char *buf)
{
  sprintf(buf, "%i km/h", data);
}


void engine_load_formula(int data, char *buf)
{
   sprintf(buf, "%.1f%%", (float)data*100/255);
}

void coolant_temp_formula(int data, char *buf)
{
  sprintf(buf, "%i%c C", data-40, 0xB0);
}


void elm_init()
{
  char string[STRLEN];
    
  Serial1.print("ATE0\r\n");
  itoa(Serial1.read(),string,16);
  Serial.print(string);
  delay(500);
  Serial1.print("ATWS\r\n");
  itoa(Serial1.read(),string,16);
  delay(500);
  Serial.print(string); 
  Serial.print("\n\n...init end...");
  delay(1000);
}

/*
int elm_command(char *str, char *cmd)
{
  //char str[STRLEN];
  
  Serial1.print(str); //elm_write(str);
  return elm_read(); //elm_read
}

/*  
void elm_write(char *str)
{
  while(*str!=NUL)
    OBD2.print(*str++);
}
*/

/*

byte elm_read(char *str, byte size)
{
  int b;
  byte i=0;

  // wait for something on com port
  while(Serial1.read()!=PROMPT && i<size)
  {
    if(b!=-1 && b>=' ')
      str[i++]=b;
  }
}
*/

/*
  if(i!=size)  // we got a prompt
  {
    str[i]=0;  // replace CR by NUL
    return ">";
  }
  else
    return DATA;
}
*/


/*
void elm_init()
{
  char str[STRLEN];

  

  // send 01 00 until we are connected
  do
  {
    elm_command(str, PSTR("0100\r"));
    delay(1000);
  }
  while(elm_check_response("0100", str)!=0);
  // ask protocol
  elm_command(str, PSTR("ATDPN\r"));
  // str[0] should be 'A' for automatic
  // set header to talk directly to ECU#1
  if(str[1]=='1')  // PWM
    elm_command(str, PSTR("ATSHE410F1\r"));
  else if(str[1]=='2')  // VPW
    elm_command(str, PSTR("ATSHA810F1\r"));
  else if(str[1]=='3')  // ISO 9141
    elm_command(str, PSTR("ATSH6810F1\r"));
  else if(str[1]=='6')  // CAN 11 bits
    elm_command(str, PSTR("ATSH7E0\r"));
  else if(str[1]=='7')  // CAN 29 bits
    elm_command(str, PSTR("ATSHDA10F1\r"));
#endif
}

*/

void setup() {
  // initialize both serial ports:
  char str[10];
  byte temp;
  int i=0;
  
  str[0]=0;
  Serial.begin(9600);
  Serial1.begin(9600);
 
  //obd_commando("ATZ\r\n");
  obd_commando("ATE1\r\n");
  
  //obd_commando("ATDPN\r");
  //obd_commando("ATSHE410F1\r");
  //obd_commando("ATSHA810F1\r");
  //obd_commando("ATSH6810F1\r");
  //obd_commando("ATSH7E0\r");
  //obd_commando("ATSHDA10F1\r");
 
 /* 
  Serial1.write("ATZ\r\n");
  do
  {
    delay(250);
    temp=Serial1.read();
    //Serial.print(temp);
    str[i]=temp;
    i++;
   }while (temp!='.');
  Serial.print(str+5);
//  Serial.print(str[0]);
//  Serial.print(str[1]);
//  Serial.print(str[2]);
*/
  delay(2000);
}


void obd_commando(char *str)
{
//  char str[10];
  byte temp;
  int i=0;
  
  Serial1.write(str);
  do
  {
    delay(250);
    temp=Serial1.read();
    //Serial.print(temp);
    str[i]=temp;
    i++;
   }while (temp!=' ');
  Serial.print(str+5);
}



void loop()
{

 
  char string[STRLEN];
  byte reading;
  int i;
 /*
 char reading;
 
 itoa(elm_command(str, "ATZ\r"), str, 10);
 Serial.print(str);
 delay(500);
// Serial.read();
// elm_command(str, "ATWS\r"));
// delay(500);
}
 */
 
 
 //Serial1.print("ATZ\r\n");
 //itoa(Serial1.read(),string,16);
 //reading=Serial1.read();
 
 //while(i<5) // <2bytes
 // { 
  //  Serial1.read();
  //  string[i++];
  //  Serial.print(string);
 //Serial.print("\r\n");
//  }
    
 // Serial1.print("ATE1\r\n");
 //itoa(Serial1.read(),string,16);
 //delay(500); 
 //Serial.print(string);
 //Serial.print(Serial1.read(),BYTE);
// Serial.print("\r\n");
// delay(500);
}

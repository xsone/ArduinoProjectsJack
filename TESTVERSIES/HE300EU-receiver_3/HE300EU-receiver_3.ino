#ifdef PLUGIN_BUILD_DEV

//#######################################################################################################
//#################################### Plugin 199: RF KaKu receiver/sender ##############################
//#######################################################################################################

// Purpose: Control Klik-Aan-Klik-Uit RF 433MHz devices directly from ESP Easy (receive and send)
// Status : "Proof of concept"

// Connect the RF Receiver data pin to the first pin selected in the webgui
// Connect the RF Transmitter data pin to the second pin selected in the webgui

// Events:
//   newKaku_<address>#<Channel>=<state> (0=off, 1-15=dimvalue, 16=on)
//   Kaku_<address>#<Channel>=<state> (0=off, 1=on)
//   HE300EU_<address>#<Channel>=<state> (0=off, 1=on)

// Commands:
//   newKakuSend <address>, <Channel>, <state/dim>
//   KakuSend <address>, <Channel>, <state>

// This is a Work in Progress mini project!
// It has limited use because in most cases, your fancy Home Automation controller can handle 433MHz devices quite well using RFLink.
// It was implemented because in some cases i would like to have local "Klik-Aan-Klik-Uit" support using a standalone ESP Easy.
//   (Just because i own quite a lot of these Kaku devices)

// Current state / limitations:
//  Implemented send and receive support for KaKu with automatic code (no code wheel)
//  Implemented send and receive support for old KaKu unit's with code wheels
//  Implemented receive support for HomeEasy HE300EU remotes
//  RF Sender and RF receiver each need their own antenna! (as opposed to using a transceiver)

#define MIN_PULSE_LENGTH           100  // Too short pulses are considered to be noise...
#define SIGNAL_TIMEOUT               5  // gap between transmissions
#define MIN_RAW_PULSES              32  // Minimum number of pulses to be received, otherwise considered to be noise...
#define RAW_BUFFER_SIZE            256
#define RAWSIGNAL_MULTIPLY          25

void RF_ISR() ICACHE_RAM_ATTR;

// We need our own rawsignal buffer here.
// During plugin rawsignal checks, the IRQ routine will alPlugin_199_ready be working on the next signal burst...
volatile byte Plugin_199_RFBuffer[RAW_BUFFER_SIZE];
volatile boolean Plugin_199_ready = false;
volatile byte Plugin_199_pulses[RAW_BUFFER_SIZE + 2];
volatile int Plugin_199_number;
unsigned long Plugin_199_codeHash;
unsigned long Plugin_199_lastTime;
int8_t Plugin_199_RXpin = -1;
int8_t Plugin_199_TXpin = -1;

#define PLUGIN_199
#define PLUGIN_ID_199        199
#define PLUGIN_NAME_199       "RF Receiver/Sender [DEVELOPMENT]"
#define PLUGIN_VALUENAME1_199 "Address"
#define PLUGIN_VALUENAME2_199 "Channel"
#define PLUGIN_VALUENAME3_199 "State"

boolean Plugin_199(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_199;
        Device[deviceCount].Type = DEVICE_TYPE_DUAL;
        Device[deviceCount].VType = SENSOR_TYPE_DUAL;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].ValueCount = 3;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_199);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_199));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_199));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_199));
        break;
      }

    case PLUGIN_INIT:
      {
        if (Settings.TaskDevicePin1[event->TaskIndex] != -1)
        {
          Plugin_199_RXpin = Settings.TaskDevicePin1[event->TaskIndex];
          pinMode(Plugin_199_RXpin, INPUT_PULLUP);
          attachInterrupt(Plugin_199_RXpin, RF_ISR, CHANGE);
          success = true;
        }
        if (Settings.TaskDevicePin1[event->TaskIndex] != -1)
        {
          Plugin_199_TXpin = Settings.TaskDevicePin2[event->TaskIndex];
          pinMode(Plugin_199_TXpin, OUTPUT);
        }
        break;
      }

    case PLUGIN_TEN_PER_SECOND:
      {
        if (Plugin_199_ready)
        {
          if (decodeNewKaku(event->BaseVarIndex));
          else if (decodeKaku());
          else if (decodeHE300EU());
          Plugin_199_ready = false;
        }
        break;
      }

    case PLUGIN_WRITE:
      {
        String command = parseString(string, 1);
        if (command == F("newkakusend"))
        {
          success = true;
          sendNewKaku(event->Par1, event->Par2, event->Par3);
        }
        if (command == F("kakusend"))
        {
          success = true;
          sendKaku(event->Par1, event->Par2, event->Par3);
        }
        break;
      }

  }
  return success;
}


//********************************************************************************
//  Interrupt handler for RF messages
//********************************************************************************
void RF_ISR()
{
  static unsigned int counter = 0;
  static unsigned long TimeStamp = 0;
  unsigned long TimeElapsed = 0;

  TimeElapsed = micros() - TimeStamp;
  TimeStamp = micros();

  if (TimeElapsed > MIN_PULSE_LENGTH && counter < RAW_BUFFER_SIZE)
  {
    counter++;
    Plugin_199_RFBuffer[counter] = TimeElapsed / 25;
  }
  else
    counter = 0;

  if (TimeElapsed > (SIGNAL_TIMEOUT * 1000) )
  {
    if (counter > MIN_RAW_PULSES)
    {
      Plugin_199_number = counter;

      // copy IRQ RF working buffer to RawSignal struct
      for (unsigned int x = 0; x <= counter; x++)
        Plugin_199_pulses[x] = Plugin_199_RFBuffer[x];

      Plugin_199_ready = true;
      counter = 0;
    }
    else
      counter = 0;
  }

  if (counter >= RAW_BUFFER_SIZE)
    counter = 0;
}

//********************************************************************************
//  Decode HomeEasy 3xx series EU protocol (without code wheel)
//********************************************************************************
boolean decodeHE300EU()
{
  boolean success = false;
  byte Par1 = 0;
  unsigned long Par2 = 0;

  unsigned long address = 0;
  unsigned long bitstream = 0;
  int counter = 0;
  byte rfbit = 0;
  byte state = 0;
  unsigned long channel = 0;

  // valid messages are 116 pulses
  if (Plugin_199_number != 116) return false;

  for (byte x = 1; x <= Plugin_199_number; x = x + 2)
  {
    if ((Plugin_199_pulses[x] * 25 < 500) & (Plugin_199_pulses[x + 1] * 25 > 500))
      rfbit = 1;
    else
      rfbit = 0;

    if ((x >= 23) && (x <= 86)) address = (address << 1) | rfbit;
    if ((x >= 87) && (x <= 114)) bitstream = (bitstream << 1) | rfbit;

  }
  state = ((bitstream >> 8) & 0x3) - 1;
  channel = (bitstream) & 0x3f;

  Par1 = state;
  Par2 = address + channel;

  // valid signal, remember timestamp to suppress repeats...
  elapsed = millis() - Plugin_199_lastTime;
  Plugin_199_lastTime = millis();
  unsigned long codeHash = Par2 + Par1;
  if (codeHash != Plugin_199_codeHash || (codeHash == Plugin_199_codeHash && elapsed > 250))
  {
    String eventString = F("HE300EU_");
    eventString += address;
    eventString += F("#");
    eventString += channel;
    eventString += F("=");
    eventString += Par1;
    rulesProcessing(eventString);
    success = true;
  }
  Plugin_199_codeHash = Par2 + Par1;
}


//********************************************************************************
//  Decode generic protocol, deriving hash value
//********************************************************************************
boolean decodeUnknown()
{
  boolean success = false;
  byte Par1 = 0;
  unsigned long Par2 = 0;

  int x;
  unsigned int MinPulse = 0xffff;
  unsigned int MinSpace = 0xffff;
  unsigned long CodeM = 0L;
  unsigned long CodeS = 0L;

  if (Plugin_199_number < MIN_RAW_PULSES) return false;

  for (x = 5; x < Plugin_199_number - 2; x += 2)
  {
    if (Plugin_199_pulses[x]  < MinPulse)MinPulse = Plugin_199_pulses[x];
    if (Plugin_199_pulses[x + 1] < MinSpace)MinSpace = Plugin_199_pulses[x + 1];
  }

  MinPulse += (MinPulse * 100) / 100;
  MinSpace += (MinSpace * 100) / 100;

  // Data kan zowel in de mark als de space zitten. Daarom pakken we beide voor data opbouw.
  for (x = 3; x <= Plugin_199_number; x += 2)
  {
    CodeM = (CodeM << 1) | (Plugin_199_pulses[x]   > MinPulse);
    CodeS = (CodeS << 1) | (Plugin_199_pulses[x + 1] > MinSpace);
  }

  // Data kan zowel in de mark als de space zitten. We nemen de grootste waarde voor de data.
  if (CodeM > CodeS)
    Par2 = CodeM;
  else
    Par2 = CodeS;

  // valid signal, remember timestamp to suppress repeats...
  elapsed = millis() - Plugin_199_lastTime;
  Plugin_199_lastTime = millis();
  unsigned long codeHash = Par2;
  if (codeHash != Plugin_199_codeHash || (codeHash == Plugin_199_codeHash && elapsed > 250))
  {
    String eventString = F("UnknownRF_");
    eventString += Par2;
    rulesProcessing(eventString);
    success = true;
  }
  Plugin_199_codeHash = Par2;
}

#endif

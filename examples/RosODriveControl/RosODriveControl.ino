
#include "commands.h"
#include "ODriveUART.h"
#include <SoftwareSerial.h> 
//#include <experimental/filesystem>

// Arduino without spare serial ports (such as Arduino UNO) have to use software serial.
// Note that this is implemented poorly and can lead to wrong data sent or read.
// pin 8: RX - connect to ODrive TX
// pin 9: TX - connect to ODrive RX
SoftwareSerial odrive_serial(8, 9);
unsigned long arduino_odrive_baudrate = 19200;  // Must match what you configure on the ODrive (see docs for details)

// Teensy 3 and 4 (all versions) - Serial1
// pin 0: RX - connect to ODrive TX
// pin 1: TX - connect to ODrive RX
// See https://www.pjrc.com/teensy/td_uart.html for other options on Teensy
// HardwareSerial& odrive_serial = Serial1;
// int baudrate = 115200; // Must match what you configure on the ODrive (see docs for details)

/* --------- VARIABLE INITIALISATION --------- */ 
const long COMP_ARDUINO_BAUDRATE = 115200; // baud rate between arduino and computer 

// pair of variables to help parse serial commands 
int arg = 0;
int index = 0;

// variable to hold an input character 
char chr; 

// variable for commmand 
char cmd; 

// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];

// The arguments converted to integers
long arg1;
long arg2;

// odrive handler 
ODriveUART odrive(odrive_serial);

void handleCharacter(char chr)
{
  if (arg == 0)
  {
    cmd = chr; // Command identifier (single-letter)
  }
  else if (arg == 1)
  {
    argv1[index++] = chr; // Append to the first argument
  }
  else if (arg == 2)
  {
    argv2[index++] = chr; // Append to the second argument
  }
}

void handleSpace()
{
  if (arg == 0)
  {
    arg = 1; // First space: Switch to first argument
  }
  else if (arg == 1)
  {
    argv1[index] = NULL; // Null-terminate the first argument
    arg = 2;             // Switch to second argument
    index = 0;           // Reset index for the second argument
  }
}

void terminateCommand()
{ 
//  Serial.println("Full command read, running."); 
  if (arg == 1)
  {
    argv1[index] = NULL; 
  }
  else if (arg == 2)
  {
    argv2[index] = NULL;
  }

  runCommand();   // Execute the parsed command
  resetCommand(); // Reset state for the next command
}

void processCharacter(char chr)
{
  if (chr == '\r') // End of command
  {
    terminateCommand(); 
  }
  else if (chr == ' ') // Argument delimiter
  {
    handleSpace();
  }
  else // Regular character
  {
    handleCharacter(chr);
  }
}


/* Clear the current command parameters */
void resetCommand() 
{
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  index = 0;
}

/* execute the command. Commands are defined in commands.h */
int runCommand() 
{
  int i = 0; 
  // char* p = argv1;
  // char* str;
  // int pid_args[4];
  arg1 = atof(argv1);
  arg2 = atof(argv2);

  Serial.print("Running command: "); 
  Serial.println( cmd ); 
  switch ( cmd ) 
  {
    case GET_BAUDRATE:
      Serial.println(COMP_ARDUINO_BAUDRATE);
      break;

    case READ_MOTOR_STATES:
    {
      Serial.println("reading motor states");

      // get motor feedback 
      ODriveFeedback feedback = odrive.getFeedback();
      
      Serial.print(feedback.pos); // print position  
      Serial.print(" "); 
      Serial.println(feedback.vel); // print veocity 

        // Serial.print("0.0"); 
        // Serial.print(" "); 
        // Serial.println("0.0"); 
      break; 
    }

    case MOTOR_SPEEDS: 
    {
      // TODO 
      Serial.println("Setting motor speeds");

      odrive.setPosition( arg1 ); // only one motor for now     
      Serial.println("DONE");     
      break; 
    }  
}
}

void setup()
{   
  odrive_serial.begin(arduino_odrive_baudrate);

  Serial.begin(COMP_ARDUINO_BAUDRATE); // Serial to PC

  delay(10);

  Serial.println("Waiting for ODrive...");
//  TODO: UNCOMMENTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
  while (odrive.getState() == AXIS_STATE_UNDEFINED)
  {
   delay(100);
  }

  Serial.println("found ODrive");

  Serial.print("DC voltage: ");
  Serial.println(odrive.getParameterAsFloat("vbus_voltage"));

  Serial.println("Enabling closed loop control...");
//  TODO: UNCOMMENTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
   while (odrive.getState() != AXIS_STATE_CLOSED_LOOP_CONTROL)
   {
     odrive.clearErrors();
     odrive.setState(AXIS_STATE_CLOSED_LOOP_CONTROL);
     delay(10);
   }

  Serial.println("ODrive running!");
}

/* Enter the main loop */
void loop() 
{
  // Continuously check and process incoming characters from the serial port
  while (Serial.available() > 0)
  {
    chr = Serial.read();
    processCharacter(chr);
  }
}

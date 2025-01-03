
#include <commands.h> 
#include <ODriveUART.h>
#include <SoftwareSerial.h>
#include <experimental/filesystem>

// Arduino without spare serial ports (such as Arduino UNO) have to use software serial.
// Note that this is implemented poorly and can lead to wrong data sent or read.
// pin 8: RX - connect to ODrive TX
// pin 9: TX - connect to ODrive RX
SoftwareSerial odrive_serial(8, 9);
unsigned long baudrate = 19200;  // Must match what you configure on the ODrive (see docs for details)

// Teensy 3 and 4 (all versions) - Serial1
// pin 0: RX - connect to ODrive TX
// pin 1: TX - connect to ODrive RX
// See https://www.pjrc.com/teensy/td_uart.html for other options on Teensy
// HardwareSerial& odrive_serial = Serial1;
// int baudrate = 115200; // Must match what you configure on the ODrive (see docs for details)

/* --------- VARIABLE INITIALISATION --------- */ 
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
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);

  switch ( cmd ) 
  {
    case GET_BAUDRATE:
      Serial.println(BAUDRATE);
      break;

    case ANALOG_READ:
      Serial.println(analogRead(arg1));
      break;

    case DIGITAL_READ:
      Serial.println(digitalRead(arg1));
      break;

    case READ_MOTOR_STATES; 
      // get motor feedback 
      ODriveFeedback feedback = odrive.getFeedback();
      
      Serial.print(odrive.pos); // print position  
      Serial.print(" "); 
      Serial.println(odrive.vel); // print veocity 

      break; 

    case MOTOR_SPEEDS: 
      // TODO 
      odrive.setVelocity( arg1 ); // only one motor for now 
      break; 
  }
}

void setup()
{   
  odrive_serial.begin(baudrate);

  Serial.begin(115200); // Serial to PC

  delay(10);

  Serial.println("Waiting for ODrive...");
  while (odrive.getState() == AXIS_STATE_UNDEFINED)
  {
    delay(100);
  }

  Serial.println("found ODrive");

  Serial.print("DC voltage: ");
  Serial.println(odrive.getParameterAsFloat("vbus_voltage"));

  Serial.println("Enabling closed loop control...");
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

  // // continuously check and process incoming characters from serial port 
  // // Serial.available returns the number of characters left 
  // while ( Serial.available() > 0 ) 
  // {
  //   // read the next 
  //   chr = Serial.read(); 

  //   // terminate a command with a carriage return 
  //   if ( chr == '\r' ) 
  //   {
  //     if (arg == 1) 
  //     {
  //       argv1[index] = NULL; 
  //     }
  //     else if (arg == 2) 
  //     {
  //       argv2[index] = NULL;
  //     }

  //     runCommand(); 
  //     resetCommand(); 

  //     continue; 
  //   }
  //   else if ( chr == ' ' )
  //   {
  //     // step through the arguments 
  //     if (arg == 0) 
  //     {
  //       arg = 1 // First space: Switch to first argument.
  //     } 
  //     else if (arg == 1) 
  //     { 
  //       argv1[index] = NULL; // Null-terminate the first argument.
  //       arg = 2;             // Switch to second argument.
  //       index = 0;           // Reset index for the second argument.
  //     }
  //     continue; // Move to the next character.
  //   }
  //   else 
  //   {
  //     if (arg == 0) cmd = chr;                 // Command identifier (single-letter).
  //     else if (arg == 1) argv1[index++] = chr; // First argument.
  //     else if (arg == 2) argv2[index++] = chr; // Second argument.
  //   }
  // }
}

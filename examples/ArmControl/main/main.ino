
/*
TO DO:
- Improve debuggability/ error handling,
- Tune the motors,
- Reset all of the motors when the reset command is received. 
- Add servo and stepper control support. 
*/ 

#include "commands.h"
#include "ODriveUART.h"
#include "ArmController.h" 
#include "Motors.h" 
#include "SerialHandler.h" 
#include <SoftwareSerial.h> 
#include <HardwareSerial.h>

// start the serial handler 
SerialHandler mySerialHandler; 

void setup() 
{
  Serial.begin(115200); 
}

void loop()
{
  mySerialHandler.readCommand(); 
  Serial.flush(); 
}

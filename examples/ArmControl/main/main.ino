
/*
TO DO:
- Improve debuggability/ error handling,
- Tune the motors,
- Reset all of the motors when the reset command is received. 
- Add servo and stepper control support. 

*/ 

// i odrive reduction_ratio tx1 rx1 baud1 \r\n (ratio = 1 whilst developing, another might have 16)
// i servo reduction_ratio pin \r\n (ratio = 1)

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

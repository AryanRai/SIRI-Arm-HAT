
#include "ODriveUART.h"
#include "Runnable.h" 
#include "SerialHandler.h"
#include "ArmController.h"
#include "CommandStore.h" 
#include <SoftwareSerial.h> 
#include <HardwareSerial.h>
#include <TeensyThreads.h>

// TODOOOOOOOOOOO
  // complete definition of command store 
  // implement actual motor controller code as another "Runnable" 

// #################################################### // 
// ################### MAIN PROGRAM ################### // 
// #################################################### // 

// Serial Handler object 
CommandStore* cmd_store; 
SerialHandler* serial_handler; 
ArmController* arm_controller; 

// start the serial handler 
void setup() 
{
  Serial.begin(115200); 
  delay(1000);  // Give time for Serial to open
  Serial.println("=== SETUP STARTED ===");

  // make a command store 
  cmd_store = new CommandStore(); 

  // start the serial handler 
  serial_handler = new SerialHandler(cmd_store); 
  serial_handler->startReading(); 

  // start the arm controller 
  arm_controller = new ArmController(cmd_store); 
  arm_controller->order66(); 
}

void loop() { 
  Serial.flush(); 
  delay(1); 
} 




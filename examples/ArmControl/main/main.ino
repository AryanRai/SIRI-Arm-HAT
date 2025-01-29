
#include "commands.h"
#include "ODriveUART.h"
#include "ArmController.h" 
#include "Motors.h" 
#include "SerialHandler.h" 
#include <SoftwareSerial.h> 

// start the serial handler 
SerialHandler mySerialHandler; 

void setup() 
{
  Serial.begin(115200); 
//  Serial.println("Serial successfully initialised"); 
//  Serial.flush(); 
}

void loop()
{
  mySerialHandler.readCommand(); 
  Serial.flush(); 
}

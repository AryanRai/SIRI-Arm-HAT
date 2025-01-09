
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
  Serial.println("Setting up");  
  Serial.println("..."); 
  Serial.println("Set up"); 
}

void loop()
{
  mySerialHandler.readCommand(); 
  Serial.flush(); 
  delay(100); // delay 0.1 seconds  
}

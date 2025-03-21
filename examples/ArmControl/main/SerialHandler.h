
#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include "commands.h"
#include "ArmController.h"
#include <HardwareSerial.h>
#include <avr/wdt.h> // Include for watchdog control


class SerialHandler 
{ 
  public: 
    SerialHandler();  // constructor  
    ~SerialHandler(); // destructor 

    // reads from the serial until a carriage return is received 
    void readCommand(); 

  private: 
    // --- CONSTANTS --- // 
    static const char ETX = 0x03; // end of text 
  
    // --- METHODS --- // 
    void processCharacter(); 
    void handleCommandEnd(); 
    void handleCharacter(); 
    void storeArgument(); 
    void resetArgumentArray(); 
    void runCommand(); 
    void argsToDoubles( double* cmd_vels ) const; 
    void resetArduino(); 
    void readMotorStates(); 
    void setMotorVels(); 
    void setMotorPos(); 

    // --- DATA --- // 
    // handy variables to help read from serial 
    int arg_, index_; 
    char chr_, cmd_; 
    bool new_word_; 

    // command/argument array variables 
    char argument_arr_[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE]; // array of the argument strings 
    char cmd_buffer_[CMD_STRING_BUFFERSIZE]; // buffer for the current command being read 
    uint8_t cmd_index_, arg_index_; 

    // arm movement handler 
    bool arm_init_; 
    ArmController arm_controller_; 
}; 

#endif 


#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include "commands.h"
#include "ArmController.h"


class SerialHandler 
{ 
  public: 
    SerialHandler();  // constructor  
    ~SerialHandler(); // destructor 

    // reads from the serial until a carriage return is received 
    void readCommand(); 

  private: 
    // --- STATIC CONSTANTS --- // 
    static const int CMD_STRING_BUFFERSIZE = 10; 
    static const int ARG_ARRAY_BUFFERSIZE = 50; 
  
    // --- METHODS --- // 
    void processCharacter(); 
    void handleCommandEnd(); 
    void handleCharacter(); 
    void storeArgument(); 
    void resetArgumentArray(); 
    void runCommand(); 
    void argsToDoubles( double* cmd_vels ) const; 

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

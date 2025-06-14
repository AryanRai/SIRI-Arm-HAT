
#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

#include "Runnable.h"
#include "CommandStore.h" 
#include "Constants.h" 
#include <TeensyThreads.h>
#include <SoftwareSerial.h> 
#include <Arduino.h>

class SerialHandler : Runnable 
{
  private: 
    // Thread object 
    std::thread *serial_thread_;

    // --- CONSTANTS --- // 
    static const char ETX = 0x03; // end of text 
  
    // --- METHODS --- // 
    void processCharacter(); 
    void handleCommandEnd(); 
    void handleCharacter(); 
    void storeArgument(); 
    void resetArgumentArray(); 
    void storeCommand(); 

    // --- DATA --- // 
    // handy variables to help read from serial 
    int arg_, index_; 
    char chr_, cmd_; 
    bool new_word_; 
    CommandStore* cmd_store_; 

    // command/argument array variables 
    char argument_arr_[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE]; // array of the argument strings 
    char cmd_buffer_[CMD_STRING_BUFFERSIZE]; // buffer for the current command being read 
    uint8_t cmd_index_, arg_index_; 

  protected: 
    // Runnable function that we need to implement
    void runTarget(void *arg); 

  public: 
    // Constructor/ Destructor
    SerialHandler( CommandStore* cmd_store ); 
    ~SerialHandler(); // destructor 

    // reads from the serial until a carriage return is received 
    void readCommand(); 

    // Start thread that will begin reading serial commands 
    void startReading(); 

};

#endif
 
#ifndef COMMAND_PACKAGE_H
#define COMMAND_PACKAGE_H

#include "Constants.h"
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <utility>
#include <Arduino.h>

// A package that stores all the relevant info of a command 
const int MOTOR_NAME_BUFFER = 10; // servo, stepper or odrive 
class MotorConfig
{ 
  public: 
    MotorConfig(); 

    char type_[MOTOR_NAME_BUFFER]; 
    float red_ratio_; 
    long int baud_; 
    HardwareSerial* serial_; 
}; 


class CommandPackage 
{ 
  public: 
    CommandPackage(); 
    void populate( char argument_arr_[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], int arg_num ); 
    void getCmdType( char& cmd ); 
    void getCmdData( double cmd_data[ARG_ARRAY_BUFFERSIZE], int& cmd_num ); 
    void getMotorList( MotorConfig motor_list[ARG_ARRAY_BUFFERSIZE], int& motor_num ); 
    void getNumMotors( int& motor_num ); 

    // Move operators 
    CommandPackage(CommandPackage&& other) noexcept; 
    CommandPackage& operator=(CommandPackage&& other) noexcept; 

    enum Cmds 
    {
      READ_MOTOR_STATES = 'e',// command: "e\r\n"
      MOTOR_SPEEDS = 'v',     // command: "v v1 v2 ... vN\r\n" in m/s
      MOTOR_POSITIONS = 'p',  // command: "p p1 p2 ... pN\r\n" in radians
      INIT_ARM = 'i',         // arduino command: "i type1 red_ratio1 tx1 rx1 baud1 type2 red_ratio2 tx2 rx2 baud2 ... typeN red_ratioN txN rxN baudN\r\n"
                              // teensy command example: "i type1 red_ratio1 baud1 Serial_n
                              // teensy command example: "i odrive 0.5 19200 Serial1    --> Serial1 is for big motor; Serial2 for small motor as of March 21
      RESET = 'r',            // command: "r\r\n"
      EMPTY_COMMAND = '\r' 
    }; 

  private: 
    // methods
    HardwareSerial* selectSerialPort(const char* port_name); 
    void manageInitCmd( char argument_arr_[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], int arg_num );  
    void manageMotorCmd( char argument_arr_[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], int arg_num );  
    
    char cmd_type_; // e, v, p, i etc 
    double cmd_data_[ARG_ARRAY_BUFFERSIZE]; // to store data for pos and vel cmds 
    int num_motors_; 
    MotorConfig motor_list_[ARG_ARRAY_BUFFERSIZE]; // to store data for motor initialisation 
}; 

#endif

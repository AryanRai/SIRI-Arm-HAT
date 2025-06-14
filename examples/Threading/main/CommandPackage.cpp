#include "CommandPackage.h"

// ####################################################### // 
// ##################### MOTOR CONFIG #################### // 
// ####################################################### // 
MotorConfig::MotorConfig()
  : red_ratio_(0), 
    baud_(0), 
    serial_(NULL) {}

// ####################################################### // 
// ################### COMMAND PACKAGE ################### // 
// ####################################################### // 
CommandPackage::CommandPackage()
: num_motors_(0)
{
  Serial.println("Command package initialised"); 
  Serial.flush(); 
} 

void CommandPackage::populate( char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], int arg_num )  
{
  Serial.println("Populating command package"); 
  for (int i=0; i < arg_num; ++i) 
  {
    Serial.print(argument_arr[i]); 
    Serial.print(" "); 
  }
  Serial.println(); 

  // Iterate over every argument 
  cmd_type_ = argument_arr[0][0]; // always the first argument 

  switch ( cmd_type_ ) 
  {
    case (INIT_ARM): 
      manageInitCmd(argument_arr, arg_num);  
      break; 

    case (READ_MOTOR_STATES): 
      // cmd is already set, no further info 
      Serial.println("Storing READ_MOTOR_STATES"); 
      break; 

    case (MOTOR_SPEEDS):
      manageMotorCmd(argument_arr, arg_num); 
      break; 

    case (MOTOR_POSITIONS):
      manageMotorCmd(argument_arr, arg_num); 
      break; 

    case (RESET):
      Serial.println("Storing RESET"); 
      // cmd is already set, no further info 
      break; 

    case (EMPTY_COMMAND):
      Serial.println("EMPTY_COMMAND RESET"); 
      // cmd is already set, no further info 
      break; 

    default: 
      Serial.println("Command not recognised"); 
      break; 
  }
}

void CommandPackage::manageInitCmd( char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], int arg_num ) 
{
  Serial.println("Storing Init Command");

  int args_per_motor = 4; 
  int num_motors_ = arg_num / args_per_motor; // integer divide to get number of motors 

  // Loop over and populate all motors 
  for ( uint8_t i = 1; i < num_motors_; i += 1 ) // 4 arguments per motor 
  {
    int j = args_per_motor*i; 
    strcpy( motor_list_[i].type_, argument_arr[j] ); 
    motor_list_[i].red_ratio_ = atof(argument_arr[j+1]); 
    motor_list_[i].baud_ = atol(argument_arr[j+2]); 

    // determine the correct serial port 
    char serial_port_name[CMD_STRING_BUFFERSIZE];
    strcpy(serial_port_name, argument_arr[j+3]); // get serial port name as string; e.g., Serial1. One ODrive per serial port. 
    motor_list_[i].serial_ = selectSerialPort(serial_port_name); // Select serial port based on input
  }
}

void CommandPackage::manageMotorCmd( char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], int arg_num ) 
{
  Serial.println("Storing MOTOR_SPEEDS"); 

  // Loop over and populate all motors 
  for ( uint8_t i = 1; i < num_motors_; i += 1 ) // 4 arguments per motor 
  {
    cmd_data_[i] = atol( argument_arr[i] ); 
  }
}

HardwareSerial* CommandPackage::selectSerialPort(const char* port_name)
{
  if (strcmp(port_name, "Serial1") == 0) 
  {
    return &Serial1;  // Return reference to Serial1
  } 
  else if (strcmp(port_name, "Serial2") == 0) 
  {
    return &Serial2;  // Return reference to Serial2
  } 
  else if (strcmp(port_name, "Serial3") == 0) 
  {
    return &Serial3;  // Return reference to Serial3
  }
  // Add more serial ports here as needed (e.g., Serial4, Serial5)
  
  // Serial.print("Don't recognise serial port name: ");
  // Serial.println(port_name); 
  return nullptr; // Return null if invalid serial port name
}

void CommandPackage::getCmdType( char& cmd )
{
    cmd = cmd_type_; 
} 

void CommandPackage::getCmdData( double cmd_data[ARG_ARRAY_BUFFERSIZE], int& cmd_num ) 
{
    cmd_num = num_motors_; 
    for (int i=0; i < num_motors_; ++i) 
    { 
        cmd_data[i] = cmd_data_[i]; 
    } 
} 

void CommandPackage::getMotorList( MotorConfig motor_list[ARG_ARRAY_BUFFERSIZE], int& motor_num ) 
{
    motor_num = num_motors_; 
    for (int i=0; i < num_motors_; ++i) 
    {
        motor_list[i] = motor_list_[i];
    } 
} 

void CommandPackage::getNumMotors( int& motor_num )
{
    motor_num = num_motors_; 
} 

CommandPackage::CommandPackage(CommandPackage&& other) noexcept {
    *this = std::move(other);
}



CommandPackage& CommandPackage::operator=(CommandPackage&& other) noexcept {
    // Move primitive types
    cmd_type_ = other.cmd_type_;
    num_motors_ = other.num_motors_;
    memcpy(cmd_data_, other.cmd_data_, sizeof(cmd_data_));
    memcpy(motor_list_, other.motor_list_, sizeof(motor_list_));    
    return *this;
}

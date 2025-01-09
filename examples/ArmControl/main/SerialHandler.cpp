
#include "SerialHandler.h"

// -------------------- SERIAL HANDLER -------------------- // 
SerialHandler::SerialHandler() 
  : new_word_(true), 
    cmd_index_(0), 
    arg_index_(0), 
    arm_init_(false), 
    arm_controller_(0) {} 

SerialHandler::~SerialHandler() 
{
  // deallocate the dynamically allocated memory 
}

void SerialHandler::readCommand() 
{
  resetArgumentArray(); 
  while ( Serial.available() > 0 ) 
  {
    if ( new_word_ ) 
    {
      cmd_index_ = 0; 
      memset(cmd_buffer_, 0, CMD_STRING_BUFFERSIZE); // Clear buffer 
      new_word_ = false;  
    }
    chr_ = Serial.read(); 
    processCharacter(); 
  }
}

void SerialHandler::resetArgumentArray()
{
//  memset( argument_arr_, 0, ARG_ARRAY_BUFFERSIZE ); 
  // iterate over the argument array and set to \0 
  for ( uint8_t i=0; i < arg_index_; ++i )
  {
    memset( argument_arr_[i], 0, CMD_STRING_BUFFERSIZE );  
  }
  arg_index_ = 0; 
  new_word_ = true; 
}

void SerialHandler::storeArgument()
{
  cmd_buffer_[cmd_index_] = '\0'; // null-terminate the string 
  strcpy( argument_arr_[arg_index_++], cmd_buffer_ ); // copy into command array 

  // prep for next iteration 
  new_word_ = true; // start new word 
} 

void SerialHandler::processCharacter() 
{
  if (chr_ == '\r') // End of command
  {
    handleCommandEnd(); 
  }
  else if (chr_ == ' ') // Argument end 
  {
    storeArgument(); 
  }
  else // Regular character
  {
    handleCharacter();
  }
}

void SerialHandler::handleCommandEnd()
{
  storeArgument(); 
  Serial.print( "Received command: " ); 
  for ( uint8_t i=0; i <= arg_index_; ++i ) 
  {
    Serial.print("|"); 
    Serial.print(argument_arr_[i]); 
    Serial.print("|"); 

    Serial.print(" "); 
  }
  Serial.println(); 
  
  runCommand(); 
  resetArgumentArray(); 
}

void SerialHandler::handleCharacter()
{
  if ( cmd_index_ < CMD_STRING_BUFFERSIZE - 1) // prevent buffer overflow 
  {
    cmd_buffer_[cmd_index_++] = chr_; 
  }
  else
  {
    // handle buffer overflow 
    Serial.println( "Command too long!" ); 
    new_word_ = true; 
  }
}

void SerialHandler::runCommand()
{
  char cmd = argument_arr_[0][0]; // always the first argument 
  switch ( cmd ) 
  {
    case INIT_ARM: 
    {
      // initialise the arm 
      arm_controller_.initArm( argument_arr_, arg_index_ ); 
      arm_init_ = true; 
      break; 
    }
      
    case READ_MOTOR_STATES:
    {
      // get info from the arm controller 
      uint8_t motor_num = arm_controller_.getNumMotors(); 
      double pos_arr[motor_num]; 
      double vel_arr[motor_num]; 

      // get the motor states 
      arm_controller_.getMotorStates( pos_arr, vel_arr ); 

      // iterate over all motors and get their state 
      bool first = true; 
      for ( uint8_t i=0; i<motor_num; ++i ) 
      {

        if (!first) { Serial.print(" "); }
        else { first = false; } 
        
        Serial.print(pos_arr[i]); 
        Serial.print(" "); 
        Serial.print(vel_arr[i]); 
      }
      Serial.println(); 
      
      break;
    }
    case MOTOR_SPEEDS: 
    {
      // TODO 
//      odrive.setVelocity( arg1 ); // only one motor for now     
//      Serial.println("DONE");     

      // make an array of doubles 
      double cmd_vels[arg_index_+1]; 
      argsToDoubles( cmd_vels ); 
      arm_controller_.setMotorVel( cmd_vels ); 
      break; 
    }
  }
}

void SerialHandler::argsToDoubles( double* cmd_vels ) const
{
  // iterate over all motors 
  uint8_t motor_num = arm_controller_.getNumMotors(); 
  
  for ( uint8_t i=1; i<=motor_num; ++i) 
  {
    Serial.print("Setting motor: "); 
    Serial.print(i); 
    Serial.print(" to velocity: "); 
    Serial.print(argument_arr_[i]); 
    Serial.println("rad/s"); 
    cmd_vels[i] = atof(argument_arr_[i]); 
  }
}

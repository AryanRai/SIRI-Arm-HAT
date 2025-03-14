
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

  // start new word 
  new_word_ = true; 
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
  
  runCommand(); 
  resetArgumentArray(); 
}

void SerialHandler::handleCharacter()
{
  if ( cmd_index_ < CMD_STRING_BUFFERSIZE - 1 ) // prevent buffer overflow 
  {
    cmd_buffer_[cmd_index_++] = chr_; 
  }
  else
  {
    // handle buffer overflow 
    Serial.println( "Error: Command too long!" ); 
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
      Serial.println("Initialise arm command received"); 
      arm_controller_.initArm( argument_arr_, arg_index_ ); 
      arm_init_ = true; 
      break; 
    }

    case RESET_ARDUINO: 
    {
      resetArduino(); 
      break; 
    }
    case READ_MOTOR_STATES:
    {
      readMotorStates(); 
      break;
    }
    case MOTOR_SPEEDS: 
    {
      setMotorVels(); 
      break; 
    }
    case MOTOR_POSITIONS:
    {
      setMotorPos(); 
      break; 
    }
    case EMPTY_COMMAND:
    {
      Serial.println("Empty command received"); 
      break; 
    }
    case PRINT_ERRORS:
    {
      arm_controller_.printMotorErrors(); 
      break; 
    }
    default:
    {
      Serial.print("Error: Command '"); 
      Serial.print(cmd); 
      Serial.println("' not recognised"); 
    }
  }
  Serial.println(ETX); // ETX (End of Text, 0x03) 
  Serial.flush(); 
}

void SerialHandler::argsToDoubles( double* cmd_vels ) const
{
  // iterate over all motors 
  uint8_t motor_num = arm_controller_.getNumMotors(); 
  
  for ( uint8_t i=0; i<motor_num; ++i) 
  {
    cmd_vels[i] = atof(argument_arr_[i+1]); 
  }
}

void SerialHandler::resetArduino()
{
   Serial.println("Reseting..."); 
   wdt_enable(WDTO_15MS); // Enable the watchdog with a 15ms timeout 
   while (true) { delay(1); } // Wait for the watchdog to trigger a reset
}

void SerialHandler::readMotorStates()
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
    
    char pos_str[12];
    char vel_str[12];
    
    dtostrf(pos_arr[i], 1, 8, pos_str);
    dtostrf(vel_arr[i], 1, 8, vel_str);

    Serial.print(pos_str); 
    Serial.print(" "); 
    Serial.print(vel_str); 
  }
  Serial.println(); 
}

void SerialHandler::setMotorVels()
{
  // make an array of doubles 
  uint8_t num_motors = arm_controller_.getNumMotors(); 
  double cmd_vels[num_motors]; 
  argsToDoubles( cmd_vels ); 

  // send to the arm controller 
  arm_controller_.setMotorVel( cmd_vels ); 

  // print to the serial for validation 
  Serial.print("Motors set to (rad/s): ");     
  for ( uint8_t i=0; i<num_motors; ++i )
  {
    Serial.print(cmd_vels[i]); 
    Serial.print(" "); 
  }
  Serial.println(); 
}

void SerialHandler::setMotorPos()
{
  // make an array of doubles 
  uint8_t num_motors = arm_controller_.getNumMotors(); 
  double cmd_pos[num_motors]; 
  argsToDoubles( cmd_pos ); 

  // send to the arm controller 
  arm_controller_.setMotorPos( cmd_pos ); 

  // print to the serial for validation 
  Serial.print("Motors set to (rad): "); 
  for ( uint8_t i=0; i<num_motors; ++i )
  {
    Serial.print(cmd_pos[i]); 
    Serial.print(" "); 
  }
  Serial.println(); 
}


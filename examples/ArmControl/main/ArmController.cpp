
#include "ArmController.h"

// -------------------- ARM CONTROLLER -------------------- // 
ArmController::ArmController( const int dofs ) 
  : motor_num_( dofs )
{
  memset( motors_, 0, MAX_MOTOR_NUM ); // clear the memory 
}

ArmController::~ArmController()
{
  // delete the dynamically allocated memory 
  for ( uint8_t i=0; i < motor_num_; i++ ) 
  {
    delete motors_[i]; 
  }
}

void ArmController::initArm( char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], const int arg_num ) 
{
  // iterate over arguments, ignoring the first 
  Serial.println("Initialising arm"); 
  for ( uint8_t i = 1; i < arg_num+1; i += 4 ) 
  { 
    // chunks of 4
    char motor_type[MOTOR_TYPE_BUFFERSIZE]; 
    strcpy( motor_type, argument_arr[i] ); // copy across 
    int pin1 = atoi(argument_arr[i+1]); 
    int pin2 = atoi(argument_arr[i+2]); 
    long baud = atol(argument_arr[i+3]); 
    addMotor( motor_type, pin1, pin2, baud ); 
  }
} 

void ArmController::addMotor( const char* type, const int pin1, const int pin2, const long baud ) 
{
  // check the type  
  if ( strcmp( "odrive", type ) == 0 ) 
  {
    // add an odrive motor class 
//    Serial.print("Adding ODrive Motor with type odrive, pin1: " );
//    Serial.print(pin1); 
//    Serial.print(", pin2: "); 
//    Serial.print(pin2); 
//    Serial.print(", baud: "); 
//    Serial.println(baud);   
    motors_[motor_num_++] = new OdriveMotor( baud, pin1, pin2 ); 
  }
  else if ( strcmp( "stepper", type ) == 0 ) 
  {
    // TODO 
  }
  else if ( strcmp( "servo", type ) == 0 )
  {
    // TODO 
  }
  else
  {
//    Serial.print( "Error: Motor type '" ); 
//    Serial.print( type ); 
//    Serial.println( "' not recognised" ); 
  }
} 
void ArmController::setMotorVel( const double* cmd_vels ) 
{
  for ( uint8_t i = 0; i < motor_num_; ++i )
  {
    motors_[i]->setVelocity( cmd_vels[i] ); 
  } 
} 
void ArmController::setMotorPos( const double* cmd_pos ) 
{
  for ( uint8_t i = 0; i < motor_num_; ++i )
  {
    motors_[i]->setPosition( cmd_pos[i] ); 
  } 
} 
void ArmController::getMotorStates( double* pos_arr, double* vel_arr ) const 
{
  // populate the provided array 
  for ( uint8_t i = 0; i < motor_num_; ++i ) 
  { 
    pos_arr[i] = motors_[i]->getPosition(); 
    vel_arr[i] = motors_[i]->getVelocity(); 
  } 
}  

int ArmController::getNumMotors() const 
{
  return motor_num_; 
} 


#include "ArmController.h"

// -------------------- ARM CONTROLLER -------------------- // 
ArmController::ArmController( const int dofs ) 
  : motor_num_( dofs )
{
  memset( motors_, 0, MAX_MOTOR_NUM * sizeof(Motor) ); // clear the memory 
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
  for ( uint8_t i = 1; i < arg_num; i += 5 ) // 5 arguments per motor 
  { 
    // chunks of 5
    char motor_type[MOTOR_TYPE_BUFFERSIZE]; 
    strcpy( motor_type, argument_arr[i] ); // copy across 
    long red_ratio = atoi(argument_arr[i+1]); 
    int tx_pin = atoi(argument_arr[i+2]); 
    int rx_pin = atoi(argument_arr[i+3]); 
    long baud = atol(argument_arr[i+4]); 

    Serial.print("initialising motor with type: "); 
    Serial.print(motor_type); 
    Serial.print(", baud: ");
    Serial.print(baud); 
    Serial.print(", red_ratio: "); 
    Serial.print(red_ratio); 
    Serial.print(", rx_pin: "); 
    Serial.print(rx_pin); 
    Serial.print(", tx_pin: "); 
    Serial.println(tx_pin); 
    addMotor( motor_type, red_ratio, tx_pin, rx_pin, baud ); 
  }
} 

void ArmController::addMotor( const char* type, const long reduction_ratio, const int pin1, const int pin2, const long baud ) 
{
  // check the type  
  if ( strcmp( "odrive", type ) == 0 ) 
  {
    Serial.print("Making new odrive motor of size: "); 
    Serial.println(sizeof(OdriveMotor)); 
    motors_[motor_num_++] = new OdriveMotor( baud, reduction_ratio, pin1, pin2 ); 
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


void ArmController::printMotorErrors() const 
{
  for ( uint8_t i = 0; i < motor_num_; ++i ) 
  { 
    uint32_t err = motors_[i]->getErrors(); 
    Serial.print("Error Code for motor "); 
    Serial.print(i); 
    Serial.print( "; " );  
    Serial.println(err, HEX); 
  } 
}











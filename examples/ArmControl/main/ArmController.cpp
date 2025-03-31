
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
  for ( uint8_t i = 0; i < motor_num_; i++ ) 
  {
    delete motors_[i]; 
  }
}

void ArmController::initArm( char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], const int arg_num ) 
{
  // iterate over arguments, ignoring the first 
  Serial.println("Initialising arm"); 
  for ( uint8_t i = 1; i < arg_num; i += 4 ) // 4 arguments per motor
  { 
    // chunks of 4 (motor_type, red_ratio, baud, serial_port_name)
    char motor_type[MOTOR_TYPE_BUFFERSIZE]; 
    strcpy( motor_type, argument_arr[i] ); // copy across 
    long red_ratio = atoi(argument_arr[i+1]); 
    long baud = atol(argument_arr[i+2]); 
    char serial_port_name[CMD_STRING_BUFFERSIZE];
    strcpy(serial_port_name, argument_arr[i+3]); // get serial port name as string; e.g., Serial1. One ODrive per serial port. 

    Serial.print("initialising motor with type: "); 
    Serial.print(motor_type); 
    Serial.print(", baud: ");
    Serial.print(baud); 
    Serial.print(", red_ratio: "); 
    Serial.print(red_ratio); 
    Serial.print(", serial_port: "); 
    Serial.println(serial_port_name);
    
    HardwareSerial* serial_port = selectSerialPort(serial_port_name); // Select serial port based on input

    if (serial_port != nullptr) {
      addMotor(motor_type, red_ratio, baud, *serial_port); 
    } else {
      Serial.println("Error: Invalid serial port name");
    }
  }
} 

HardwareSerial* ArmController::selectSerialPort(const char* port_name)
{
  if (strcmp(port_name, "Serial1") == 0) {
    return &Serial1;  // Return reference to Serial1
  } else if (strcmp(port_name, "Serial2") == 0) {
    return &Serial2;  // Return reference to Serial2
  } else if (strcmp(port_name, "Serial3") == 0) {
    return &Serial3;  // Return reference to Serial3
  }
  // Add more serial ports here as needed (e.g., Serial4, Serial5)
  
  Serial.print("Don't recognise serial port name: ");
  Serial.println(port_name); 
  return nullptr; // Return null if invalid serial port name
}

void ArmController::addMotor( const char* type, const long reduction_ratio, const long baud, HardwareSerial& serial_port ) 
{
  // check the type  
  if ( strcmp( "odrive", type ) == 0 ) 
  {
    Serial.print("Making new odrive motor of size: "); 
    Serial.println(sizeof(OdriveMotor)); 
    motors_[motor_num_++] = new OdriveMotor( baud, reduction_ratio, serial_port ); // Use the selected serial port
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
    // Error: unrecognized motor type 
    Serial.print( "Error: Motor type '" ); 
    Serial.print( type ); 
    Serial.println( "' not recognized" ); 
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
  double pos; 
  double vel; 
  for ( uint8_t i = 0; i < motor_num_; ++i ) 
  { 
    motors_[i]->getPosVel( pos, vel ); 
    pos_arr[i] = pos; 
    vel_arr[i] = vel; 
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

void ArmController::reset()
{
  for ( uint8_t i = 0; i < motor_num_; ++i ) 
  { 
    motors_[i]->reset(); 
  } 
} 


/* Old version for Arduino below */


//
//#include "ArmController.h"
//
//// -------------------- ARM CONTROLLER -------------------- // 
//ArmController::ArmController( const int dofs ) 
//  : motor_num_( dofs )
//{
//  memset( motors_, 0, MAX_MOTOR_NUM * sizeof(Motor) ); // clear the memory 
//}
//
//ArmController::~ArmController()
//{
//  // delete the dynamically allocated memory 
//  for ( uint8_t i=0; i < motor_num_; i++ ) 
//  {
//    delete motors_[i]; 
//  }
//}
//
//void ArmController::initArm( char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], const int arg_num ) 
//{
//  // iterate over arguments, ignoring the first 
//  Serial.println("Initialising arm"); 
//  for ( uint8_t i = 1; i < arg_num; i += 5 ) // 5 arguments per motor 
//  { 
//    // chunks of 5
//    char motor_type[MOTOR_TYPE_BUFFERSIZE]; 
//    strcpy( motor_type, argument_arr[i] ); // copy across 
//    long red_ratio = atoi(argument_arr[i+1]); 
//    int tx_pin = atoi(argument_arr[i+2]); 
//    int rx_pin = atoi(argument_arr[i+3]); 
//    long baud = atol(argument_arr[i+4]); 
//
//    Serial.print("initialising motor with type: "); 
//    Serial.print(motor_type); 
//    Serial.print(", baud: ");
//    Serial.print(baud); 
//    Serial.print(", red_ratio: "); 
//    Serial.print(red_ratio); 
//    Serial.print(", rx_pin: "); 
//    Serial.print(rx_pin); 
//    Serial.print(", tx_pin: "); 
//    Serial.println(tx_pin); 
//    addMotor( motor_type, red_ratio, tx_pin, rx_pin, baud ); 
//  }
//} 
//
//void ArmController::addMotor( const char* type, const long reduction_ratio, const int pin1, const int pin2, const long baud ) 
//{
//  // check the type  
//  if ( strcmp( "odrive", type ) == 0 ) 
//  {
//    Serial.print("Making new odrive motor of size: "); 
//    Serial.println(sizeof(OdriveMotor)); 
//    motors_[motor_num_++] = new OdriveMotor( reduction_ratio, baud, Serial1 );   
//  }
//  else if ( strcmp( "stepper", type ) == 0 ) 
//  {
//    // TODO 
//  }
//  else if ( strcmp( "servo", type ) == 0 )
//  {
//    // TODO 
//  }
//  else
//  {
////    Serial.print( "Error: Motor type '" ); 
////    Serial.print( type ); 
////    Serial.println( "' not recognised" ); 
//  }
//} 
//
//void ArmController::setMotorVel( const double* cmd_vels ) 
//{
//  for ( uint8_t i = 0; i < motor_num_; ++i )
//  {
//    motors_[i]->setVelocity( cmd_vels[i] ); 
//  } 
//} 
//void ArmController::setMotorPos( const double* cmd_pos ) 
//{
//  for ( uint8_t i = 0; i < motor_num_; ++i )
//  {
//    motors_[i]->setPosition( cmd_pos[i] ); 
//  } 
//} 
//void ArmController::getMotorStates( double* pos_arr, double* vel_arr ) const 
//{
//  // populate the provided array 
//  for ( uint8_t i = 0; i < motor_num_; ++i ) 
//  { 
//    pos_arr[i] = motors_[i]->getPosition(); 
//    vel_arr[i] = motors_[i]->getVelocity(); 
//  } 
//}  
//
//int ArmController::getNumMotors() const 
//{
//  return motor_num_; 
//} 

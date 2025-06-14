#include "ArmController.h" 

ArmController::ArmController( CommandStore* cmd_store ) 
  : cmd_store_( cmd_store ) 
{
  memset( motors_, 0, MAX_MOTOR_NUM * sizeof(Motor) ); // clear the memory 
}

ArmController::~ArmController()
{
  // Delete the thread
  delete arm_control_thread_; 

  // delete the dynamically allocated memory 
  for ( uint8_t i = 0; i < motor_num_; i++ ) 
  {
    delete motors_[i]; 
  }
}

void ArmController::order66() 
{
	// Start thread
  Serial.println("Starting ArmController thread"); 
	arm_control_thread_ = new std::thread(&Runnable::runThread, this); 
 	arm_control_thread_->detach();
}

void ArmController::runTarget(void *arg) 
{
  Serial.println("Running ArmController"); 

  // run while there is a commmand there 
  CommandPackage* cmd_pkg = new CommandPackage(); // dynamic to reduce load on the stack 

  while ( cmd_store_->getMostRecentCmd(*cmd_pkg) ) 
  {
    char cmd; 
    cmd_pkg->getCmdType(cmd); 
    
    switch ( cmd )
    {
      case ( cmd_pkg->INIT_ARM ):
      { 
        initArm( *cmd_pkg ); 
        break; 
      }

      case ( cmd_pkg->READ_MOTOR_STATES ):
      { 
        double* pos_arr = new double[motor_num_];
        double* vel_arr = new double[motor_num_];
        getMotorStates(pos_arr, vel_arr);
        // TODO: will have to print it 

        delete[] pos_arr;
        delete[] vel_arr;
        break; 
      }
      case ( cmd_pkg->MOTOR_SPEEDS ):
      { 
        break; 
      }
      case ( cmd_pkg->MOTOR_POSITIONS ):
      { 
        break; 
      }
      case ( cmd_pkg->RESET ):
      { 
        break; 
      }
      case ( cmd_pkg->EMPTY_COMMAND ):
      { 
        break; 
      } 
      default:
      {
        Serial.println("Command not recognised."); 
      }
    }
  }
  delete cmd_pkg; 
}

void ArmController::initArm( CommandPackage& cmd_pkg ) 
{ 
  Serial.println("Initialising arm"); 
  int num_motors; 
  cmd_pkg.getNumMotors(num_motors); 

  MotorConfig* motor_list = new MotorConfig[ARG_ARRAY_BUFFERSIZE];
  cmd_pkg.getMotorList(motor_list, num_motors);

  // Iterate over motors 
  for (int i = 0; i < num_motors; ++i) 
  {
    Serial.print("Motor type: "); 
    Serial.println( motor_list[i].type_ ); 
    addMotor( motor_list[i] );  
  }
  delete[] motor_list;
}


void ArmController::addMotor( MotorConfig motor_config )
{
  // check the type  
  if ( strcmp( "odrive", motor_config.type_ ) == 0 ) 
  {
    Serial.print("Making new odrive motor of size: "); 
    Serial.println(sizeof(OdriveMotor)); 
    motors_[motor_num_++] = new OdriveMotor( motor_config.baud_, motor_config.red_ratio_, motor_config.serial_ ); // Use the selected serial port
  }
  else if ( strcmp( "stepper", motor_config.type_ ) == 0 ) 
  {
    // TODO 
  }
  else if ( strcmp( "servo", motor_config.type_ ) == 0 )
  {
    // TODO 
  }
  else
  {
    // Error: unrecognized motor type 
    Serial.print( "Error: Motor type '" ); 
    Serial.print( motor_config.type_ ); 
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
    delete motors_[i]; 
  } 
  motor_num_ = 0; 
} 



#include "Motors.h"


// ---------- MOTOR ---------- // 
Motor::Motor() {} 

// -------------------- BLDC MOTOR -------------------- // 
OdriveMotor::OdriveMotor( const long baud_rate, const int rx_pin, const int tx_pin ) 
  : //Motor(),                          // init the base class 
    odrive_serial_( rx_pin, tx_pin ), // init the odrive's serial 
    odrive_( odrive_serial_ )         // init the odrive handle 
{
  // start the serial communication 
  odrive_serial_.begin( baud_rate ); 
}

void OdriveMotor::setVelocity( const double vel ) 
{
  odrive_.setVelocity( vel ); 
}

void OdriveMotor::setPosition( const double pos )
{
  odrive_.setPosition( pos ); 
}

double OdriveMotor::getPosition()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_.getFeedback(); 
//  return feedback.pos; 
  return 1.0; 
}

double OdriveMotor::getVelocity()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_.getFeedback(); 
  return feedback.vel; 
}

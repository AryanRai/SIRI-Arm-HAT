
#include "Motors.h"


// ---------- MOTOR ---------- // 
Motor::Motor() {} 

// -------------------- BLDC MOTOR -------------------- // 
OdriveMotor::OdriveMotor( const long baud_rate, const int rx_pin, const int tx_pin ) 
  : odrive_serial_( rx_pin, tx_pin ) // init pointers to null 
{
  // configure the odrive 
  odrive_ = new ODriveUART( odrive_serial_ ); 
  
  // start the serial communication 
  odrive_serial_.begin( baud_rate ); 

//  Serial.println("Waiting for odrive..."); 
  while (odrive_->getState() == AXIS_STATE_UNDEFINED) 
  {
    delay(100);
  }
//  Serial.println("Found ODrive"); 
//  Serial.print("DC voltage: ");
//  Serial.println(odrive_->getParameterAsFloat("vbus_voltage"));

//  Serial.println("Enabling closed loop control...");
  while (odrive_->getState() != AXIS_STATE_CLOSED_LOOP_CONTROL) 
  {
    odrive_->clearErrors();
    odrive_->setState(AXIS_STATE_CLOSED_LOOP_CONTROL);
    delay(10);
  }
  
//  Serial.println("ODrive running!");
}

void OdriveMotor::setVelocity( const double vel ) 
{
  odrive_->setVelocity( vel ); 
}

void OdriveMotor::setPosition( const double pos )
{
  odrive_->setPosition( pos ); 
}

double OdriveMotor::getPosition()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_->getFeedback(); 
  return feedback.pos; 
}

double OdriveMotor::getVelocity()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_->getFeedback(); 
  return feedback.vel; 
}

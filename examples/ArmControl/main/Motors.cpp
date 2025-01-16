
#include "Motors.h"


// ---------- MOTOR ---------- // 
Motor::Motor() 
{
  Serial.println("Motor initialised");
} 

// -------------------- BLDC MOTOR -------------------- // 

OdriveMotor::OdriveMotor( const long baud_rate, const int tx_pin, const int rx_pin ) 
  : Motor(),                          // init the base class 
    odrive_serial_( tx_pin, rx_pin ), // init the odrive's serial 
    odrive_( odrive_serial_ )         // init the odrive handle 
{
  Serial.print("Initialising odrive motor on pin rx: "); 
  Serial.print(rx_pin); 
  Serial.print(", tx: ");
  Serial.println(tx_pin); 

  // start the serial communication 
  odrive_serial_.begin( baud_rate ); 

  delay(10);

  Serial.println("Waiting for ODrive...");
  while  (odrive_.getState() == AXIS_STATE_UNDEFINED ) {
    delay(100);
  }

  Serial.println("found ODrive");
  
  Serial.print("DC voltage: ");
  Serial.println(odrive_.getParameterAsFloat("vbus_voltage"));
  
  Serial.println("Enabling closed loop control...");
  while (odrive_.getState() != AXIS_STATE_CLOSED_LOOP_CONTROL) {
    odrive_.clearErrors();
    odrive_.setState(AXIS_STATE_CLOSED_LOOP_CONTROL);
    delay(10);
  }
  
  Serial.println("ODrive running!");
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
  return feedback.pos; 
}

double OdriveMotor::getVelocity()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_.getFeedback(); 
  return feedback.vel; 
}

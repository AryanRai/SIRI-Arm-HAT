
#include "Motors.h"


// ---------- MOTOR ---------- // 
Motor::Motor( long reduction_ratio ) : reduction_ratio_( reduction_ratio ) {} 

// -------------------- BLDC MOTOR -------------------- // 
OdriveMotor::OdriveMotor( const long baud_rate, const long reduction_ratio, const int rx_pin, const int tx_pin ) 
  : Motor( reduction_ratio ), 
    odrive_serial_( rx_pin, tx_pin ) 
{
  bool success = true; 
  
  // configure the odrive 
  odrive_ = new ODriveUART( odrive_serial_ ); 
  
  // start the serial communication 
  odrive_serial_.begin( baud_rate ); 

  Serial.println("Waiting for odrive..."); 
  int attempts = 0; 
  while ( odrive_->getState() == AXIS_STATE_UNDEFINED ) 
  {
//    printOdriveErrors(); 

    if ( attempts > 25 )
    {
      Serial.println("Error: Failed to connect to ODrive");
      success = false;  
      break;
    }
    else
    {
      attempts++;
    }
    
    delay(100);
  }

  // Enable closed loop control 
  Serial.println("Enabling closed loop control"); 
  attempts = 0; 
  while ( odrive_->getState() != AXIS_STATE_CLOSED_LOOP_CONTROL ) 
  {
//    printOdriveErrors(); 

    odrive_->clearErrors();
    odrive_->setState(AXIS_STATE_CLOSED_LOOP_CONTROL);

    if ( attempts > 25 )
    {
      Serial.println("Error: Failed to connect to ODrive"); 
      success = false; 
      break;
    }
    else
    {
      attempts++;
    }    
    delay(10);
  }

  if (success)
  {
    Serial.println("ODrive Running!");
  }
  else
  {
    Serial.println("Error: ODrive initialisation failed"); 
  }
  
}

void OdriveMotor::setVelocity( const double vel ) 
{
  odrive_->setVelocity( reduction_ratio_ * vel, MOVEMENT_TORQUE ); 
}

void OdriveMotor::setPosition( const double pos )
{
  odrive_->setPosition( reduction_ratio_ * pos, MOVEMENT_SPEED, MOVEMENT_TORQUE ); 
}

double OdriveMotor::getPosition()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_->getFeedback(); 
  return feedback.pos / reduction_ratio_; 
}

double OdriveMotor::getVelocity()
{
  // get feedback from odrive 
  ODriveFeedback feedback = odrive_->getFeedback(); 
  return feedback.vel / reduction_ratio_; 
}

//// Function that prints active errors to the Serial monitor
//void OdriveMotor::printOdriveErrors()  
//{
//  uint32_t errors = atol( odrive_->getParameterAsString( "axis0.active_errors" ).c_str() ); 
//  
//  if (errors == ODRIVE_ERROR_NONE) {
//    Serial.println("No errors (ODRIVE_ERROR_NONE)");
//    return;
//  }
//
//  Serial.println("Active Errors:");
//  if (errors & ODRIVE_ERROR_INITIALIZING)
//    Serial.println("  ODRIVE_ERROR_INITIALIZING");
//  if (errors & ODRIVE_ERROR_SYSTEM_LEVEL)
//    Serial.println("  ODRIVE_ERROR_SYSTEM_LEVEL");
//  if (errors & ODRIVE_ERROR_TIMING_ERROR)
//    Serial.println("  ODRIVE_ERROR_TIMING_ERROR");
//  if (errors & ODRIVE_ERROR_MISSING_ESTIMATE)
//    Serial.println("  ODRIVE_ERROR_MISSING_ESTIMATE");
//  if (errors & ODRIVE_ERROR_BAD_CONFIG)
//    Serial.println("  ODRIVE_ERROR_BAD_CONFIG");
//  if (errors & ODRIVE_ERROR_DRV_FAULT)
//    Serial.println("  ODRIVE_ERROR_DRV_FAULT");
//  if (errors & ODRIVE_ERROR_MISSING_INPUT)
//    Serial.println("  ODRIVE_ERROR_MISSING_INPUT");
//  if (errors & ODRIVE_ERROR_DC_BUS_OVER_VOLTAGE)
//    Serial.println("  ODRIVE_ERROR_DC_BUS_OVER_VOLTAGE");
//  if (errors & ODRIVE_ERROR_DC_BUS_UNDER_VOLTAGE)
//    Serial.println("  ODRIVE_ERROR_DC_BUS_UNDER_VOLTAGE");
//  if (errors & ODRIVE_ERROR_DC_BUS_OVER_CURRENT)
//    Serial.println("  ODRIVE_ERROR_DC_BUS_OVER_CURRENT");
//  if (errors & ODRIVE_ERROR_DC_BUS_OVER_REGEN_CURRENT)
//    Serial.println("  ODRIVE_ERROR_DC_BUS_OVER_REGEN_CURRENT");
//  if (errors & ODRIVE_ERROR_CURRENT_LIMIT_VIOLATION)
//    Serial.println("  ODRIVE_ERROR_CURRENT_LIMIT_VIOLATION");
//  if (errors & ODRIVE_ERROR_MOTOR_OVER_TEMP)
//    Serial.println("  ODRIVE_ERROR_MOTOR_OVER_TEMP");
//  if (errors & ODRIVE_ERROR_INVERTER_OVER_TEMP)
//    Serial.println("  ODRIVE_ERROR_INVERTER_OVER_TEMP");
//  if (errors & ODRIVE_ERROR_VELOCITY_LIMIT_VIOLATION)
//    Serial.println("  ODRIVE_ERROR_VELOCITY_LIMIT_VIOLATION");
//  if (errors & ODRIVE_ERROR_POSITION_LIMIT_VIOLATION)
//    Serial.println("  ODRIVE_ERROR_POSITION_LIMIT_VIOLATION");
//  if (errors & ODRIVE_ERROR_WATCHDOG_TIMER_EXPIRED)
//    Serial.println("  ODRIVE_ERROR_WATCHDOG_TIMER_EXPIRED");
//  if (errors & ODRIVE_ERROR_ESTOP_REQUESTED)
//    Serial.println("  ODRIVE_ERROR_ESTOP_REQUESTED");
//  if (errors & ODRIVE_ERROR_SPINOUT_DETECTED)
//    Serial.println("  ODRIVE_ERROR_SPINOUT_DETECTED");
//  if (errors & ODRIVE_ERROR_BRAKE_RESISTOR_DISARMED)
//    Serial.println("  ODRIVE_ERROR_BRAKE_RESISTOR_DISARMED");
//  if (errors & ODRIVE_ERROR_THERMISTOR_DISCONNECTED)
//    Serial.println("  ODRIVE_ERROR_THERMISTOR_DISCONNECTED");
//  if (errors & ODRIVE_ERROR_CALIBRATION_ERROR)
//    Serial.println("  ODRIVE_ERROR_CALIBRATION_ERROR");
//}

//void OdriveMotor::printOdriveState()
//{
//  int state = odrive_->getState(); 
//  switch (state)
//  {
//    case AXIS_STATE_UNDEFINED:
//      Serial.println("AXIS_STATE_UNDEFINED");
//      break;
//    case AXIS_STATE_IDLE:
//      Serial.println("AXIS_STATE_IDLE");
//      break;
//    case AXIS_STATE_STARTUP_SEQUENCE:
//      Serial.println("AXIS_STATE_STARTUP_SEQUENCE");
//      break;
//    case AXIS_STATE_FULL_CALIBRATION_SEQUENCE:
//      Serial.println("AXIS_STATE_FULL_CALIBRATION_SEQUENCE");
//      break;
//    case AXIS_STATE_MOTOR_CALIBRATION:
//      Serial.println("AXIS_STATE_MOTOR_CALIBRATION");
//      break;
//    case AXIS_STATE_ENCODER_INDEX_SEARCH:
//      Serial.println("AXIS_STATE_ENCODER_INDEX_SEARCH");
//      break;
//    case AXIS_STATE_ENCODER_OFFSET_CALIBRATION:
//      Serial.println("AXIS_STATE_ENCODER_OFFSET_CALIBRATION");
//      break;
//    case AXIS_STATE_CLOSED_LOOP_CONTROL:
//      Serial.println("AXIS_STATE_CLOSED_LOOP_CONTROL");
//      break;
//    case AXIS_STATE_LOCKIN_SPIN:
//      Serial.println("AXIS_STATE_LOCKIN_SPIN");
//      break;
//    case AXIS_STATE_ENCODER_DIR_FIND:
//      Serial.println("AXIS_STATE_ENCODER_DIR_FIND");
//      break;
//    case AXIS_STATE_HOMING:
//      Serial.println("AXIS_STATE_HOMING");
//      break;
//    case AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION:
//      Serial.println("AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION");
//      break;
//    case AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION:
//      Serial.println("AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION");
//      break;
//    case AXIS_STATE_ANTICOGGING_CALIBRATION:
//      Serial.println("AXIS_STATE_ANTICOGGING_CALIBRATION");
//      break;
//    default:
//      Serial.println("Error: State not recognised");
//      break;
//  }
//}

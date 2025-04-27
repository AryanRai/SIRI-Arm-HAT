
#ifndef MOTORS_H
#define MOTORS_H

#include "Arduino.h"
#include "ODriveUART.h" 


//// ----------- MOTOR BASE CLASS ----------- //
class Motor
{
  public: 
    Motor( long reduction_ratio ); 
    virtual ~Motor() {} 

    virtual void setVelocity( const double vel ) = 0; 
    virtual void setPosition( const double pos ) = 0; 
    virtual double getPosition() = 0;
    virtual double getVelocity() = 0; 
    virtual void getPosVel( double& pos, double& vel ) = 0; 
    virtual uint32_t getErrors() = 0;
    virtual void reset() = 0;

  protected:
    long reduction_ratio_;
}; 


//// --------------- ODrive MOTOR CLASS --------------- //
class OdriveMotor : public Motor 
{ 
  public: 
    OdriveMotor( const long reduction_ratio, const long baud_rate, HardwareSerial& serial_port );

    void setVelocity( const double vel ); // Set motor velocity
    void setPosition( const double pos ); 
    double getPosition(); 
    double getVelocity();
    void getPosVel( double& pos, double& vel ); 

    uint32_t getErrors();   // Get error code from ODrive
    void reset();           // Reset the odrive

  private: 
    const float METRIC_TO_RAD = 2*3.14159; 
    const int ATTEMPT_LIMIT = 100; 
    const int INVALID_COMMAND = -9999; 

    HardwareSerial& odrive_serial_;
    ODriveUART* odrive_; 
}; 

#endif 




//
//#ifndef MOTORS_H
//#define MOTORS_H
//
//#include "Arduino.h"
//#include "ODriveUART.h"
//#include <SoftwareSerial.h>
//
//
//class Motor
//{
//  public: 
//    Motor( long reduction_ratio ); 
//    virtual ~Motor() {} 
//
//    virtual void setVelocity( const double vel ) = 0; 
//    virtual void setPosition( const double pos ) = 0; 
//    virtual double getPosition() = 0;
//    virtual double getVelocity() = 0; 
//    
//  protected:
//    long reduction_ratio_; // speed reduction of the motor, e.g. a 1:10 reduction would be input at 10
//
//}; 
//
//class OdriveMotor : public Motor 
//{ 
//  public: 
//    OdriveMotor( const long reduction_ratio, const long baud_rate, const int tx_pin, const int rx_pin ); 
//
//    void setVelocity( const double vel ); 
//    void setPosition( const double pos ); 
//    double getPosition(); 
//    double getVelocity(); 
//
//  private: 
//    // ----- SUBCLASSES/STRUCTS ----- // 
//
//    // ----- CONSTANTS ----- // 
//    const float MOVEMENT_SPEED = 2.0; // Rad/s
//    const float MOVEMENT_TORQUE = 1.0; // Nm 
//    const int ATTEMPT_LIMIT = 100; 
//    const int INVALID_COMMAND = -9999; 
//
//    // ----- METHODS ----- // 
//    //void printOdriveState();
//    void printOdriveErrors();
//
//    // ----- DATA ----- // 
//    // normal odrive handle 
//
//    SoftwareSerial odrive_serial_;
//
////    HardwareSerial& odrive_serial_;
//    
//    ODriveUART* odrive_; 
//    
//    long baud_rate_; 
//}; 
//
//#endif 


#ifndef MOTORS_H
#define MOTORS_H

#include "ODriveUART.h"
#include <SoftwareSerial.h> 


class Motor
{
  public: 
    Motor(); 
    virtual ~Motor() {} 

    virtual void setVelocity( const double vel ) = 0; 
    virtual void setPosition( const double pos ) = 0; 
    virtual double getPosition() = 0;
    virtual double getVelocity() = 0; 
    
  protected:

}; 

class OdriveMotor : public Motor 
{ 
  public: 
      OdriveMotor( const long baud_rate, const int tx_pin, const int rx_pin ); 

      void setVelocity( const double vel ); 
      void setPosition( const double pos ); 
      double getPosition(); 
      double getVelocity(); 
      
      // normal odrive handle 
      SoftwareSerial odrive_serial_;
      ODriveUART odrive_; 

  private: 
      long baud_rate_; 
}; 

#endif 

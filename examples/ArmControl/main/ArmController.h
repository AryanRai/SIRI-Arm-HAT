
#ifndef ARM_CONTROLLER_H
#define ARM_CONTROLLER_H

#include "Motors.h"
#include <HardwareSerial.h>  // Include HardwareSerial for serial communication

// Define constants for motor types, buffers, etc.
#define MAX_MOTOR_NUM 10  // Adjust based on your system
#define ARG_ARRAY_BUFFERSIZE 10  // Example size, adjust as needed
#define CMD_STRING_BUFFERSIZE 20  // Example size, adjust as needed
#define MOTOR_TYPE_BUFFERSIZE 10  // Example size, adjust as needed

class ArmController
{
public:
    // Constructor and Destructor
    ArmController(const int dofs); // 'dofs' is Degrees of Freedom (motor count)
    ~ArmController();

    // Method to initialize arm (including motors)
    void initArm(char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], const int arg_num);

    // Method to add motor (pass serial port dynamically)
    void addMotor(const char* type, const long reduction_ratio, const long baud, HardwareSerial& serial_port, int pin);

    // Method to set motor velocities
    void setMotorVel(const double* cmd_vels);

    // Method to set motor positions
    void setMotorPos(const double* cmd_pos);

    // Method to get the current motor states (positions and velocities)
    void getMotorStates(double* pos_arr, double* vel_arr) const;

    // Method to get the number of motors
    int getNumMotors() const;

    // Method to print motor error codes
    void printMotorErrors() const;

    // Method to reset all motors 
    void reset();

private:
    int motor_num_;   // Number of motors
    Motor* motors_[MAX_MOTOR_NUM]; // Array of motor pointers

    // Function to select serial port dynamically (based on string input)
    HardwareSerial* selectSerialPort(const char* port_name); // Select serial port based on a string identifier
};

#endif



/* Old version for Arduino below */


//#ifndef ARMCONTROLLER_H
//#define ARMCONTROLLER_H
//
//#include "Motors.h"
//#include "commands.h"
//
//class ArmController
//{
//  public:
//    ArmController( const int dofs ); 
//    ~ArmController(); 
//
//    void initArm( char argument_arr[ARG_ARRAY_BUFFERSIZE][CMD_STRING_BUFFERSIZE], const int arg_num ); 
//    void addMotor( const char* type, const long reduction_ratio, HardwareSerial& serial_port, const long baud ); 
//    void setMotorVel( const double* cmd_vels ); 
//    void setMotorPos( const double* cmd_pos ); 
//    void getMotorStates( double* pos_arr, double* vel_arr ) const; 
//    int getNumMotors() const; 
//
//  private:
//    // --- STATIC CONSTANTS --- // 
//    static const int MAX_MOTOR_NUM = 3; 
//    static const int MOTOR_TYPE_BUFFERSIZE = 7; // can only be odrive, stepper or servo 
//
//    // --- DATA --- // 
//    Motor* motors_[MAX_MOTOR_NUM]; 
//    int motor_num_; 
//}; 
//
//
//#endif 

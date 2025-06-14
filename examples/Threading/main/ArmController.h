#ifndef ARM_CONTROLLER_H
#define ARM_CONTROLLER_H

#include "Motors.h"
#include "Runnable.h"
#include "CommandStore.h" 
#include <HardwareSerial.h>  // Include HardwareSerial for serial communication
#include <SoftwareSerial.h> 
#include <TeensyThreads.h>

// Define constants for motor types, buffers, etc.
#define MAX_MOTOR_NUM 10  // Adjust based on your system
#define ARG_ARRAY_BUFFERSIZE 10  // Example size, adjust as needed
#define CMD_STRING_BUFFERSIZE 20  // Example size, adjust as needed
#define MOTOR_TYPE_BUFFERSIZE 10  // Example size, adjust as needed

class ArmController : Runnable 
{
public:
    // Constructor and Destructor
    ArmController( CommandStore* cmd_store ); 
    ~ArmController();

    // Method to initialize arm (including motors) 
    void initArm( CommandPackage& cmd_pkg ); 

    // Method to add motor (pass serial port dynamically)
    void addMotor( MotorConfig motor_config );

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

    // Start the thread that will begin reading and executing commands 
    void order66(); // startExecuting(); 

protected:
    // Runnable function that we need to implement 
    void runTarget(void *arg); 


private:
    int motor_num_;   // Number of motors 
    CommandStore* cmd_store_; 
    Motor* motors_[MAX_MOTOR_NUM]; // Array of motor pointers 
    std::thread *arm_control_thread_; 

    // Function to select serial port dynamically (based on string input)
    HardwareSerial* selectSerialPort(const char* port_name); // Select serial port based on a string identifier
};

#endif
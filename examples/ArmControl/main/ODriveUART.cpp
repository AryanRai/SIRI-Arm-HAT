
// Author: ODrive Robotics Inc.
// License: MIT
// Documentation: https://docs.odriverobotics.com/v/latest/guides/arduino-uart-guide.html

#include "Arduino.h"
#include "ODriveUART.h" 

static const int kMotorNumber = 0;

// Print with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

ODriveUART::ODriveUART(Stream& serial)
    : serial_(serial) 
{
//  Serial.println("Odrive UART initialised"); 
}

void ODriveUART::clearErrors() {
    serial_ << "sc\n";
}

void ODriveUART::setPosition(float position) {
    setPosition(position, 0.0f, 0.0f);
}

void ODriveUART::setPosition(float position, float velocity_feedforward) {
    setPosition(position, velocity_feedforward, 0.0f);
}

void ODriveUART::setPosition(float position, float velocity_feedforward, float torque_feedforward) {
    serial_ << "p " << kMotorNumber  << " " << position << " " << velocity_feedforward << " " << torque_feedforward << "\n";
}

void ODriveUART::setVelocity(float velocity) {
    setVelocity(velocity, 0.0f);
}

void ODriveUART::setVelocity(float velocity, float torque_feedforward) {
    serial_ << "v " << kMotorNumber  << " " << velocity << " " << torque_feedforward << "\n";
}

void ODriveUART::setTorque(float torque) {
    serial_ << "c " << kMotorNumber << " " << torque << "\n";
}

void ODriveUART::trapezoidalMove(float position) {
    serial_ << "t " << kMotorNumber << " " << position << "\n";
}

ODriveFeedback ODriveUART::getFeedback() {
    // Flush RX
    while (serial_.available()) {
        serial_.read();
    }

    serial_ << "f " << kMotorNumber << "\n";

    String response = readLine();

    int spacePos = response.indexOf(' ');
    if (spacePos >= 0) {
        return {
            response.substring(0, spacePos).toFloat(),
            response.substring(spacePos+1).toFloat()
        };
    } else {
        return {0.0f, 0.0f};
    }
}

String ODriveUART::getParameterAsString(const String& path) {
    serial_ << "r " << path << "\n";
    return readLine();
}

void ODriveUART::setParameter(const String& path, const String& value) {
    serial_ << "w " << path << " " << value << "\n";
}

void ODriveUART::setState(ODriveAxisState requested_state) {
    setParameter("axis0.requested_state", String((long)requested_state));
}

ODriveAxisState ODriveUART::getState() {
    return (ODriveAxisState)getParameterAsInt("axis0.current_state");
}

String ODriveUART::readLine(unsigned long timeout_ms) {
    String str = "";
    unsigned long timeout_start = millis();
    for (;;) {
        while (!serial_.available()) {
            if (millis() - timeout_start >= timeout_ms) {
                return str;
            }
        }
        char c = serial_.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}


/* Below is the version where I tried to have it slowly ramping up the speed */

//// Author: ODrive Robotics Inc.
//// License: MIT
//// Documentation: https://docs.odriverobotics.com/v/latest/guides/arduino-uart-guide.html
//
//#include "Arduino.h"
//#include "ODriveUART.h" 
//
//static const int kMotorNumber = 0;
//
//// Print with stream operator
//template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
//template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }
//
//ODriveUART::ODriveUART(Stream& serial)
//    : serial_(serial) 
//{
////  Serial.println("Odrive UART initialised"); 
//}
//
//void ODriveUART::clearErrors() {
//    serial_ << "sc\n";
//}
//
///* 
//Original setPosition functions
//
//// void ODriveUART::setPosition(float position) {
////     setPosition(position, 0.0f, 0.0f);
//// }
//
//// void ODriveUART::setPosition(float position, float velocity_feedforward) {
////     setPosition(position, velocity_feedforward, 0.0f);
//// }
//
//// void ODriveUART::setPosition(float position, float velocity_feedforward, float torque_feedforward) {
////     serial_ << "p " << kMotorNumber  << " " << position << " " << velocity_feedforward << " " << torque_feedforward << "\n";
//// }
//
//*/
//
//
//void ODriveUART::setPosition(float position) {
//    setPosition(position, 0.0f, 0.0f);  // Default to no velocity/torque feedforward
//}
//
//// Set target position with velocity feedforward (the anticipated velocity)
//void ODriveUART::setPosition(float position, float velocity_feedforward) {
//    setPosition(position, velocity_feedforward, 0.0f);  // Default to no torque feedforward
//}
//
//// Set target position with both velocity and torque feedforward and send position and feedforward values to the motor
//void ODriveUART::setPosition(float position, float velocity_feedforward, float torque_feedforward) {
//    serial_ << "p " << kMotorNumber << " " << position << " " << velocity_feedforward << " " << torque_feedforward << "\n";
//    targetPosition = position;
//    this->velocityFeedforward = velocity_feedforward;
//    this->torqueFeedforward = torque_feedforward;
//    rampStartTime = millis();  // Record start time for ramping
//    isRamping = true;  // Begin ramping process
//}
//
//
//// Update position gradually
//void ODriveUART::updatePosition() {
//    if (isRamping) {
//        unsigned long elapsedTime = millis() - rampStartTime;
//
//        if (elapsedTime < rampDuration) {
//            // Calculate the progress (0.0 to rampduration) based on elapsed time
//            float progress = float(elapsedTime) / rampDuration;
//
//            // Gradually interpolate the position towards the target
//            currentPosition = targetPosition * progress;
//            
//            currentVelocity = targetVelocity * progress;
//
//            // Send updated position and feedforward values to the motor controller
//            setPosition(currentPosition, velocityFeedforward, torqueFeedforward);
//        }  
//        else {
//            // If the ramp is complete, directly set the target position
//            currentPosition = targetPosition;
//
//            // Send the final position and feedforward values
//            setPosition(currentPosition, velocityFeedforward, torqueFeedforward);
//
//            // Stop ramping
//            isRamping = false;
//        }
//    } 
//    
//    else {
//        // If no ramping, just set the target position directly
//        setPosition(targetPosition, velocityFeedforward, torqueFeedforward);
//    }
//}
//
//
//
//
//// Set target velocity and torque feedforward with a ramp duration
//void ODriveUART::setVelocity(float velocity) {
//    setVelocity(velocity, 0.0f, 2000);  // Default ramp duration to 2000 ms
//}
//
//// Send the velocity and torque feedforward to the motor
//void ODriveUART::setVelocity(float velocity, float torque_feedforward) {
//    // Send command to motor controller via serial communication
//    serial_ << "v " << kMotorNumber << " " << velocity << " " << torque_feedforward << "\n";
//}
//
//// Set target velocity and torque feedforward with a ramp duration
//void ODriveUART::setVelocity(float velocity, float torque_feedforward, unsigned long rampDuration) {
//    targetVelocity = velocity;  
//    targetTorqueFeedforward = torque_feedforward;
//    this->rampDuration = rampDuration;
//    rampStartTime = millis();
//    isRamping = true;
//}
//
//
//// Update velocity and torque feedforward gradually over time
//void ODriveUART::updateVelocity() {
//  if (isRamping) {
//      unsigned long elapsedTime = millis() - rampStartTime;
//
//      if (elapsedTime < rampDuration) {
//          // Calculate the progress ratio between 0 and 1
//          float progress = float(elapsedTime) / rampDuration;
//
//          // Interpolate velocity and torque_feedforward based on progress
//          currentVelocity = (targetVelocity * progress);
//          currentTorqueFeedforward = (targetTorqueFeedforward * progress);
//
//          // Send the updated values to the motor
//          setVelocity(currentVelocity, currentTorqueFeedforward);
//      } 
//      else {
//          // If the ramp-up is complete, set the target values directly
//          currentVelocity = targetVelocity;
//          currentTorqueFeedforward = targetTorqueFeedforward;
//
//          // Send the final values
//          setVelocity(currentVelocity, currentTorqueFeedforward);
//
//          // Stop ramping
//          isRamping = false;
//      }
//  } 
//  else {
//      // If no ramping, set the target velocity and torque feedforward directly
//      setVelocity(targetVelocity, targetTorqueFeedforward);
//  }
//}
//
//
//
//void ODriveUART::setTorque(float torque) {
//    serial_ << "c " << kMotorNumber << " " << torque << "\n";
//}
//
//void ODriveUART::trapezoidalMove(float position) {
//    serial_ << "t " << kMotorNumber << " " << position << "\n";
//}
//
//ODriveFeedback ODriveUART::getFeedback() {
//    // Flush RX
//    while (serial_.available()) {
//        serial_.read();
//    }
//
//    serial_ << "f " << kMotorNumber << "\n";
//
//    String response = readLine();
//
//    int spacePos = response.indexOf(' ');
//    if (spacePos >= 0) {
//        return {
//            response.substring(0, spacePos).toFloat(),
//            response.substring(spacePos+1).toFloat()
//        };
//    } else {
//        return {0.0f, 0.0f};
//    }
//}
//
//String ODriveUART::getParameterAsString(const String& path) {
//    serial_ << "r " << path << "\n";
//    return readLine();
//}
//
//void ODriveUART::setParameter(const String& path, const String& value) {
//    serial_ << "w " << path << " " << value << "\n";
//}
//
//void ODriveUART::setState(ODriveAxisState requested_state) {
//    setParameter("axis0.requested_state", String((long)requested_state));
//}
//
//ODriveAxisState ODriveUART::getState() {
//    return (ODriveAxisState)getParameterAsInt("axis0.current_state");
//}
//
//String ODriveUART::readLine(unsigned long timeout_ms) {
//    String str = "";
//    unsigned long timeout_start = millis();
//    for (;;) {
//        while (!serial_.available()) {
//            if (millis() - timeout_start >= timeout_ms) {
//                return str;
//            }
//        }
//        char c = serial_.read();
//        if (c == '\n')
//            break;
//        str += c;
//    }
//    return str;
//}

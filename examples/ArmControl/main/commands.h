/* Define single-letter commands that will be sent by the PC over the
   serial link.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#define READ_MOTOR_STATES  'e' // command: "e\r\n"
#define MOTOR_SPEEDS       'v' // command: "v v1 v2 ... vN\r\n"
#define MOTOR_POSITIONS    'p' // command: "p p1 p2 ... pN\r\n"
#define INIT_ARM           'i' // command: "i type1 red_ratio1 tx1 rx1 baud1 type2 red_ratio2 tx2 rx2 baud2 ... typeN red_ratioN txN rxN baudN\r\n"
                               // e.g. i odrive 1 2 3 19200 odrive 16 8 9 19200
#define RESET_ARDUINO      'r' // command: "r\r\n"
#define EMPTY_COMMAND      '\r'
#define PRINT_ERRORS       's' // command: "e\r\n", will tell you current error state of the motors 
//#define LEFT               0
//#define RIGHT              1

const int CMD_STRING_BUFFERSIZE = 15; // longest command word will be positions sent by ROS, usually max 11 chars long
const int ARG_ARRAY_BUFFERSIZE = 37; // longest command string is the arm init, would be 7 motors * 5 arguments + 1 command = 36
const int ERROR_STR_BUFFER = 100; // longest allowed error string

//#define ANALOG_READ        'a'
//#define GET_BAUDRATE       'b'
//#define PIN_MODE           'c'
//#define DIGITAL_READ       'd'
//#define MOTOR_RAW_PWM      'o'
//#define PING               'p'
//#define RESET_ENCODERS     'r'
//#define UPDATE_PID         'u'
//#define DIGITAL_WRITE      'w'
//#define ANALOG_WRITE       'x'

#endif

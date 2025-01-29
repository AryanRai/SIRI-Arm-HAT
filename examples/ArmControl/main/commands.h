/* Define single-letter commands that will be sent by the PC over the
   serial link.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#define READ_MOTOR_STATES  'e' // command: "e\r\n"
#define MOTOR_SPEEDS       'v' // command: "v v1 v2 ... vN\r\n"
#define MOTOR_POSITIONS    'p' // command: "p p1 p2 ... pN\r\n"
#define INIT_ARM           'i' // command: "i type1 pin11 pin12 baud1 type2 pin21 pin 22 baud2 ... typeN pinN1 pinN2 baudN\r\n"
#define RESET_ARDUINO      'r' // command: "r\r\n"
#define LEFT               0
#define RIGHT              1

const int CMD_STRING_BUFFERSIZE = 30; 
const int ARG_ARRAY_BUFFERSIZE = 30; 

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

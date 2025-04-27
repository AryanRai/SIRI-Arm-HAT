/* Define single-letter commands that will be sent by the PC over the
   serial link.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#define READ_MOTOR_STATES  'e' // command: "e\r\n"
#define MOTOR_SPEEDS       'v' // command: "v v1 v2 ... vN\r\n" in m/s
#define MOTOR_POSITIONS    'p' // command: "p p1 p2 ... pN\r\n" in radians
#define INIT_ARM           'i' // arduino command: "i type1 red_ratio1 tx1 rx1 baud1 type2 red_ratio2 tx2 rx2 baud2 ... typeN red_ratioN txN rxN baudN\r\n"
                               // teensy command example: "i type1 red_ratio1 baud1 Serial_n
                               // teensy command example: "i odrive 0.5 19200 Serial1    --> Serial1 is for big motor; Serial2 for small motor as of March 21
#define RESET_ARDUINO      'r' // command: "r\r\n"
#define EMPTY_COMMAND      '\r'

const int CMD_STRING_BUFFERSIZE = 15; // longest command word will be positions sent by ROS, usually max 11 chars long
const int ARG_ARRAY_BUFFERSIZE = 36; // longest command string is the arm init, would be 7 motors * 5 arguments + 1 command = 36

#endif

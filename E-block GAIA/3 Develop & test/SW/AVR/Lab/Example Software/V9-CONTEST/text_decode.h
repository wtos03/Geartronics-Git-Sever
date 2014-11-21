/* Include file for text command 


*/

#include <avr/pgmspace.h>



// Function prototype 

uint8_t text_decode (uint8_t *, uint8_t *);


// Define command interface later change to ptr to function
#define  MOTOR_CMD   		1
#define  SPEED_CMD  		2
#define  ANGLE_CMD			3
#define  PROGRAM_CMD		4
#define  SAVE_CMD			5
#define  RUN_CMD			6
 

#define  CMD_LENGTH			10
#define  CMD_NUM			7    // Number of Command +1 (NUll characters) 
#define  CMD_LINE_END   	0xFF


// No change part 

#define  WAIT_CMD_STATE		1
#define  GET_CMD_STATE		2
#define  WAIT_ARG_STATE		3
#define  GET_ARG_STATE 		4

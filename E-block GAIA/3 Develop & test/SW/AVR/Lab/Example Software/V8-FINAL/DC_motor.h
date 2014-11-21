/* 
 Include file for DC_motor.c
 
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   4/5/2006
 
 Summary for include files
 -------------------------
 Constant variable				X
 Function prototype				-
 Variable (Flash memory)		-
 Memory usage					0 bytes
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
*/



/* External Dependents    Need outside parameters 


 
*/


#include "cpu.h"



// Function prototypes
void  init_motor (void);
void  motor_control (uint8_t, uint8_t);
 


 

// Port use for Motor Control
#define  MOTOR_DIRECTION_CONTROL		E_OUT_PORTC
#define  MOTOR_ENABLE12_CONTROL			E_OUT_PORTD
#define  MOTOR_ENABLE34_CONTROL			E_OUT_PORTD

#define  MOTOR_DIRECTION_DIR			E_DIR_PORTC
#define  MOTOR_ENABLE12_DIR				E_DIR_PORTD
#define  MOTOR_ENABLE34_DIR				E_DIR_PORTD


//  port Number

#define MOTORA_1    			E_PORT4			 
#define MOTORA_2    			E_PORT5			 
#define MOTORB_1    			E_PORT6			 
#define MOTORB_2    			E_PORT7			 
#define MOTOR_ENABLE12			E_PORT5
#define MOTOR_ENABLE34			E_PORT4

 
#define MOTOR_STOP      0
#define MOTOR_FORWARD   1
#define MOTOR_BACKWARD  2
 
#define MOTOR_TURN_LEFT  4
#define MOTOR_TURN_RIGHT 5

#define MOTOR_MAX       2
#define MOTOR_SPEED_MULTIPLY  30
#define MOTOR_ANGLE_COEF  100
#define MOTOR_ANGLE_SPEED 3
#define MOTOR_OFFSET 	  50
#define MOTOR_MAX_SPEED   9


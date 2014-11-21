/* 
 Include file for Dual_7Segment.c
 
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   21/4/2006
 
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
 
void d7segment_display(int8_t, uint8_t);
void d7segment_init (void);
 

// Port use for Dual 7 Segments

#define  _7SEGMENT_PORT   	 	E_OUT_PORTA
#define  _7SEGMENT_DIGIT_PORT	E_OUT_PORTB

#define  _7SEGMENT_A		 E_PORT0
#define  _7SEGMENT_B		 E_PORT1
#define  _7SEGMENT_C		 E_PORT2
#define  _7SEGMENT_D		 E_PORT3
#define  _7SEGMENT_E		 E_PORT4
#define  _7SEGMENT_F		 E_PORT5
#define  _7SEGMENT_G		 E_PORT6
#define  _7SEGMENT_DOT	 	 E_PORT7

#define  _7SEGMENT_DIGIT1 	 E_PORT3
#define  _7SEGMENT_DIGIT2 	 E_PORT4

#define  _7SEGMENT_DIGIT_DIR E_DIR_PORTA
#define  _7SEGMENT_PORT_DIR	 E_DIR_PORTB


// Port use for Press SW
#define   _7SEGMENT_SW1_IN_PORT  	E_IN_PORTD
#define   _7SEGMENT_SW2_IN_PORT  	E_IN_PORTB
#define   _7SEGMENT_SW1_PORT  		E_OUT_PORTD
#define   _7SEGMENT_SW2_PORT  		E_OUT_PORTB
#define   _7SEGMENT_SW1_DIR	 		E_DIR_PORTD
#define   _7SEGMENT_SW2_DIR	 		E_DIR_PORTB
#define   _7SEGMENT_SW1		 		E_INT1
#define   _7SEGMENT_SW2		 		E_PORT2
 
 // Digit 0 - 9 

#define _7SEGMENT_0		 _7SEGMENT_A | _7SEGMENT_B | _7SEGMENT_C | _7SEGMENT_D | _7SEGMENT_E | _7SEGMENT_F
#define _7SEGMENT_1		 _7SEGMENT_B | _7SEGMENT_C
#define _7SEGMENT_2		 _7SEGMENT_A | _7SEGMENT_B | _7SEGMENT_G | _7SEGMENT_E | _7SEGMENT_D
#define _7SEGMENT_3		 _7SEGMENT_A | _7SEGMENT_B | _7SEGMENT_C | _7SEGMENT_D | _7SEGMENT_G
#define _7SEGMENT_4		 _7SEGMENT_B | _7SEGMENT_C | _7SEGMENT_F | _7SEGMENT_G
#define _7SEGMENT_5		 _7SEGMENT_A | _7SEGMENT_F | _7SEGMENT_G | _7SEGMENT_C | _7SEGMENT_D
#define _7SEGMENT_6		 _7SEGMENT_C | _7SEGMENT_D | _7SEGMENT_E | _7SEGMENT_F | _7SEGMENT_G
#define _7SEGMENT_7		 _7SEGMENT_A | _7SEGMENT_B | _7SEGMENT_C
#define _7SEGMENT_8		 _7SEGMENT_A | _7SEGMENT_B | _7SEGMENT_C | _7SEGMENT_D | _7SEGMENT_E | _7SEGMENT_F |_7SEGMENT_G
#define _7SEGMENT_9		 _7SEGMENT_A | _7SEGMENT_B | _7SEGMENT_C | _7SEGMENT_F | _7SEGMENT_G
#define _7SEGMENT_C_F	 _7SEGMENT_A | _7SEGMENT_E | _7SEGMENT_F |_7SEGMENT_G
#define _7SEGMENT_C_B	 _7SEGMENT_C | _7SEGMENT_D | _7SEGMENT_E | _7SEGMENT_F |_7SEGMENT_G
#define DOT	10	

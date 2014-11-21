/* 
 Include file for timer.c
 
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   18/5/2006
 
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
CPU_CLOCK_SPEED

include  file 			 
*/


#include "cpu.h"



// Function prototypes
 
// Need to enable interrupt by yourself
 
 


// ??? port Number
#define  TIMER_PORT					E_OUT_PORTD
#define  TIMER_DDR					E_DIR_PORTD
#define  TIMER0_OC    				E_PORT3			//  Output for TImer0
#define  TIMER1B_OC				    E_PORT4
#define  TIMER1A_OC				    E_PORT5
#define  TIMER2_OC					E_PORT7

// TIMER NUMBER
#define TIMER0				1
#define TIMER1A				2
#define TIMER1B				3
#define TIMER2				4

#define  NUMBER_TIMER        4
#define  NUMBER_OUTPUT_MODE	 4
#define  NUMBER_TIMER_MODE   4
#define  NUMBER_COMBINATION  NUMBER_TIMER*NUMBER_OUTPUT_MODE*NUMBER_TIMER_MODE 


#define  TIMER_EXTERNAL_CLK	   1

// TIMER/COUNTER MODE 
#define  TIMER_COUNTER_MODE       0x10
#define  TIMER_CLEAR_TIMER_MODE   0x20
#define  TIMER_FAST_PWM_MODE      0x30
#define  TIMER_PHASE_PWM_MODE     0x40






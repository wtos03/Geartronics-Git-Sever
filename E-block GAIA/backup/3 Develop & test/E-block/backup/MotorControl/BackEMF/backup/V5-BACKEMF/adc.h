/* 
 Include file for 		adc.c
 
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   17/5/2006
 
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

*/


#include "cpu.h"



// Function prototypes
void  adc_enable_int (uint8_t);
uint16_t read_adc(uint8_t);
void adc_init(uint8_t);

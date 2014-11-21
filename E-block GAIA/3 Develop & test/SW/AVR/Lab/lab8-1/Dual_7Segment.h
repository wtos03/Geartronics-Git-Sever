 /*
 Include File			:   Dual_7Segment.h
 Description 			:   Include file for Dual_7Segment
 Specification			:   Define the number code for 7 Segments
 CPU port/tested		:   ATMEGA32
 Hardware use			:   E_Mega32, E_IO Module
 Port use				:   Port name / Function

 Written by 			:  wichai@geartronics.net
 Date					:  3/Sep/2007
  
 Update History
 
 Date			By 					Comments
 ----			--					---------

*/

 


/* External Dependents    Need outside parameters 


*/

#include <avr/io.h>

 
 
 

// Port use for Dual 7 Segments
#define F_CPU 16000000UL 
#define  _7SEGMENT_PORT   	 	PORTB
#define  _7SEGMENT_DIGIT_PORT	PORTC

#define  _7SEGMENT_A		 0x01
#define  _7SEGMENT_B		 0x02
#define  _7SEGMENT_C		 0x04
#define  _7SEGMENT_D		 0x08
#define  _7SEGMENT_E		 0x10
#define  _7SEGMENT_F		 0x20
#define  _7SEGMENT_G		 0x40
#define  _7SEGMENT_DOT	 	 0x80

#define  _7SEGMENT_DIGIT1 	 0x08
#define  _7SEGMENT_DIGIT2 	 0x10

#define  _7SEGMENT_DIGIT_DIR DDRB
#define  _7SEGMENT_PORT_DIR	 DDRC


// Port use for Press SW
#define   _7SEGMENT_SW1_IN_PORT  	PINC
#define   _7SEGMENT_SW2_IN_PORT  	PINC
#define   _7SEGMENT_SW1_PORT  		PORTC
#define   _7SEGMENT_SW2_PORT  		PORTC
#define   _7SEGMENT_SW1_DIR	 		DDRC
#define   _7SEGMENT_SW2_DIR	 		DDRC
#define   _7SEGMENT_SW1		 		0x20
#define   _7SEGMENT_SW2		 		0x04
 
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

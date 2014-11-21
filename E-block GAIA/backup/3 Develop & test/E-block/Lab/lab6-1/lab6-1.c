/*
 Program 			    :   lab6-1.c
 Description 			:   Basic Input/Output lab
 Specification			:   7 Segments increase from current to 9 when
							press UP SW
						:   7 Segment decrease to 0 when press down SW
 CPU port/tested		:   ATMEGA32
 Hardware use			:   E_Mega32, E_IO Module
 Port use				:   Port name / Function

 Written by 			:  wichai@geartronics.net
 Date					:  3/Sep/2007
  
 What we learn
		-  Solve the debounce from SW
		-  Autorepeat for SW
		-  Output 7 segments 1 digit

 Try		-  Adjust delay time what happen
		-  Remove delay for key bounce
 
 Improve
		-  Cancel autorepeat mode 
		-  More SW status  Press, Hold
		-  Hold 2 keys simultaneous
		
 Update History
 
 Date			By 					Comments
 ----			--					---------

*/
/* Display numeric 0-9  on  digit 0 or 1 
  
	INPUT
	num		:	number to show
	digit 	: 	Digit  to show
	
	OUTPUT	:	null
	
*/  

#include <stdio.h>
#include "Dual_7Segment.h"

// Display number 0 - 9

void d7segment_display( int8_t num)
{
 switch (num)
 {
	case 0:
		_7SEGMENT_PORT = _7SEGMENT_0;
		break;
	case 1:
		_7SEGMENT_PORT = _7SEGMENT_1;
		break;
	case 2:
		_7SEGMENT_PORT = _7SEGMENT_2;
		break;
	case 3:
		_7SEGMENT_PORT = _7SEGMENT_3;
		break;
	case 4:
		_7SEGMENT_PORT = _7SEGMENT_4;
		break;
	case 5:
		_7SEGMENT_PORT = _7SEGMENT_5;
		break;
	case 6:
		_7SEGMENT_PORT = _7SEGMENT_6;
		break;
	case 7:
		_7SEGMENT_PORT = _7SEGMENT_7;
		break;
	case 8:
		_7SEGMENT_PORT = _7SEGMENT_8;
		break;
	case 9:
		_7SEGMENT_PORT = _7SEGMENT_9;
	    break;
	case 'f':
	    _7SEGMENT_PORT = _7SEGMENT_C_F;
		break;
	case 'b':
	    _7SEGMENT_PORT = _7SEGMENT_C_B;
		break;
	case DOT:
		_7SEGMENT_PORT = _7SEGMENT_DOT;
		break;
 
 }
// Output  digit
// ON Digit 1 OFF  ( 1 Off  0 On)
	   _7SEGMENT_DIGIT_PORT &= ~(_7SEGMENT_DIGIT1);
// OFF Digit 2 
	   _7SEGMENT_DIGIT_PORT |= _7SEGMENT_DIGIT2;
 
}	

void delay ( uint16_t time)
{
   volatile uint16_t  t1,t2;   // Prevent optimizer skip command
   for (t1 = 0 ; t1 < time ; t1++)
   {
	   for (t2 = 0 ; t2 < 3000 ;t2++);		
   }
}

void main (void)
{
   uint8_t sw1,sw2;
   int8_t digit = 0;

   
   
// Initial value for E_IO  7 Segment as output and SW as input

	_7SEGMENT_DIGIT_DIR   = 0xFF;  // Set direction for output all to 1 for 7 Segment     
	_7SEGMENT_PORT_DIR	  |= _7SEGMENT_DIGIT1 | _7SEGMENT_DIGIT2; // Set output for 7 segments digit
 
// Set direction for input for SW 
	_7SEGMENT_SW1_DIR	&=  ~(_7SEGMENT_SW1);   // Inverse to 1 except SW1
	_7SEGMENT_SW2_DIR	&=  ~(_7SEGMENT_SW2);   // Inverse to 1 except SW2

// Read value from SW, out 1 to input port for sync and nop 1 clock before read
	 _7SEGMENT_SW1_PORT   |= _7SEGMENT_SW1;  // Output 1 to port SW1, SW2 for enable internal pull up resisters
     _7SEGMENT_SW2_PORT   |= _7SEGMENT_SW2;


// Enable pull up set PUD in SFIOR default = 0??
   
     
  while (1)
  {
// NOP     
	 sw1 =  _7SEGMENT_SW1_IN_PORT;          // Get status for ALL input port 
	 sw1 &=  (_7SEGMENT_SW1);				// Mask data for SW1
	 sw2 =  (_7SEGMENT_SW2_IN_PORT);      
     sw2 &=  (_7SEGMENT_SW2);
 
	
	 	
     if (sw2 == 0)  // Press and release SW
	 {
	    delay(100);     // Protect Key bounce
	    if (digit < 9)
			digit++;
	 }
	   
	 	
     if (sw1 == 0)  // Press and release SW
	 {
	    delay(100);     // Protect Key bounce
	    if (digit > 0)
			digit--;
	 }
 
// 	 
  	d7segment_display(digit);
  
  };
}


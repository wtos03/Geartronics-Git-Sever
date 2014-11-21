/*

 Module 				:   Dual_7Segment common  cathode
 Description 			:   Dual_7Segments common cathode
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   Dual 7 Segments common cathode
						:   2 Button Switch ( Press on release off)
 Port use				:   E_PORTA (0-7), E_PORTB (1-2)  7 Segments
						:   E_INT1 (SW1)  E_PB0 (SW2)
						
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Wichai  wichai@geartronics.net
 Date					:  	23/04/2006
  
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

#include "Dual_7Segment.h"

void d7segment_display( int8_t num, uint8_t digit)
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
	case '-':
	    _7SEGMENT_PORT = _7SEGMENT_DASH;
		break;
	case DOT:
		_7SEGMENT_PORT = _7SEGMENT_DOT;
		break;
 
 }
// Out digit
	 
    if ( digit == 1 )
	{  // ON Digit 1 OFF  ( 1 Off  0 On)
		_7SEGMENT_DIGIT_PORT &= ~(_7SEGMENT_DIGIT1);
	   // OFF Digit 2 
	   _7SEGMENT_DIGIT_PORT |= _7SEGMENT_DIGIT2;
	}
	if ( digit == 2 )
	{  // ON Digit 2 OFF  ( 1 Off  0 On)
		_7SEGMENT_DIGIT_PORT &= ~(_7SEGMENT_DIGIT2);
	   // OFF Digit 1
	   _7SEGMENT_DIGIT_PORT |= _7SEGMENT_DIGIT1;
	}	

}	


/* Initial 7 Segments. Set port to output before use 
*/
void d7segment_init (void)
{
// Set direction for output for 7 Segment
	_7SEGMENT_DIGIT_DIR   = 0xFF;
	_7SEGMENT_PORT_DIR	  |= _7SEGMENT_DIGIT1 | _7SEGMENT_DIGIT2;
 
// Set direction for input for SW 
	_7SEGMENT_SW1_DIR	&=  ~(_7SEGMENT_SW1);
	_7SEGMENT_SW2_DIR	&=  ~(_7SEGMENT_SW2);

// Enable pull up
    _7SEGMENT_SW1_PORT   |= _7SEGMENT_SW1;
    _7SEGMENT_SW2_PORT   |= _7SEGMENT_SW2;
	
}

/*
 Program 			    :   lab6-2.c
 Description 			:   Electronics Dices
 Specification			:   Press Sw1 or SW2 The number will run from 0 - 12  
						:   when release the number will run for a while and stop randomly
 CPU port/tested		:   ATMEGA32
 Hardware use			:   E_Mega32, E_IO Module
 Port use				:   Port name / Function

 Written by 			:  wichai@geartronics.net
 Date					:  4/Sep/2007
  
 What we learn
		-  Keypress and Keyrelease event
		-  Display 2 digits
		-  Multiplex display for 7 Segments
		

 Try	-  Adjust delay between d7segment_display
		-  Adjust trig_delay
 
 Improve
		-  Use random time 
		-  blank display for digit2 when show 0 ex 02 show 2
		-  Time to scroll longer and longer until stopped
		
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
	case DOT:
		_7SEGMENT_PORT = _7SEGMENT_DOT;
		break;
 
 }
// Output  digit
   if (digit == 1)
   {
// ON Digit 1    ( 1 Off  0 On)
	   _7SEGMENT_DIGIT_PORT &= ~(_7SEGMENT_DIGIT1);
// OFF Digit 2 
	   _7SEGMENT_DIGIT_PORT |= _7SEGMENT_DIGIT2;
    }

   if (digit == 2)
   {
// ON Digit 2   ( 1 Off  0 On)
	   _7SEGMENT_DIGIT_PORT &= ~(_7SEGMENT_DIGIT2);
// OFF Digit 1 
	   _7SEGMENT_DIGIT_PORT |= _7SEGMENT_DIGIT1;
    }

	
}	

// Delay for display routine
void delay ( uint16_t time)
{
   volatile uint16_t  t1,t2;   // Prevent optimizer skip command
   for (t1 = 0 ; t1 < time ; t1++)
   {
	   for (t2 = 0 ; t2 < 300 ;t2++);		
   }
}



void main (void)
{
   uint8_t sw1,sw2;
   int8_t num =0,digit1 = 0, digit2 =0;
   uint8_t  key_release,time = 0;
   uint8_t trig_delay = 0;
   uint8_t toggle =  0;
   
   
// Initial value for E_IO  7 Segment as output and SW as input

	_7SEGMENT_DIGIT_DIR   = 0xFF;  // Set direction for output all to 1 for 7 Segment     
	_7SEGMENT_PORT_DIR	  |= _7SEGMENT_DIGIT1 | _7SEGMENT_DIGIT2; // Set output for 7 segments digit
 
// Set direction for input for SW 
	_7SEGMENT_SW1_DIR	&=  ~(_7SEGMENT_SW1);   // Inverse to 1 except SW1
	_7SEGMENT_SW2_DIR	&=  ~(_7SEGMENT_SW2);   // Inverse to 1 except SW2
	
// Read value from SW, out 1 to input port for sync and nop 1 clock before read
    _7SEGMENT_SW2_PORT   |= _7SEGMENT_SW2;
	 _7SEGMENT_SW1_PORT   |= _7SEGMENT_SW1;  // Output 1 to port SW1, SW2 for enable internal pull up resisters

// Test for piezo
     DDRD = 0xFF; 
	 
// Enable pull up set PUD in SFIOR default = 0??
   
     
  while (1)
  {

// NOP     
	 sw1 =  _7SEGMENT_SW1_IN_PORT;          // Get status for ALL input port 
	 sw1 &=  (_7SEGMENT_SW1);				// Mask data for SW1
	 sw2 =  (_7SEGMENT_SW2_IN_PORT);
     sw2 &=  (_7SEGMENT_SW2);
 
	key_release  = 1;                       // Flag for key release
	 	
     if ((sw1 == 0) || (sw2 == 0))  // Press  SW
	 {
	   	if  (trig_delay  == 0 )
		{
			trig_delay = 2;     				// Delay for number scroll
			time++; 							// time to scroll after key release
			if ( time >  15)
				time = 3;						// Min = scroll another 3 when release key
			num++;								// Increase number when key press and end of trig delay loop
		}		
		key_release = 0;
	
	

	}

	
	 if (key_release && time)                  // Key release detect and time to scroll after key release > 0
	 {
 		if  (trig_delay  == 0 )
		{
			trig_delay = 10;				    //scroll number slower than  pressed key scroll
			time--;
			num++;
		}	
	 }		
	 
// Loop for scroll number stopped when 0	 
	if ( trig_delay > 0)
	{
	    trig_delay--;
	}
	
// Calculate digit to display	
	if (num > 12)
		num = 0;	  	   
	digit2 = num/10;    // Find second digit
	digit1 = num%10;    // Find first digitdelay_trig--;
   
// Display 2 digit   
  	d7segment_display(digit1,2);
		toggle ^= 1;
		if (toggle)
		    PORTD = 0;
		else
			PORTD  =  0x80;
//	delay(4);                     // Delay to show 2 digit clearly
 	d7segment_display(digit2,1);
	delay(1);
	
  
  };
}


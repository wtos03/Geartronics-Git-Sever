/* 
 Module 				:   Infrared Detecting Module  IR TV Sony Remote control driver
 Description 			:   Driver for IR Remote control for Sony TV 
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   TSOP4838
 Port use				:	ICP 	(input capture 16 bit)
 Document				:
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   21/3/2006
 
 
 Update History
 
 Date			By 					Comments
 ----			--					---------
 28/3/2006		Wichai				Initial version
 
 
 
*/
#include <avr/interrupt.h>
#include "ir_tv.h"
#include "Dual_7Segment.h"

 

void init_ir()
{
 //Set Direction
    IR_DDR_PORT &= ~IR_ICP; 
	
	TCCR1B  |= ( _BV(ICNC1) | _BV(CS12) | _BV(CS10));			// Time 1 input to clock/1024 input capture on falling edge  noise cancel
	TIMSK   |= _BV(TICIE1);   								// Enable input capture enable 
	sei();
}
/*  Decode for Sony remote TV IR 
			Input  Input capturn counter 16 bit register
		   Output  0  = no code detect
				   ir_decode  = detect IR code 
				   
	Algorithm
		Calculate pulse width by substract ICP counter. from pulse width 
		can determine whether it is  START BIT 		 =  XX
									  BIT1 			 =  XX
		until 12 bits then return ir code							  
									 
*/

uint16_t sony_ir_decode ( uint16_t capture_counter)
{
  static 	uint16_t  	prev_capture_counter = 0;
  static 	uint8_t   	stop_bit = 0;
  static	uint16_t	ir_code = 0;
 			uint16_t	width  = 0;	  

      if ( capture_counter > prev_capture_counter)
			width = capture_counter - prev_capture_counter;
	  else
			width = capture_counter+(MAX_INT16 - prev_capture_counter);
//Check for start bit
     if ( width > IR_TV_PULSE_WIDTH)
	 {
		  if (width > IR_TV_START_BIT)  			//  Found Start bit 
			    {
				  stop_bit = 0;
				  prev_capture_counter = 0;
				  ir_code = 0;     			// New code
				}
		  else if (width > IR_TV_BIT1)	  		// bit 1 (3T)
				{
				   ir_code <<= 1;    			//Shift right
				   ir_code  |= 1;    			// Or 1
				    stop_bit++;
				}
		        else 	  // bit 0
				{
					ir_code  <<=  1;  //Shift right
					stop_bit++;
				}
	}
	prev_capture_counter = capture_counter;
	if (stop_bit == IR_TV_STOP_BIT)
	{       //End code dispatch command
  	    stop_bit = 0;

		return ir_code;
			
//Note if  SELFTEST is selected g_irint = 0;
	}   
	else
		return 0;		// No code detect yet
}

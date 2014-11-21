/*

 Module 				:   IO Test Module
 Description 			:   IO  Test module for CHapter xxx
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   Dual 7 Segments common cathode module
 Port use				:   E_PORTA (0-7), E_PORTB (0-1)
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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Dual_7Segment.h"
#include "DC_motor.h"
#include "ir_tv.h"

// Global variable

uint8_t  g_mode1  = MOTOR_STOP;
uint8_t  g_mode2  = MOTOR_STOP;

void main(void);

void delay ( uint16_t  time)
{
   uint16_t i;
   for (i = 0 ; i < time; i++);

}


/* This subroutine will handle IR remote decode by getting remote code from Interrupt handle for input capture
and will response  to each remote key. For other applications user can modified here for different remote active
However this routine support Sony remote control only
   global effect   g_irint;
*/

uint8_t remote_decode ( uint16_t ircode)
{ 
  uint16_t pwm;
  uint8_t speed, found = 1;
  speed = 10;
  switch ( ircode)
	   {
	    case IR_TV_1:	
			speed = 2;
			 break;
		case IR_TV_2:
			speed = 3;
			 break;
	    case IR_TV_3:
			speed = 4;
			 break;
		case IR_TV_4:
			speed = 5;
			 break;
	    case IR_TV_5:
			speed = 6;
			 break;
		case IR_TV_6:
			speed = 7;
			 break;
	    case IR_TV_7:
			speed = 8;
			break;
		case IR_TV_8:
			speed = 9;
			break;
	    case IR_TV_9:
			speed = 10;
			break;
		case IR_TV_0:
			g_mode1  = MOTOR_STOP;
			g_mode2  = MOTOR_STOP;
			break;
		case IR_TV_CHUP:							
        	g_mode1 = MOTOR_FORWARD;		
			g_mode2 = MOTOR_FORWARD;
			break;
		case IR_TV_CHDOWN:							
			g_mode1 = MOTOR_BACKWARD;
			g_mode2 = MOTOR_BACKWARD;
			break;
		case IR_TV_TVMUTE:
			break;
		case  IR_TV_TVPOWER:
		  	break;
		case   IR_TV_TVAV:
		    break;
		case  IR_TV_VOLUP:
            g_mode1 = MOTOR_FORWARD;		
			g_mode2 = MOTOR_BACKWARD;
			break;
		case  IR_TV_VOLDOWN:                 			
			g_mode1 = MOTOR_BACKWARD;
			g_mode2 = MOTOR_FORWARD;
			break;
		default:
			found = 0;
			g_mode1  = MOTOR_STOP;
			g_mode2  = MOTOR_STOP;
			break;
 	   }
       if (found)
	   {	pwm = (speed * 100);             // Start from +  speed*2^4
			OCR1A = pwm;
			OCR1B = pwm;
		}	
	   return found;

}


 
 
/* Interrupt handle for Input Capture
This routine will detect/ process the Sony IR remote command. Protocol using Sony remote control
INPUT   :  Infrared signal from  ICP pin
		   Command table 
		  

OUTPUT 	: g_num  Number of message to display
		: g_control.display_mode   Display mode of message
		: g_start_bit
		

*/
SIGNAL(SIG_INPUT_CAPTURE1)
{
	uint16_t  ir_code;
    ir_code = ICR1;
	cli(); 

    TCCR1B  |= _BV(ICES1);	     // Rising edge
 	ir_code  = sony_ir_decode(ir_code);  

	if (ir_code)
	{       //End code dispatch command    
 
		remote_decode (ir_code);
// Return to falling edge for Start		
		TCCR1B  &= ~(_BV(ICES1));

//Note if  SELFTEST is selected g_irint = 0;
	} 
    sei();
 
}


void main()
{
  uint8_t sw1,sw2;
  uint16_t k;
  int8_t d1,d2;
  uint8_t show[3] = {0,'f','b'};
  
  
//  enum {MOTOR_STOP,MOTOR_FORWARD,MOTOR_BACKWARD,MOTOR_BRAKE};
  d7segment_init();
  init_ir();
  init_motor();
  sei();
  d1 = 0;
  d2 = 0;
  
  while (1)
  {
   sw1 = _7SEGMENT_SW1_IN_PORT & _7SEGMENT_SW1; 
   
   if (!sw1) // SW1  Press
    {   	
		d1++;
	     if (d1 > MOTOR_BACKWARD)
			d1 = MOTOR_STOP;
		for (k = 0; k < 500 ;k++)                         // Delay for key bounce
				delay(6000);						
		g_mode1 = d1;

	}
   
   sw2 = _7SEGMENT_SW2_IN_PORT & _7SEGMENT_SW2; 
   if (!sw2) // SW2 Press
   {
        d2++;
		if (d2 > MOTOR_BACKWARD)
			d2 = MOTOR_STOP;
 		for (k = 0; k < 500 ;k++)                         // Delay for key bounce		
 				delay(6000);
		g_mode2 = d2;
	
   }

  	motor_control(1, g_mode1);
	motor_control(2, g_mode2);
	
//  		for (k = 0; k < 10 ;k++)                         // Delay for key bounce
//			delay (30000);
//		motor_control (1, MOTOR_STOP);

   	d7segment_display(show[g_mode1],1);
 			delay(12000);
	d7segment_display(show[g_mode2],2);
 			delay(12000);
		
    
  }
     
}



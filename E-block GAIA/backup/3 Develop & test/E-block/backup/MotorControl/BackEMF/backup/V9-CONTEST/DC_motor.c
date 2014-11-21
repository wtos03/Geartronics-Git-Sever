/*
 DC Motor control
 

 Module 				:   DC_motor
 Description 			:   Control H-bridge for BDC
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   SN754410
 Port use				:   Port name / Function
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:  wichai / wichai@geartronics.net
 Date					:  22/06/2006
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 */
 
#include "DC_motor.h" 
#include <avr/io.h>

/*
 Freq = Clk/256 = 62.5 K If too high Motor cannot start
 PWM Mode 8 bits
*/ 
void init_motor ()
{
 // Initial for PWM Motor control for TIMER0 & 2 
 

//   TIMER0
//     TCCR0  |= _BV(WGM00) |_BV(WGM01) | _BV(CS01) | _BV(CS00)  ;   // Clk/64 Fast Pwm
    TCCR0  |= _BV(WGM00) |_BV(WGM01) | _BV(CS02)    ;   // Clk/256 Pwm
    OCR0    = 0x80;
	
	
//   TIMER2
//     TCCR2  |= _BV(WGM20) | _BV(CS22) ;   // Clk/64 Pwm
     TCCR2  |= _BV(WGM20) |_BV(WGM21) | _BV(CS22) | _BV(CS21) ;   // Clk/256 Pwm
      OCR2   = 0x80;
	
	
	
// Enable Interrupt 	Watch out nest interrupt 
	TIMSK  |=  _BV(OCIE0) | _BV(TOIE0) | _BV(OCIE2) | _BV(TOIE2);	
 
    MOTOR_DIRECTION_DIR  |= 	MOTORA_1 | MOTORA_2 | MOTORB_1 | MOTORB_2;
	MOTOR_ENABLE12_DIR	 |= 	MOTOR_ENABLE12;
	MOTOR_ENABLE34_DIR	 |=  	MOTOR_ENABLE34;		
}


// speed from 1 - 10

void  motor_control (uint8_t port, uint8_t direction) 
{
  
   uint8_t  controlA,controlB;
   
   switch (direction)
   {
     case MOTOR_FORWARD:
	    controlA = 0;
		controlB = 0xFF;
		break;
	 case MOTOR_BACKWARD:
        controlA = 0xFF;
		controlB = 0;
		break;
	 case MOTOR_STOP:
	    controlA = 0;
		controlB = 0;
		break;
	default:
	    controlA = 0;
		controlB = 0;
   }
 if (port == 1)
   {
//       MOTOR_ENABLE12_CONTROL  |= MOTOR_ENABLE12;
	  MOTOR_DIRECTION_CONTROL &= ~(MOTORA_1 | MOTORA_2); 
	  MOTOR_DIRECTION_CONTROL |= ((MOTORA_1 & controlA ) | (MOTORA_2 & controlB));
  }
  if (port == 2)
   {
//  	  MOTOR_ENABLE34_CONTROL  |= MOTOR_ENABLE34;
	  MOTOR_DIRECTION_CONTROL &= ~(MOTORB_1 | MOTORB_2); 
  	  MOTOR_DIRECTION_CONTROL |= ((MOTORB_1 & controlA ) | (MOTORB_2 & controlB));
	   
   }
   
}

/*
 Template for program header 
 

 Module 				:   Module name
 Description 			:   Module Description
 Original written for 	:   CPU/Microcontroller
 CPU port/tested		:   CPU/Microcontroller
 Hardware use			:   IC Chip
 Port use				:   Port name / Function
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Name / Email
 Date					:  DD/MM/20XX
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 */
 
#include "DC_motor.h" 
#include <avr/io.h>

/*
 Freq = Clk/1024 = 15.625 K If too high Motor cannot start
 PWM Mode 8 bits
*/ 
void init_motor ()
{
 // Initial for PWM Motor control for 16 timer for OC1A and OC1B
// 8 bit fast PWM set OC1A OC1B on comapare match
    TCCR1A |= _BV(WGM10) |_BV(WGM11) | _BV(COM1B1)|_BV(COM1A1);
    TCCR1B |= _BV(WGM12) | _BV(CS12) | _BV(CS10);   // Clk /1024
	OCR1A = 0x00FF;
	OCR1B = 0x00FF;
//    OCR1A = 0;
//	OCR1B = 0;
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
	case MOTOR_BRAKE:
	    controlA = 0xFF;
		controlB = 0xFF;
		break;
	default:
	    controlA = 0;
		controlB = 0;
   }
   if (port == 1)
   {
//      MOTOR_ENABLE12_CONTROL  |= MOTOR_ENABLE12;
	  MOTOR_DIRECTION_CONTROL &= ~(MOTORA_1 | MOTORA_2); 
	  MOTOR_DIRECTION_CONTROL |= ((MOTORA_1 & controlA ) | (MOTORA_2 & controlB));
  }
   if (port == 2)
   {
// 	  MOTOR_ENABLE34_CONTROL  |= MOTOR_ENABLE34;
	  MOTOR_DIRECTION_CONTROL &= ~(MOTORB_1 | MOTORB_2); 
  	  MOTOR_DIRECTION_CONTROL |= ((MOTORB_1 & controlA ) | (MOTORB_2 & controlB));
   }
}

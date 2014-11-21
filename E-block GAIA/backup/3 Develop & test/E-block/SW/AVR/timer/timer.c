 /*
 

 Module 				:   Timer
 Description 			:   Set timer, Counter mode
 Original written for 	:   ATMEGA32
 CPU port/tested		:   CPU/Microcontroller
 Hardware use			:   ATMEGA32-16 (TQFP)
 Port use				:   Timer0, Timer1, Timer2 
						:   OCn, OCnA, OCnB
						
 Memory	FLASH	:   XXX
		RAM		:   XXX
		EEPROM	:   XXX
 Document				:   Document describe algorithm
 Written by 			:  Wichai Tossamrtvorakul / wichai@geartronics.net
 Date					:  19/05/2006
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
 */
 
 
 
 #include <avr/io.h>
 #include "timer.h"
 
 
 // Global variable for timer mode in each timer
 
 
 
// Register pointer
 
 
 
 void main (void);
 
 
 // Set correct register for each timer 
  
 
 // Set timer mode for  timer 0, 1, 2
uint8_t  timer_init_mode ( uint8_t timer, uint8_t mode)
 {
 
    uint8_t status = 1;
	uint16_t  tmp;
	uint8_t mix;
	switch (timer| mode)
	{
	  case (TIMER0 |TIMER_COUNTER_MODE ):
	  TCCR0  &= 0xB7;
	  break;
	  
	  case  (TIMER0 |TIMER_CLEAR_TIMER_MODE):
	  TCCR0 &=  0xBF;
	  TCCR0 |=  0x08;
	  break;
	
	  case (TIMER0 |  TIMER_FAST_PWM_MODE):
	  TCCR0  |=  0x48;
	  break;
	 
	  case (TIMER0 |  TIMER_PHASE_PWM_MODE):
	  TCCR0 &=  0xF7;
	  TCCR0 |=  0x40;
	  break;
	
	  case (TIMER1A |TIMER_COUNTER_MODE ):
	    
	  break;
	  
	  case  (TIMER1A |TIMER_CLEAR_TIMER_MODE):
	  
	  break;
	
	  case (TIMER1A |  TIMER_FAST_PWM_MODE):
	  break;
	 
	  case (TIMER1A |  TIMER_PHASE_PWM_MODE):
	  break;
	  
	   case (TIMER1B |TIMER_COUNTER_MODE ):
	  
	  break;
	  
	  case  (TIMER1B |TIMER_CLEAR_TIMER_MODE):
	  
	  break;
	
	  case (TIMER1B |  TIMER_FAST_PWM_MODE):
	  break;
	 
	  case (TIMER1B |  TIMER_PHASE_PWM_MODE):
	  break;
	   case (TIMER2 |TIMER_COUNTER_MODE ):
	  
	  break;
	  
	  case  (TIMER2 |TIMER_CLEAR_TIMER_MODE):
	  
	  break;
	
	  case (TIMER2 |  TIMER_FAST_PWM_MODE):
	  break;
	 
	  case (TIMER2 |  TIMER_PHASE_PWM_MODE):
	  break;
	  
	  default:
	    status = 0;   // Error set mode  
	 
	}
	
 return status;
 
 }
 
 void timer_set_clock ( uint8_t timer,uint16_t freq)
 {
	 
 
 }
 
 void  main ()
 {
 
 
 
 }
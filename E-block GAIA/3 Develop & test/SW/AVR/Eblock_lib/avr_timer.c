/*!  
	\file   	avr_timer.c
	\brief  	Setting timer for AVR
	\ingroup	avr_timer
	\par	This module initial the timer for avr 
	\n		TIMER0  use for internal tick for E_IO Module SW and  Display 7 Segments
	\n		TIMER1  use for	ICP mode, IR detecting
	\n		TIMER2  use for Piezo
	\n		Other timer mode will be added
 	\n Original written for :   \b AVR
 	\n CPU port/tested		: 	ATMEGA32
 	\n Hardware use			:   E_MEGA32
	\n Peripheral use		:   TIMER2,TIMER0, TIMER1
 	\n Library use			:   x
 	\n Memory	FLASH		:   XXX
 	\n			RAM			:   XXX
 	\n		EEPROM			:   XXX
 	\n Reference Document	:   ATMEGA32 Datasheet

	\version 0.1
 	\author  wichai tosssamartvorakul / wichai@geartronics.net
	\date 	 13/08/08  

	\par Update History
 	\nDate			By 					Comments
 	\n----			--					--------
	\n 
 
*/

 

//! Include file
#include <avr/interrupt.h>
#include "avr_timer.h"



//  Start routine here

/*!	\brief  Initial timer for operation mode
	\param  timer no    0,1,2
	\n		timer mode  Mode of opertaion
	\n	    output mode Output mode to OCn pin
	\n		Clock source   From prescale interna clock or external
	\return None
	\par Port/Peripheral Usage   TIMER
	\n    
	\par Register setting
	\n	 TCCR0    
	\par Theory of Operation   
	\n   Setting  timer mode, output pin  and clock source for each timer 
	\n   Mode of operations
	\n	 NORMAL_MODE  	Count from 0 to 0xFF and restart again Can update TCNTn to set the timer tick
	\n   CTC_Mode     	Count from 0 until reach OCRn and restart again
	\n   FAST_PWM_MODE	Create PWM with counting  0 - MAX (Out 1) - TOP  (Out 0)  Reset to 0 
	\n   PWM_MODE       Create PWM with counting  MAX- 0 - MAX  ( Out 1)  MAX - TOP - MAX (Out 0)
	\par Pseudo code
	\n
	\note   (Pseudo code)
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/          

void set_timer_mode (uint8_t timer, uint8_t mode, uint8_t output_mode, uint8_t clock)
{
// Set mode to 

   switch (timer)
   {
      case TIMER0:
		TCCR0 = ( mode | output_mode |clock);
	    break;
	  case TIMER2:
	    TCCR2 = ( mode | output_mode |clock);
		break;
	  case TIMER1A:
	    break;
	  case TIMER1B:
	    TCCR1B  = ( mode | output_mode |clock);			 
		break;
	  default:
	    break;
   
   }
 
}

 

/*!	\brief  Initial timer tick for using in E_IO
	\n   This setting will effect the refresh rate of 7 Segments
	\param  Value  Value for timer tick   ~ (0.128*value + 0.12)  ms
	\return None
	\par Port/Peripheral Usage   TIMER
	\n    
	\par Register setting
	\n	 TCNT0  
	\par Theory of Operation   
	\n   Set timer0 to counter mode and disable the Output. Because we want to use
	\n   just timer to tick the routine tasks. Ex. Update display 
	\n   Mode of operations
	\n   CTC_Mode     	Count from 0 until reach OCRn and restart again
 	\n   	timer tick  = 2.n.(1+OCRn)/fclk    n = Prescale  fclk = 16 Mhz
	\n      timer tick  = 0.128 (1+0CRn)  ms
	\n
	\par Pseudo code
	\n
	\note   Need to Add   
	\n		SIGNAL(SIG_OUTPUT_COMPARE0)  into main source code

	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/          

void timer_tick (uint8_t value)
{
// Set mode to 
  set_timer_mode (0,CTC_MODE,OC_DISCONNECT, CLK_BY_1024);
  OCR0 = value;
 // Enable interrupt Flag
  TIMSK  |= _BV(OCIE0);
 // Enable interrupt Set i bit in the status register
  sei();   

}

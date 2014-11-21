/*!  
	\file   	E_io_piezo.c
	\brief  	Piezo driver on E_iO block
	\ingroup	E_io_piezo
	\par  Create square wave form with vary Frequency,duty cycle and time length.
    \n Start with tab +\n	
 	\n Original written for :   \b for bold ATMEGA32
 	\n CPU port/tested		:   ATMEG32
 	\n E-block use			:   Piezo on E_IO
 	\n Port use				:   TIMER0 pin  (OC2)  Timer2 
 	\n Memory	FLASH		:   XXX
 	\n		RAM		:   XXX
 	\n		EEPROM		:   XXX
 	\n Reference Document		:   Document describe algorithm

	\version xxx.yyy
 	\author  Name / Email
	\date 	 DD/MM/20XX  

	\par Update History
 	\nDate			By 					Comments
 	\n----			--					--------
	\n 
 
*/






// Include file
#include <avr/interrupt.h>
#include "E_io_piezo.h"
#include "avr_timer.h"




// Start routine here



/*!	\brief  Initial timer for using on piezo
	\param  none	  
	\return none
	\n		   
	\par Port/Periheral Usage 
	\n   E_TIMER0   ( OC2 for timer2 on ATMEGA32)
	\par Theory of Operation   
	\n   Initial timer2 for generate Frequency in Melody notes between 27 Hz - 4 Khz 
	\n   or  36.3 ms - 0.24 ms Period. Adjust the duty cycle will adjust the sound level
	\n   Then the time mode shoud use Fast PWM
	\n	 
	\par Pseudo Code
	\note   To correct debounce by allow return value more than debouce interval ex 10 - 100
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/       

void io_piezo_init (void)
{
   E_DIR_TIMER0(OUTPUT);	  						// Need to set E_TIMER0 as output ****
}


/*!	\brief  Setting tone time and volumn on 
	\param  freq	Sound Frequency 	(0 -6636)
 	\n 		volumne Adjust volumn by adjust duty cycle    (1-100)
	\return  PWM signal at E_TIMER0
	\par Port/Periheral Usage 
	\n   E_TIMER0
	\par Theory of Operation   
	\n   Music Notes are represented by Frequency  as the following
	\n  	c   261 Hz 
	\n		d   294 Hz 
	\n		e	329 Hz 
	\n		f	349 Hz 
	\n		g	392 Hz 
	\n		a  	440 Hz 
	\n		b   493 Hz 
	\n		C   523 Hz 
	\n   We can control the frequency and volumn by controll the following parameters
	\n		 TCNT2				-  Timer counter max 
	\n		 Clock presclae		-  Clock prescale from Internal clock source
	\n		 OCN2				-  For duty cycle which effect the loudness
	\n   Formula to calculate frequency   = Fclk / 2*N*(1_OCR2)   N = Prescale for CTC Mode
 	\par Pseudo Code
	\note  PWM and FAST PWM can change duty cycle. But not frequency. Use CTC_MODE for changing
	\n	   Frequency
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/

void io_piezo_write ( uint8_t freq )
{
// Stop the timer before set all parameters	
	set_timer_mode (TIMER2,CTC_MODE,TOGGLE_OC, STOPPED_CLOCK);

   if (freq)  // Play sound if sound != 0
	{   
 		OCR2  =  (freq) ;// period;
		set_timer_mode (TIMER2,CTC_MODE,TOGGLE_OC, CLK_BY_256);
	}
}



/*!  
	\file   	avr_adc.c
	\brief  	Analog to digital module
	\ingroup	avr_adc
	\par  		Convert analog voltage to Digital value 
	\n			There is two modes Polling using adc_read() and interrupt
	\n			using adc_int_read()  Need to call once to set ADC channel
	\n			Before initialize interrupt using adc_init()
 	
 	\n Original written for :   \b for bold ATMEGA32
 	\n CPU port/tested		:   ATMEGA32
 	\n Hardware use			:   E_Mega32
 	\n Port use				:   ADC0 - ADC7
 	\n Memory	FLASH		:   XXX
 	\n		RAM				:   XXX
 	\n		EEPROM			:   XXX
 	\n Reference Document		:   Document describe algorithm

	\version 0.1
 	\author  Wichai / wichai@geartronics.net
	\date 	16/09/2008

	\par Update History
 	\nDate			By 					Comments
 	\n----			--					--------
	\n 
 
*/






//! Include file
#include <stdlib.h>
#include <avr/interrupt.h>
#include "avr_adc.h"
#include "avr_serial.h"

//! Global variable

uint16_t g_value;    // ADC value from interrupt read


// Start routine here


/*!	\brief  Initial ADC Module
	\param  mode	Mode of operation and Clock prescale for division
	\n		Voltage reference for ADC
	\n			- 0 = AREF pin
	\n			- 1 = AVCC
	\n			- 3 = Internal 2.56 v
	\n		Interrupt Enable  Disable
	\n			DISABLE	Disable Interrupt
    \n    		ENABLE 	Enable Interrupt
	\return None
	\par Port/Periheral Usage 
	\n   ADC0 - ADC7
	\par Theory of Operation   
	\n   To start interrupt enable  ADIE must be set
	\n   Interrrupt mode will turn on auto trigger mode 
	\n	 default mode is TIMER0 COMAPRE MATCH which using by Display 7 segments 
	\n	 (free running mean no trigger) to change mode set ADTS bits in SFIOR
	\n   To set ADSC  the ADC module will start conversion immediately 
	\n	
	\par Pseudo code
	\n   Set register ADMUX, ADCSRA depend on mode
	\note  If interrupt enable Need to use ??? to get value from ISR 
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     

void adc_init(uint8_t mode, uint8_t voltage, uint8_t enable)
{
  
// Set voltage reference 	
	ADMUX  =  voltage;
 
	if (enable)
	{
		mode  |= ((1 << ADATE) | (1 << ADIE) | (1<< ADSC) ); // Enable interrupt and start conversion in auto trigger mode
        SFIOR |= ((1 << ADTS1) | (1 << ADTS0)); // Trigger on timer0 compare match
	}
	else
	{
		mode  &= ~((1 << ADIE) | (1<< ADSC));  // Disable interrupt and stop conversion 
	}
	ADCSRA = mode;	
}
 
 


/*!	\brief  read ADC on specific channel
	\param  channel  to read adc  ( See table for Differential INput and Gain)
	\return None
	\par Port/Periheral Usage 
	\n  ADC0 -ADC7
	\par Theory of Operation   
	\n   Convert ADC and wait to finished  
	\par Pseudo code
	\n   - Set channel
	\n   - Start conversion
	\n	 - Wait to finish
	\n	 - Stop conversion
	\note   
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     

uint16_t adc_read(uint8_t channel)
{
  int16_t  value;
  
// Before that need to clear ADATE and ADEN
    ADCSRA &= ~((1 << ADEN) | (1<< ADATE));

// Set channel
    ADMUX |= channel;
	
// Enable and start conversion
	ADCSRA |= ((1<<ADEN) |(1<<ADSC)) ;

// Start conversion    
//       ADCSRA |= (1<<ADSC);        // do single conversion

//  while(!(ADCSRA & (1<<ADIF)));    // wait for conversion done, ADIF flag active  (Interrupt)
	while(ADCSRA & (1 << ADSC));		// wait until conversion complete (Normal COnversion)
        
//Read value
 
	value = ADCW;

// Stop conversion
    ADCSRA &= ~(1 << ADEN);
	return value;
}



/*!	\brief  	Read ADC using Interrupt
	\param  	channel  to read adc  ( Use to set channel for using interrupt ADC)
	\return 	ADC Converted value
				0  No value or 0 read
	     
	\par Port/Periheral Usage 
	\n  ADC0 -ADC7
	\par Theory of Operation   
	\n
	\par Pseudo code
	\n   	- Set channel
	\n		- Get value from interrupt routine (g_value) if > 0 clear for new coming value
	\n		  and return the vaule 
	\note  This routine need to call once to set channgel before call adc_init to initial interrupt
	\n		How to differentiate value 0 and Convert not finished 
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/  
uint16_t adc_int_read(uint8_t channel)
{
   uint16_t tmp = 0;

// Set channel
    ADMUX |= channel;
	   
   if (g_value)   
   {
		tmp = g_value;
		g_value = 0;   // Clear value for next read
   } 
   return tmp;
}



/*!	\brief   ISR for ADC conversion
	\param  	 
	\return 	ADC Converted value put in global variable
				 
	     
	\par Port/Periheral Usage 
	\n  ADC0 -ADC7
	\par Theory of Operation   
	\n    If ISR is called mean the ADC is complete save the value in global variable
	\n	  g_value for procedure adc_int_read () to read.
	\n	  When using interrupt, mostly we need to use auto trigger. If we use interrupt in single 
	\n	  conversion. We need to wait for conversion complete. THe should use adc_read () for single
	\n	  conversion need 
	\par Pseudo code
	\n
	\note   How to make sure that g_value had been read before the new values is saved  
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/  
SIGNAL(SIG_ADC)
{
 
    g_value = ADCW;
// Start the next conversion
//   ADCSRA |=  (1<< ADSC); //  This is not need if using auto trigger mode
}


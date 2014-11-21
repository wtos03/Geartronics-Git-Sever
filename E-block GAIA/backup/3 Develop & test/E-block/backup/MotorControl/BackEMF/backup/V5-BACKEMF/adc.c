/*
 

 Module 				:   ADC
 Description 			:   ADC Module
 Original written for 	:   ATMEGA32
 CPU port/tested		:   CPU/Microcontroller
 Hardware use			:   
 Port use				:   ADC
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Wichai / wichai@geartronics.net
 Date					:  15/07/2007
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
*/ 


#include "cpu.h"
#include "E_uart.h"


/* Initialize for ADC
ADCSRA 
	   Bit			Name			Purpose
		7			ADEN			ADC Enable bit
		6			ADSC			ADC Start conversion bit
		5			ADATE			ADC Auto trigger Enable (Advance need to set ADTS in SFIOR
		4			ADIF			ADC Interrupt Flag
		3			ADIE			ADC Interrupt Mask bit
		2- 0       ADPS0-2			ADC Prescaler select bits
                    Use 111 for 128 for 16 Mhz   Freq should be 50 KHz - 200 Khz
					
	INPUT : Voltage reference for ADC
	        0 = AREF pin
			1 = AVCC
			3 = Internal 2.56 v
ADMUX 
      Bit 5   left or right adjust bit in ADLAR
*/
void adc_init(uint8_t voltage)
{
    ADCSRA = 0xC7 | (1<< ADEN);
	
// Set voltage reference 	
	ADMUX  =  voltage << 6;

// Set input for ADC pin
    DDRA    &= 0xF0;	

}

/*
  Input mode = 0  Disable
        mode > 0  Enable


*/
void  adc_enable_int (uint8_t mode)
{
    if (mode)
	{
		ADCSRA  |= ( (1 << ADEN) | (1 << ADIE) | (1<< ADSC));
    }
	else
	{
		ADCSRA  &= ~((1 << ADIE) | (1<< ADSC));
	}
}


/*
Read adc value		
	
	INPUT :  channel  to read adc  ( See table for Differential INput and Gain)
    Polling mode 
*/

int16_t read_adc(uint8_t channel)
{
  int16_t  value;
  
 
// Before that need to clear ADATE and ADEN
    ADCSRA &= ~((1 << ADEN) | (1<< ADATE));

// Set channel
    ADMUX |= channel;
	
// Enable conversion
	ADCSRA |= (1<<ADEN);
// Start conversion
 
    
// Check whether conversion is complete
 
    ADCSRA |= (1<<ADSC);         // do single conversion
    while(!(ADCSRA & 0x10));    // wait for conversion done, ADIF flag active

//Read value
//      value = ADCL;            // read out ADCL register
//      value += (ADCH << 8);    // read out ADCH register        
	value = ADCW;
// Stop conversion
    ADCSRA &= ~(1 << ADEN);
	return value;
	
}



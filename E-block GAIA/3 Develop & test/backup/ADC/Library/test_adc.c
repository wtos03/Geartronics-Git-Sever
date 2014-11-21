/*
 Module 				:   Test ADC
 Description 			:   Universal Async Transmitter
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   
 Port use				:   ADC
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Wichai / wichai@geartronics.net
 Date					:   17/07/2006
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
*/


#include <stdlib.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "E_uart.h"
#include "adc.h"


int16_t  voltage_sum  = 0;
/*
 UART receiver ready
*/

SIGNAL(SIG_UART_RECV)
{
   uint8_t  ch;
   ch = UDR;
   uart_put_RX (ch);
}
  

/*************************************************************************
Function: UART Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/

SIGNAL(SIG_UART_DATA)
{ 
    uint8_t  ch;
	if ((ch = uart_get_TX()))
	{
 		UDR = ch;
	}	
	else 
// No character disable  interrupt to avoid repetable call Enable again when
//	send character in putc procedure
	{
	    UCSRB &=  ~(1<<UDRIE);
	}
	
}

SIGNAL(SIG_ADC)
{
 int16_t value;
 uint8_t buffer[20];
 
 value = ADCW;
 itoa( value, buffer, 10);   // convert interger into string (decimal format)         
 uart_puts("               From interrupt ");
 uart_puts(buffer);
 
// Start the next conversion
  ADCSRA |= ADSC;
//  adc_enable_int (0);
}

int main(void)
{

    uint8_t buffer[20];
    int16_t  num=1034;
    int16_t value,i, j;
// Initial DDR  for input ADC
     DDRA = 0x00;
    
    /*
     *  Initialize UART library, pass baudrate and AVR cpu clock
     *  with the macro 
     *  UART_BAUD_SELECT() (normal speed mode )
     *  or 
     *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
     */
    uart_init(); 

    
    /*
     *  Transmit string to UART
     *  The string is buffered by the uart library in a circular buffer
     *  and one character at a time is transmitted to the UART using interrupts.
     *  uart_puts() blocks if it can not write the whole string to the circular 
     *  buffer
     */
    uart_puts("Start ADC\n");
         itoa( num, buffer, 10);   // convert interger into string (decimal format)         
    
	adc_init(1);

	  /*
     * now enable interrupt, since UART library is interrupt controlled
     */
    sei();
    voltage_sum = 0;
	while(1)
	{
// Start conversion by using polling

  			value = read_adc(0x10);
			if (value > 1000)
			     value = 0;
			itoa( value, buffer, 10);   // convert interger into string (decimal format)         
			uart_puts("                    From polling ");
 
//			uart_putc('-');
			uart_puts(buffer);
			voltage_sum  += value;
			 
// Start conversion by using interrupt   
//		adc_enable_int (1);
// Loop to wait conversion complete
		for ( i = 0 ; i < 500 ;i++)	
		for ( j = 0 ; j < 20000 ;j++);
// 		adc_enable_int (0);
		itoa( voltage_sum, buffer, 10);   // convert interger into string (decimal format)          
		uart_putc('=');
		uart_puts(buffer);

    }
}

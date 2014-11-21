/*
 Module 				:   UART
 Description 			:   Universal Async Transmitter
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   RS-232
 Port use				:   UART Port Tx/Rx PD0, PD1
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Wichai / wichai@geartronics.net
 Date					:   1/06/2006
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
*/


#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#include "E_uart.h"


//  Global Data for Rx/Tx
//uint8_t g_rcv_buff[MAX_BUFF];  // Circular buffer
//uint8_t g_tra_buff[MAX_BUFF];
//uint8_t g_rcv_ptr = 0;		  // Pointer for Rx buffer
//uint8_t g_tra_ptr = 0;        // Pointer for Tx buffer    
//uint8_t g_getc_ptr = 0;       //  Pointer for getc fn 
//uint8_t g_putc_ptr = 0;       //  Pointer for putc fn
//uint8_t g_ch;

/* UART receiver ready
*/

SIGNAL(SIG_UART_RECV)
{  
   uint8_t ch;
   ch =  UDR;
   uart_put_RX (ch);    // Should not use UDR here to avoid characters missing
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


int main(void)
{
    uint8_t c;
    uint8_t buffer[7];
    uint8_t  num=134;

    
    /*
     *  Initialize UART library, pass baudrate and AVR cpu clock
     *  with the macro 
     *  UART_BAUD_SELECT() (normal speed mode )
     *  or 
     *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
     */
    uart_init(); 
     

 
    /*
     * now enable interrupt, since UART library is interrupt controlled
     */
    sei();
    
    /*
     *  Transmit string to UART
     *  The string is buffered by the uart library in a circular buffer
     *  and one character at a time is transmitted to the UART using interrupts.
     *  uart_puts() blocks if it can not write the whole string to the circular 
     *  buffer
     */
    uart_puts("String stored in SRAM\n");
    
    
        
    /* 
     * Use standard avr-libc functions to convert numbers into string
     * before transmitting via UART
     */     
    itoa( num, buffer, 10);   // convert interger into string (decimal format)         
    uart_puts(buffer);        // and transmit string to UART
    /*
     * Transmit single character to UART
     */
    uart_putc('\r');
         
    for(;;)
    {
        c  = uart_getc();
		uart_putc(c);
    }
    
}

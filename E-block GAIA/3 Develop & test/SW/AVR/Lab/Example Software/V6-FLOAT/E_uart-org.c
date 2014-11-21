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
24/7/2007     Wichai			Change from single pointer to dual pointer for circular buffer 
 
*/
 
#include <stdlib.h> 
#include "E_uart.h"

//  Global Data for Rx/Tx
uint8_t g_rx_buff[MAX_BUFF];
uint8_t g_tx_buff[MAX_BUFF];
uint8_t g_get_rx_ptr = 0;
uint8_t g_put_rx_ptr = 0;

uint8_t g_get_tx_ptr = 0;
uint8_t g_put_tx_ptr = 0;


 
/* USART, Rx Complete */

 
// UART Receive interrupt


/*
 * these functions are only for ATmegas with two USART
 */
/*************************************************************************
Function: uart_init()
Purpose:  initialize UART1 and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
void uart_init()
{
 
// Initialize UART  
    uint8_t i;
    UCSRC  = 0x86;    // 8 bit non parity  1 stop bit
     UCSRB  = 0x98;    // Enable RX Interrupt and Enable TX, RX pin
//    UCSRB  = 0B8;    // Enable RX Interrupt, Data registerempty interrupt enable and Enable TX, RX pin 1011-1000
	UBRRL  = 0x67;    // Speed 9600 UBRR = 103 at 16 MHZ
	UBRRH  = 0;
//    DDRD = 0x00;	 
 
	g_get_rx_ptr = 0;
	g_put_rx_ptr = 0;
	
	g_put_tx_ptr = 0;
	g_get_tx_ptr = 0;
 /* Set baud rate */
    for (i =0;i < MAX_BUFF; i++)
	    g_tx_buff[i] = 0xFF;
}

/*
Put character into Rx buffer

Return  = Success 0
        = Fail   Buffer Full
		
*/
uint8_t uart_put_RX ( uint8_t ch)
{
   if ((g_put_rx_ptr < MAX_BUFF) ) 
   {
//  End with Null
		g_rx_buff[g_put_rx_ptr] = ch;
		g_put_rx_ptr++;
		g_rx_buff[g_put_rx_ptr] = 0;   // End with null characters
		return 0;
    }
    else  // Buffer full do nothing  return error
    {
//       if (g_get_rx_ptr > 0)
//		     g_put_rx_ptr = 0;
		return UART_BUFFER_FULL;
    }
}

// Get character from RX buffer

uint8_t uart_get_RX (void)
{
   uint8_t  i,j,ch;
   if (g_get_rx_ptr)
   {
		ch = g_rx_buff[0];
        i =  g_get_rx_ptr;
        g_get_rx_ptr--;
		j = 0;
// Move left all characters
        while (i)
		{
	      g_rx_buff[j] =  g_rx_buff[j+1];
	      j++;
	 	  i--;
		}
		return ch;
   }
   return 0;
}

/* Put character into g_tx_buff  If it is 0xFF mean slot free can put if not wait until slot is free
    INPUT  : Character to put in buffer
	Output : = 0 oK
		   : UART_BUFFER_FULL  Buffer full
*/

uint8_t uart_put_TX ( uint8_t ch)
{
// If exceed buffer do nothing until buffer is flushed
  
// If buffer full wait until g_get_tx_ptr reach	
    if ( g_tx_buff[g_put_tx_ptr] == 0xFF)
	{
		g_tx_buff[g_put_tx_ptr]= ch;
		g_put_tx_ptr++;
		if ( g_put_tx_ptr > (MAX_BUFF))
		{
	 		g_put_tx_ptr = 0;  // Start from 0
	    }
		return 0;
      		
    }
	else
    {
		return UART_BUFFER_FULL;   // No increase pointer
    }
	
}


// Get character from TX buffer if it is 0xFF mean no more characters 
uint8_t uart_get_TX (void)
{
   uint8_t ch =0;
   
   if (g_tx_buff[g_get_tx_ptr] != 0xFF)
   {   
		ch = g_tx_buff[g_get_tx_ptr];
// Put 0xFF for Free slot;
		g_tx_buff[g_get_tx_ptr] = 0xFF;
		g_get_tx_ptr++;
		if (g_get_tx_ptr > (MAX_BUFF))
		{
		    g_get_tx_ptr = 0;  // Start from 0
		}
		return ch;
	}
	else    // 0xFF   Note than NUll char also return 0 which get the same results (NO more characters in buffer)
	   return 0;   // No characters in buffer

}


uint8_t uart_getc(void)
{
    uint8_t ch;
	ch = uart_get_RX();
    return ch ;
}

uint8_t uart_putc(uint8_t c)
{
   uint8_t status;
   status = uart_put_TX(c);
// Enable interrupt UART Data Register empty
   UCSRB  |=  (1<<UDRIE);	   
    return status;

}


/*************************************************************************
Function: uart_puts()
Purpose:  transmit string to UART1
Input:    string to be transmitted
Returns:  none          
**************************************************************************/
void uart_puts(const uint8_t *s )
{
    while (*s) 
	{
//      if(!uart_putc(*s))      // 0 = Success
	    uart_putc(*s);
			s++;
	}

    uart_putc(0x0D);
//	uart_putc(0x0A);
	uart_putc(0x00);   // Null at the end
}

/*
Show debug value
*/

void  debug_value ( uint16_t value, uint8_t radix)
{
   uint8_t  buff[20];
   
   itoa (value,buff, radix);
    uart_puts ( buff);

}

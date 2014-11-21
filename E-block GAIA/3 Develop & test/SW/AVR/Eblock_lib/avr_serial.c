 /*!  
	\file   	avr_comm.c
	\brief  	Communication module for AVR 
	\ingroup	avr_comm  
	\par  Module description
    \n Communication Module use in AVR
	\n		- UART (serial Port)
	\n		- I2C  
	\n		- SPI	
 	\n Original written for 	:   \b for bold AVR
 	\n CPU port/tested			:   ATMEGA32
 	\n Hardware use				:   E_MEGA32
 	\n Port use					:   UART Port Rx,TX
	\n							:   SPI  SCK, MOSI, MISO
	\n							:   I2C  SCL, SDA
	\n							:   SET JUMPER J7,J8 connect to RS232 port  (ALL UP)
	\n Interrrupt Vector table  :   SIG_UART_DATA, SIG_UART_RECV
 	\n Memory	FLASH			:   XXX
 	\n		RAM					:   XXX
 	\n		EEPROM				:   XXX
 	\n Reference Document		:   Document describe algorithm

	\version 0.1
 	\author  wichai@geartronics.net
	\date 	 04/09/08  

	\par Update History
 	\nDate			By 					Comments
 	\n----			--					--------
	\n 
	\note  Future enhancement for Error checking and Handling
 
*/






//! Include file
 
#include <avr/interrupt.h>
#include <stdlib.h> 
#include "avr_serial.h"


//  Global Data for Rx/Tx
uint8_t g_rx_buff[MAX_BUFF];
uint8_t g_tx_buff[MAX_BUFF];
uint8_t g_rx_ptr = 0;
uint8_t g_tx_ptr = 0;




// Internal Function Do not call from outside module

/*!	\brief  Put character into Rx buffer
	\param      Character to put in buffer 
	\return  	Success 	0
	\n			Fail   		Buffer Full
	\par Port/Periheral Usage 
	\n   Tx,Rx
	\par Theory of Operation  
	\n    Put characters into RX buffer until buffer full
	\par Pseudo code
	\n    Put characters into RX buffer until it's full ( Send BUFFER_FULL error)
	\n	  Increase rx ptr
	\note   Internal function  Do not call from outside
	\bug    If send characters too fast become garbage

	\author wichai tossamartvorakul  wichai@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
uint8_t uart_put_RX ( uint8_t ch)
{
   if ((g_rx_ptr < MAX_BUFF)) 
   {
//  End with Null
		g_rx_buff[g_rx_ptr] = ch;
		g_rx_ptr++;
		g_rx_buff[g_rx_ptr] = 0;   // End with null characters
		return 0;
    }
   else  // Buffer full do nothing  return error
   {
        return UART_BUFFER_FULL;
   }
}


/*!	\brief  Get character from RX buffer
	\param  None  
	\return Character read from RX buffer
			0  =  No characters ?
	\par Port/Periheral Usage 
	\n   TX,RX
	\par Theory of Operation   
	\n   Get characters from RX buffer and move all characters to the left
	\par Pseudo code
	\n    If there are some characters left in buffer
	\n       get it and move all characters to the left
	\note   Internal function  Do not call from outside
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/      

uint8_t uart_get_RX (void)
{
   uint8_t  i,j,ch;
   if (g_rx_ptr)
   {
		ch = g_rx_buff[0];
        i =  g_rx_ptr;
        g_rx_ptr--;
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

/* Put character into g_tx_buff
    INPUT  : Character to put in buffer
	Output : 0  OK
		   : UART_BUFFER_FULL  Buffer full
*/

/*!	\brief  Put character in to TX Buffer 
	\param  None
	\return None
	\par Port/Periheral Usage 
	\n   TX,RX
	\par Theory of Operation   
	\n    Put characters into TX buffer until buffer full
	\par Pseudo code
	\n    Put characters into TX buffer until it's full ( Send BUFFER_FULL error)
	\n	  Increase tx ptr
	\n	  Put null characters at the end
	\n
	\note   Internal function  Do not call from outside 
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
uint8_t uart_put_TX ( uint8_t ch)
{
// If exceed buffer do nothing until buffer is flushed
    if ((g_tx_ptr > MAX_BUFF)) 
    {
		return UART_BUFFER_FULL;   // No increase pointer
    }
	g_tx_buff[g_tx_ptr]= ch;
	g_tx_ptr++;
	g_tx_buff[g_tx_ptr] = 0;    // Put null character at the end
	return 0;
}


/*!	\brief  Get character from TX buffer to send and shift left all characters
	\param  None  
	\return Character to send 
	\par Port/Periheral Usage 
	\n   TX,RX
	\par Theory of Operation   
	\n   Get characters from buffer(FIFO) and shift all characters to left 
 	\par Pseudo code
	\n   Check whether there is data in TX buffer
	\n	 If yes  get characters  from TX buffer
	\n
	\note   Internal function  Do not call from outside
	\bug    <bug to fix>
	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
 
uint8_t uart_get_TX (void)
{
   uint8_t i,j,ch =0;
   
   if ( g_tx_ptr)   // There are data in TX buffer
   {
		i  =  g_tx_ptr;
		g_tx_ptr--;
		ch =  g_tx_buff[0];   // Start from 0 FIFO
		j = 0;
// Move left all characters Need to rewrite !!!
        while (i)
		{
	      g_tx_buff[j] =  g_tx_buff[j+1];
	      j++;
	 	  i--;
		}
		return ch;
   }
   return 0;
}



 

/*!	\brief  Show debug value for short integer
	\param  Value to show (16 bits)
	\n		radix to show 10, 16,2	
	\return none
	\par Port/Periheral Usage 
	\n   TX,RX
	\par Theory of Operation   
	\n   Convert value to string and show on UART port
	\par Pseudo code
	\n   Convert long integer to string with  
	\note    
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
void  debug_value ( int16_t value, uint8_t radix)
{
   uint8_t  buff[20];
  
   itoa (value,buff, radix);
   uart_write_str ( buff);
}


/*!	\brief  Show debug value in long format
	\param  Value to show (32 bits)
	\n		radix  to show  10, 16,2
	\return None
	\par Port/Periheral Usage 
	\n  TX,RX
	\par Theory of Operation   
	\n   Convert value to string and show on UART port
	\par Pseudo code
	\n   Convert long integer to string with radix specified
	\n	 Write string to UART port
	\note   (Pseudo code)
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     

void  debug_lvalue ( int32_t value, uint8_t radix)
{
   uint8_t  buff[40];
   
   ltoa (value,buff, radix);
   uart_write_str ( buff);
}
 

/*!	\brief  Initial serial Port UART
	\param  mode  =  TX/RX  Mode of operation  (Intrrupt/Polling) Enable RX, TX
			frame =  Set Data frame  8 bits  parity bits, stop bit
			baud  =  Baud rate 
			These parameter are predefined in avr_serial.h for easy set up
 	\return None
	\par Port/Periheral Usage 
	\n   TX, RX
	\par Theory of Operation   
	\n    Initial control register to set up for baud rate, data frame and mode of operations
	\n    For receive we are using interrupt for RX and TX 
	\par Pseudo code
	\n    Set and initial register for UART mode
	\note Future enhancement for error checking and overrun detect 
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
void uart_init(uint8_t mode,uint8_t frame, uint16_t baud)
{
 
// Initialize UART  
    
    UCSRC  = frame;    // 8 bit non parity  1 stop bit
    UCSRB  = mode;    // Enable RX Interrupt and Enable TX, RX pin
	UBRRL  = (uint8_t) baud;
	UBRRH  = (uint8_t)(baud >>8); 
	g_rx_ptr = 0;
	g_tx_ptr = 0;
    sei();
}


/*!	\brief  Get character from RX buffer
	\param  None  
	\return Read character from buffer  0 if none characters
	\par Port/Periheral Usage 
	\n   TX,RX
	\par Theory of Operation   
	\n    Read character from RX buffer reside inside this module
	\par Pseudo code
	\n
	\note 
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
uint8_t uart_read(void)
{
    uint8_t ch;
	ch = uart_get_RX();
    return ch ;
}


/*!	\brief  Write character
	\param  None  
	\return None
	\par Port/Periheral Usage 
	\n   E_PB0,E_PB3
	\par Theory of Operation   
	\n
	\par Pseudo code
	\n
	\note   (Pseudo code)
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
uint8_t uart_write_char(uint8_t c)
{
   while (UART_BUFFER_FULL == uart_put_TX(c));  //If buffer full wait until buffer clear
// Enable interrupt UART Data Register empty
  UCSRB  |=  (1<<UDRIE);	
   return 0;
}



/*!	\brief   Transmit string to UART1
	\param   Pointer to String  
	\return None
	\par Port/Periheral Usage 
	\n   TX,RX
	\par Theory of Operation   
	\n   Write each characters pointed by string pointer to UART until end of string
	\par Pseudo code
	\n    Write each char until NULL string found
	\n	  Send CR + LF for new line
	\note    
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
void uart_write_str(const uint8_t *s)
{
 
    while (*s) 
    {
		uart_write_char(*s);
	    if (*s ==  0x0A)
		{
			uart_write_char(0x0D); // CR
//			uart_write_char(0x0A); // LF
		}
		s++;
	}
	
}


/*!	\brief  Interrupt handler for RX 
	\param     
	\return Rx characters put in buffer
	\par Port/Periheral Usage 
	\n   TX,RX
	\par Theory of Operation   
	\n    
	\par Pseudo code
	\n   Put each received characters in buffer
	\note  
	\bug  <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
/* UART receiver ready
*/

SIGNAL(SIG_UART_RECV)
{  
   uint8_t ch;
   ch =  UDR;
   uart_put_RX (ch);    // Should not use UDR here to avoid characters missing
}
 
 

/*!	\brief   Interrupt service routine for UART TX Data Register Empty interrupt
	\n		 Called when the UART is ready to transmit the next byte
	\param   Pointer to String  
	\return None
	\par Port/Periheral Usage 
	\n   TX,RX
	\par Theory of Operation   
	\n   When TX data register is empty to send the next characters. This routine will
	\n   activate and the characters will be read from TX buffer until end of text. 
	\n   If end of characers, this interrupt will be disable.
	\par Pseudo code
	\n   Send characters from TX buffer. If null char had found disable interrupt to avoid 
	\n   repeat interrupt call
	\note    
	\bug    Send too fast get garbage

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
 
SIGNAL(SIG_UART_DATA)
{ 
    uint8_t  ch;
	if ((ch = uart_get_TX()))
	{
 		UDR = ch;
	}	
	else 
// If there are no character,  disable interrupt to avoid repeat call again 
	{
	    UCSRB &=  ~(1<<UDRIE);
	}
	
}


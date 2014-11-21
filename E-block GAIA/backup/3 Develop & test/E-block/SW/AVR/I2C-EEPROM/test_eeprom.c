/*

 Module 				:   Test eeprom module
 Description 			:   Test eeprom read/write using i2c interface 
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   Dual 7 Segments common cathode module
 Port use				:   E_PORTA (0-7), E_PORTB (0-1), SCL, SDA
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Wichai  wichai@geartronics.net
 Date					:  	23/04/2006
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
  */
 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/delay.h>
#include "cpu.h"
#include "Dual_7Segment.h"
#include "E_i2ceeprom.h"
#include "E_i2c.h"
#include "E_uart.h"
 
  

#define READ 1
#define WRITE 2
#define STOP 0 
#define NULL 0
#define I2C_EEPROM_ADDR  0xA0  // Address eeprom 1010 	- 0000
							//                 EEprom   - A2,A1,A0 - R/W



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
 
void main(void);



 
/*
This program test for eeprom read/ write.  Press SW1 to start recording text Press SW1 again to stop recording
Press SW2 to read stored text from eeprom  Press again to stop The 7 Segments will show 01 for read mode and  02 for write mode
00 for stop

*/

void main()
{
  uint8_t sw1,sw2,ch;
  uint16_t i;
  int8_t d1,d2;
  uint8_t  g_mode  = 0;
  uint8_t index = 0;
//  uint32_t  memAddr = 0;   // Start memory Address
  
  d7segment_init();
/*
 *  Initialize UART library, pass baudrate and AVR cpu clock
 *  with the macro 
 *  UART_BAUD_SELECT() (normal speed mode )
 *  or 
 *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
*/
  DDRC = 0xFC;
  uart_init(); 

  i2ceeprom_init();
  i2c_set_localdeviceaddr(I2C_EEPROM_ADDR,FALSE);
 
  sei();
  d1 = 0;
  d2 = 0;
  uart_puts("Program Start\n");

	i2ceeprom_write_byte(I2C_EEPROM_ADDR,0,1);
	i = i2ceeprom_read_byte(I2C_EEPROM_ADDR,0);
		

 
  while (1)
  {
 		
		
   sw1 = _7SEGMENT_SW1_IN_PORT & _7SEGMENT_SW1; 
   
 
 // Delay
   
   if (!sw1) // write eeprom 
    {   	
        while (!(sw1 = _7SEGMENT_SW1_IN_PORT & _7SEGMENT_SW1));
		d1  = d1^1;           //Toggle 
		if (d1)
		{
			g_mode = WRITE;
	        uart_puts("\nText will stored in EEPROM\n");
			index  = 0;
		}	
		else 
		{    g_mode = STOP;
// Write null character
			i2ceeprom_write_byte(I2C_EEPROM_ADDR,( index),NULL);
		     index  = 0; 
		}	 

	}
   
   sw2 = _7SEGMENT_SW2_IN_PORT & _7SEGMENT_SW2; 
   
 
   if (!sw2) // Read eeprom 
   {
 // Check until release
	   while (!(sw2 = _7SEGMENT_SW2_IN_PORT & _7SEGMENT_SW2));
	   d2 = d2^1;
	   if (d2)
	    {
			g_mode = READ;
		    uart_puts("\nText read from EEPROM\n");
		}
		else	
		{
			g_mode = STOP;
			index  = 0;
	    }
    	
   }
   
   if (g_mode == WRITE)
   {
 // Note that if not check for ch ch will equal 0 whether the key pressed or not
		if ((ch  = uart_getc()))
		{
			uart_putc(ch);   // Show on screen
			i2ceeprom_write_byte(I2C_EEPROM_ADDR,( index),ch);
			index ++;
	    }
   }
   
   if (g_mode == READ)
   {
// Read until found null char        
        i = 0;
        while ((ch = i2ceeprom_read_byte(I2C_EEPROM_ADDR,(i))))
		{		
				uart_putc(ch);   // Show on screen
				i++;
		}
		uart_putc('\n');
		g_mode = STOP;
   }
   	
   	d7segment_display(0,1);
 		    _delay_ms(10);
	d7segment_display(g_mode,2);
 			_delay_ms(10);
		
    
  }
     
}



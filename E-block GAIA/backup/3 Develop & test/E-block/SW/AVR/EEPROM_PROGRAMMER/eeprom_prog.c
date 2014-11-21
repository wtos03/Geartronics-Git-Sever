/*

 Module 				:   Eeprom  programmer module
 Description 			:   eeprom read/write/copy  using i2c interface 
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   Dual 7 Segments common cathode module
						:   Socket programmer for 24LC32
 Port use				:   E_PORTA (0-7), E_PORTB (0-1), SCL, SDA
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Wichai  wichai@geartronics.net
 Date					:  	24/6/2008
  
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
#define I2C_EEPROM_MASTER_ADDR  0xA2        // Address eeprom 1010 	- 0010
							                //  EEprom   - A2,A1,A0 - R/W


#define I2C_EEPROM_SLAVE_ADDR  0xA0        	// Address eeprom 1010 	- 0000
											// EEprom  - A2,A1,A0 - R/W
#define  E_24LC32_MEM_ADDR   0x0800			// 0x1000 For 24LC32  32 kbits
#define  EEPROM_DELAY	   5				  
 
void main(void);



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
 
 
/*
This program read the data from master eeprom and copy to slave eeprom by copy from master eeprom into master
and copy from buffer to slave eeprom 

*/

void main()
{
  uint8_t sw1,sw2;
  uint16_t  memaddr = 0;   // Start memory Address
  uint8_t  dat,c,i;
  
 /*
 *  Initialize UART library, pass baudrate and AVR cpu clock
 *  with the macro 
 *  UART_BAUD_SELECT() (normal speed mode )
 *  or 
 *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
*/
  DDRC = 0xFC;
  uart_init(); 
 
  
  d7segment_init();
  i2ceeprom_init();
  i2c_set_localdeviceaddr(I2C_EEPROM_MASTER_ADDR,FALSE);
  i2c_set_localdeviceaddr(I2C_EEPROM_SLAVE_ADDR,FALSE);

  sei();
 


//	i2ceeprom_write_byte(I2C_EEPROM_ADDR,0,1);
//	i = i2ceeprom_read_byte(I2C_EEPROM_ADDR,0);
		
 
  while (1)
  {
 		
		
   sw1 = _7SEGMENT_SW1_IN_PORT & _7SEGMENT_SW1; 
   sw2 = _7SEGMENT_SW2_IN_PORT & _7SEGMENT_SW2; 
     
  
 // Delay
   
   if (!sw1) // Start copy eeprom 
    {   	
        i = 0;
		for (memaddr = 0  ; memaddr < E_24LC32_MEM_ADDR ;memaddr++)
		{
   
			dat = i2ceeprom_read_byte(I2C_EEPROM_MASTER_ADDR,memaddr);

			debug_value (dat,16); 
 			i2ceeprom_write_byte(I2C_EEPROM_SLAVE_ADDR, memaddr,dat);
//			_delay_ms( EEPROM_DELAY);
	          
			if (i < 100)
			{
				c =0;
			}
			if (i > 100) 
			{
				c ='-';			
            }			
			if (i > 200)
				i = 0;
			
			d7segment_display(c,1);
			_delay_ms(2);
			d7segment_display(c,2);
			_delay_ms(2);
			
		    i++;
   
		}  

//	

	}
// Finish copy   	
   	d7segment_display(0,2);
	_delay_ms( 10);
	d7segment_display(0,1);
	_delay_ms( 10);
    
  }
     
}



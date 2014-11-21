/* 
 Include file			: cpu_atmega32.h
 Description			: To setup parameters for ATMEGA32. 
						  Every microcontroller will need to include this file for port mapping
						  and other parameters ex. Clock speed. This will be the first files to include
						  
 CPU					:  ATMEGA32						  
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   21/3/2006
 
 
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
*/

#include <avr/io.h>


#define 	CPU_CLOCK_SPEED		16   // 16 Mhz

 
// General Port 

//-------  Standard  E block port
#define E_INT1			0x08

#define E_PORT0			0x01
#define E_PORT1			0x02
#define E_PORT2			0x04
#define E_PORT3			0x08
#define E_PORT4			0x10
#define E_PORT5			0x20
#define E_PORT6			0x40
#define E_PORT7			0x80

#define E_OUT_PORTA		PORTB
#define E_OUT_PORTB		PORTC
#define E_OUT_PORTC		PORTA
#define E_OUT_PORTD		PORTD

#define E_IN_PORTA		PINB
#define E_IN_PORTB		PINC
#define E_IN_PORTC		PINA
#define E_IN_PORTD		PIND

#define E_DIR_PORTA		DDRB
#define E_DIR_PORTB		DDRC
#define E_DIR_PORTC		DDRA
#define E_DIR_PORTD		DDRD

 
 



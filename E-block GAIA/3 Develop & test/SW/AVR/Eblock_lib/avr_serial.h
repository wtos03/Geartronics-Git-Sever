 /*!  \file  	avr_serial.h			 
      \brief    Include file for Communication Module in AVR
      \defgroup  avr_serial
      \ingroup   Eblock_lib
	\nSummary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable				X   
	\n Function prototype				-
	\n Variable (Keep in Flash Program memory)	-
	\n Memory usage					?? bytes
	\n      <Module detail>
      \code #include <avr_serial.h> \endcode
 
      \version	0.1
      \author 	wichai@geartronics.net
      \date		5/9/08

      \n \b Update \b History
 
	\n Date                 By                     Comments
	\n ----                 --                     ---------
        \n
 
 
	\note   
	   Not include avr in function because can port these API to other CPU. 
 
*/
 
 

//! External include file
#include "cpu.h"

 




//! Constant variable, For comment in doxygen before the variable declaration ???? 
// For UART
//! Set baud rate   UBRR = ((fosc)/16BAUD)-1   for Normal mode
//                  UBRR = ((fosc)/8BAUD)-1  for Double speed mode
//! Set for Data frame

 
#define UART_9600         	 0x0067
#define UART_RX_INTERRUPT	_BV(RXCIE) | _BV(RXEN) | _BV(TXEN)          // Use interrupt mode for TX, RX
#define UART_8_N_1 			_BV(URSEL) | _BV(UCSZ1)	| _BV(UCSZ0)		// Async 8 bit Non-parity 1 Stop bit

//! Framing Error by UART            
#define UART_FRAME_ERROR      0x0800      
//! Overrun condition by UART          
#define UART_OVERRUN_ERROR    0x0400     
//! receive ringbuffer overflow         
#define UART_BUFFER_OVERFLOW  0x0200    
//! no receive data available           
#define UART_NO_DATA          0x0100   
          
#define	UART_BUFFER_FULL	  2
#define MAX_BUFF  			  250



#define UART_BAUD_SELECT (CPU_CLOCK_SPEED*1000000/(UART_BAUD_RATE*16l)-1)


//! Variable in Flash
//!  For comment in doxygen before the variable declaration ???? 


//! Function prototypes


// Function prototypes

// UART 
void uart_init(uint8_t,uint8_t,uint16_t);   // Baud rate setting
uint8_t	uart_read (void);
uint8_t	uart_write_char(uint8_t);
void  	uart_write_str(const uint8_t *);
void  debug_value ( int16_t,uint8_t);   // Comment when debug off  
void  debug_lvalue ( int32_t,uint8_t);   // Comment when debug off  

  





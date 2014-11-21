 /*!  \file  	avr_comm.h			 
      \brief    Include file for Communication Module in AVR
      \defgroup  avr_comm
      \ingroup   Eblock_lib
	\nSummary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable				X   
	\n Function prototype				-
	\n Variable (Keep in Flash Program memory)	-
	\n Memory usage					?? bytes
	\n      <Module detail>
      \code #include <avr_comm.h> \endcode
 
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
//! Set baud rate
#define UART_BAUD_RATE        9600   
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
void   uart_init (void);    // Baud rate setting
uint8_t uart_getc (void);
uint8_t uart_putc (uint8_t);
void   uart_puts(const uint8_t *);

uint8_t uart_put_RX (uint8_t); 
uint8_t uart_get_RX (void);
uint8_t uart_put_TX (uint8_t);
uint8_t uart_get_TX (void);
void  debug_value ( int16_t,uint8_t);   // Comment when debug off  
void  debug_lvalue ( int32_t,uint8_t);   // Comment when debug off  
  





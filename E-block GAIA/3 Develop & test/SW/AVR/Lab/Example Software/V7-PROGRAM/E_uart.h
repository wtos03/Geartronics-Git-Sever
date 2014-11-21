/* 
 Include file for E_usart.c
 
 Written by 			:   Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   1/6/2006
 
 Summary for include files
 -------------------------
 Constant variable				X
 Function prototype				-
 Variable (Flash memory)		-
 Memory usage					0 bytes
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
*/



/* External Dependents    Need outside parameters 
CPU_CLOCK_SPEED

*/


#include "cpu.h"

#define UART_BAUD_RATE     9600        /* 19200 baud */


#define UART_FRAME_ERROR      0x0800              /* Framing Error by UART       */
#define UART_OVERRUN_ERROR    0x0400              /* Overrun condition by UART   */
#define UART_BUFFER_OVERFLOW  0x0200              /* receive ringbuffer overflow */
#define UART_NO_DATA          0x0100              /* no receive data available   */
#define	UART_BUFFER_FULL	   2
#define MAX_BUFF  250



#define UART_BAUD_SELECT (CPU_CLOCK_SPEED*1000000/(UART_BAUD_RATE*16l)-1)

// Function prototypes

 
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
 


/*
    Title:    UART library
    Author:   Volker Oth
    Date:     5/1999
    Purpose:  Sending a block of data to uart.
    needed
    Software: AVR-GCC to compile, AVA to assemble and link
    needed
    Hardware: ATS90S8515 on STK200 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#ifndef UART_H
#define UART_H

/* set baud rate here */
#define UART_BAUD_RATE 9600
#define ESC            0x1b
#define UART_BUF_SIZE    16


/* automatically calcuate baud register value */
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)


/* prototypes */
extern void uart_init(void);
extern void uart_clr(void);
extern void uart_nl(void);
extern bool uart_putchar(u08 c);
extern s16  uart_getchar(void);
extern bool uart_putstr(u08 s[]);

#endif 

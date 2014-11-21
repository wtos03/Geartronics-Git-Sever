/*
    Title:    AVR-GCC test program #6 for the STK200 eva board
    Author:   Volker Oth
    Date:     5/1999
    Purpose:  Demonstrates use of floating point library.
              Executes 4 simple floating point instructions.
              These are converted to 16bit fixed point
              and transmitted to the PC via the UART.
              The results can be watched with a terminal program:
                  $0000 (start identifier)
                  $006b = 107 = 10.0*(7.5+3.2)
                  $002b =  43 = 10.0*(7.5-3.2)
                  $00f0 = 240 = 10.0*(7.5*3.2)
                  $0017 =  23 = 10.0*(7.5/3.2)
              
    needed
    Software: AVR-GCC to compile
    needed
    Hardware: ATS90S8515/8535/mega(?) on STK200/300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#include <io.h>
#include <interrupt.h>
#include <signal.h>


#define F_CPU          4000000
#define UART_BAUD_RATE    9600
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)


typedef unsigned char  u08;
typedef          char  s08;
typedef unsigned short u16;
typedef          short s16;


u08 uart_ready;       /* uart globals */
u08 *uart_data_ptr;
s08 uart_counter;
s16 result_buf[5];    /* result buffer */
float a_buf[2] = { 7.};


SIGNAL(SIG_UART_TRANS)      
/* signal handler for uart txd ready interrupt */
{
    uart_data_ptr++;
    uart_counter--;

    if (uart_counter>0)
        outp(*uart_data_ptr, UDR);       /* write byte to data buffer */
    else
        uart_ready = 1;        /* ready to send */
}


void uart_send(u08 *buf, u08 size)
/* send buffer <buf> to uart */
{
    /* write first byte to data buffer */
    if (!uart_ready) return;
    uart_ready = 0;        /* not ready to send */
    uart_data_ptr  = buf;
    uart_counter   = size;
    outp(*buf, UDR);
}


void calc(float a, float b)
{
    result_buf[0] = 0;
    result_buf[1] = (a+b)*10.0;
    result_buf[2] = (a-b)*10.0;
    result_buf[3] = (a*b)*10.0;
    result_buf[4] = (a/b)*10.0;
}

int main(void)
{

    /* enable RxD/TxD and ints */
    outp((1<<RXCIE)|(1<<TXCIE)|(1<<TXEN),UCR);       
    /* set baud rate */
    outp((u08)UART_BAUD_SELECT, UBRR);          

    uart_ready = 1;        /* ready to send */
    sei();                 /* enable interrupts */

    for (;;) {             /* loop forever */
        calc( 7.5, 3.2);
        
        uart_send((u08*)result_buf, 5*sizeof(s16));
    }            
}
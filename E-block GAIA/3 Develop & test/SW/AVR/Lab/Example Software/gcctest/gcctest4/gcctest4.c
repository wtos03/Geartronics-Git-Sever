/*
    Title:    AVR-GCC test program #4 for the STK200 eva board
    Author:   Volker Oth
    Date:     4/1999
    Purpose:  Uses the UART to communicate with a terminal program on the pc.
              The "tranceive complete" interrupt is used to send the string 
              "Serial Data from AVR receiced###" continuously to the pc.
              When a byte was received from the pc, the "receive complete"
              interrupt is called, which outputs the byte to PortB where the
              LEDs visualize the 8 bits it consists of.
              UART format: 9600 baud, 8bit, 1 stopbit, no parity
    needed
    Software: AVR-GCC to compile
    needed
    Hardware: ATS90S8515/8535/2313/mega(?) on STK200/300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#include <io.h>
#include <interrupt.h>
#include <signal.h>


#define F_CPU            4000000      /* 4Mhz */
#define UART_BAUD_RATE      9600      /* 9600 baud */


#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)


typedef unsigned char  u08;
typedef          char  s08;
typedef unsigned short u16;
typedef          short s16;


/* uart globals */
static volatile u08 *uart_data_ptr;
static volatile u08 uart_counter;


SIGNAL(SIG_UART_TRANS)      
/* signal handler for uart txd ready interrupt */
{
    uart_data_ptr++;

    if (--uart_counter)
        outp(*uart_data_ptr, UDR);       /* write byte to data buffer */
}


SIGNAL(SIG_UART_RECV)      
/* signal handler for receive complete interrupt */
{
    register char led;

    led = inp(UDR);        /* read byte for UART data buffer */
    outp(~led, PORTB);     /* output received byte to PortB (LEDs) */
}


void uart_send(u08 *buf, u08 size)
/* send buffer <buf> to uart */
{   
    if (!uart_counter) {
        /* write first byte to data buffer */
        uart_data_ptr  = buf;
        uart_counter   = size;
        outp(*buf, UDR);
    }
}


void uart_init(void)
/* initialize uart */
{
    /* enable RxD/TxD and ints */
    outp((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN),UCR);       
    /* set baud rate */
    outp((u08)UART_BAUD_SELECT, UBRR);          
}


int main(void)
{
    outp(0xff ,DDRB);      /* PortB output */
    outp(0x00, PORTB);     /* switch LEDs on */

    uart_init();
    sei();                 /* enable interrupts */

    for (;;) {             /* loop forever */
        uart_send("Serial Data from AVR received###", 32);
    }            
}
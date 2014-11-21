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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>


#define F_CPU            8000000      /* 8Mhz */
#define UART_BAUD_RATE      19200      /* 19200 baud */


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
	UDR &= *uart_data_ptr;
//    outp(*uart_data_ptr, UDR);       /* write byte to data buffer */
}


SIGNAL(SIG_UART_RECV)      
/* signal handler for receive complete interrupt */
{
    register char led;
	led = UDR;
//  led = inp(UDR);        /* read byte for UART data buffer */
	PORTC &= ~led;
//  outp(~led, PORTB);     /* output received byte to PortB (LEDs) */
}


void uart_send(u08 *buf, u08 size)
/* send buffer <buf> to uart */
{   
    if (!uart_counter) {
        /* write first byte to data buffer */
        uart_data_ptr  = buf;
        uart_counter   = size;
 //     outp(*buf, UDR); 
		uart_send(*buf, UDR);
    }
}


void uart_init(void)
/* initialize uart */
{
    /* enable RxD/TxD and ints */
    UCSRB &= ((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN));
//	outp((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN),UCSRB);       
    /* set baud rate */
	UBRRL&= ((u08)UART_BAUD_SELECT);
//    outp((u08)UART_BAUD_SELECT,UBRRL);          
}


int main(void)
{
		DDRC = 0x7E;
		PORTC = 0x00;

//    outp(0x7E ,DDRC);      /* PortB output */
//    outp(0x00, PORTB);     /* switch LEDs on */

    uart_init();
    sei();                 /* enable interrupts */

    for (;;) {             /* loop forever */
        uart_send("Serial Data from AVR received###", 32);
    }            
}


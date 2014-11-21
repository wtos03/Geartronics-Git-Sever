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

#include <io.h>
#include <signal.h>
#include <interrupt.h>
#include "global.h"
#include "timer.h"
#include "uart.h"


/* uart globals */
volatile u08 uart_txd_buf_cnt;
volatile u08 uart_rxd_buf_cnt;
u08 *uart_txd_in_ptr, *uart_txd_out_ptr;
u08 *uart_rxd_in_ptr, *uart_rxd_out_ptr;
u08 UART_CLR[] = {ESC, '[','H', ESC, '[', '2', 'J',0};
u08 UART_NL[] = {0x0d,0x0a,0};
u08 uart_txd_buffer[UART_BUF_SIZE];
u08 uart_rxd_buffer[UART_BUF_SIZE];


void uart_init(void)
/* initialize uart */
{
    /* enable RxD/TxD, receive complete int */
    outp((1<<RXCIE)|(1<<RXEN)|(1<<TXEN),UCR);       
    /* set baud rate */
    outp((u08)UART_BAUD_SELECT, UBRR);          
    uart_txd_in_ptr  = uart_txd_out_ptr = uart_txd_buffer;
    uart_rxd_in_ptr  = uart_rxd_out_ptr = uart_rxd_buffer;
    uart_txd_buf_cnt = 0;
    uart_rxd_buf_cnt = 0;
}


SIGNAL(SIG_UART_DATA)      
/* signal handler for uart data buffer empty interrupt */
{
    if (uart_txd_buf_cnt > 0) {
        outp(*uart_txd_out_ptr, UDR);           /* write byte to data buffer */
        if (++uart_txd_out_ptr >= uart_txd_buffer + UART_BUF_SIZE) /* Pointer wrapping */
            uart_txd_out_ptr = uart_txd_buffer;
        if(--uart_txd_buf_cnt == 0)             /* if buffer is empty: */
            cbi(UCR, UDRIE);                    /* disable UDRIE int */
    }
}


SIGNAL(SIG_UART_RECV)      
/* signal handler for receive complete interrupt */
{
    *uart_rxd_in_ptr = inp(UDR);            /* read byte from receive register */
    uart_rxd_buf_cnt++;
    if (++uart_rxd_in_ptr >= uart_rxd_buffer + UART_BUF_SIZE) /* Pointer wrapping */
        uart_rxd_in_ptr = uart_rxd_buffer;
}


s16 uart_getchar(void)
{
    u08 c;

    if (uart_rxd_buf_cnt>0) {
        cli();
        uart_rxd_buf_cnt--;
        c = *uart_rxd_out_ptr;              /* get character from buffer */
        if (++uart_rxd_out_ptr >= uart_rxd_buffer + UART_BUF_SIZE) /* pointer wrapping */
            uart_rxd_out_ptr = uart_rxd_buffer;
        sei();
        return c;
    } 
    else 
        return -1;                          /* buffer is empty */
}


bool uart_putchar(u08 c)
{
    if (uart_txd_buf_cnt<UART_BUF_SIZE) {
        cli();
        uart_txd_buf_cnt++;
        *uart_txd_in_ptr = c;               /* put character into buffer */
        if (++uart_txd_in_ptr >= uart_txd_buffer + UART_BUF_SIZE) /* pointer wrapping */
            uart_txd_in_ptr = uart_txd_buffer;
        sbi(UCR, UDRIE);                    /* enable UDRIE int */
        sei();
        return 1;
    } 
    else 
        return 0;                           /* buffer is full */
}


bool uart_putstr(u08 s[])
{
    char *c = s;

    while (*c)
        if (uart_putchar(*c))
            c++;

    return 1;
}


void uart_clr(void)
/* Send a 'clear screen' to a VT100 terminal */
{
    uart_putstr(UART_CLR); 
}


void uart_nl(void)
/* Send a 'new line' */
{
    uart_putstr(UART_NL); 
}


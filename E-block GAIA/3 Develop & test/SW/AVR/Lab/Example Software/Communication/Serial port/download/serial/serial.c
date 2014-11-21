/***************************************************************************

	Title	: E-Block firmware V 1.0
	File 	: serial.c
    Author	: Narongsak Ketsuwan (narongsak@geartronics.net)
    Date	: 22/02/2006
    Purpose : Firmware for e-block project 
	Software: AVR-GCC
    Hardware: ATMEGA8L
UPDATE HISTORY	
	First Skeleton for firmware
 	File name:Interrupt_serial_i_o
	By 		: AVR FREAKS :http://www.avrfreaks.com
	Date 	: 22 Feb 2006

***************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "UART.h"


/* Buffersizes must be 2^n */

#define TBUFSIZE	32
#define RBUFSIZE	32

#define TMASK		(TBUFSIZE-1)
#define RMASK		(RBUFSIZE-1)

volatile unsigned char tbuf[TBUFSIZE];
volatile unsigned char rbuf[RBUFSIZE];

volatile unsigned char t_in;
volatile unsigned char t_out;

volatile unsigned char r_in;
volatile unsigned char r_out;
static uint8_t i=0;

/* When modifing this code remember there is a difference between SIGNAL and
   INTERRUPT, the first instruction in the SIGNAL routine is 'cli' thus disabeling
   all interrupts while executing the ISR. INTERRUPT's first instruction is 'sei'
   allowing further interrupt processing */
SIGNAL (SIG_INTERRUPT1) 
{	
	sei();
	GICR = (~1<<INT1);			//disable int1
	
	printf_P(PSTR("interrupt_1 display\r\n"));

	GICR = (1<<INT1);			//enable int1
}

SIGNAL(SIG_UART_RECV) 
{

	/* Receive interrupt */
	char c;
	
	c = UDR;							// Get received char

	rbuf[r_in & RMASK] = c;
	r_in++;
}

SIGNAL(SIG_UART_DATA) 
{

	/* Data register empty indicating next char can be transmitted */
	if(t_in != t_out) 
	{
		UDR = tbuf[t_out & TMASK];
		t_out++;	
	}
	else 
	{
		UCSRB &= ~(1<<UDRIE);
	}
}

SIGNAL(SIG_OVERFLOW0)
{
	i++;	
	if(i>100)
	{		
		printf_P(PSTR("Timer0 interrupt display\r\n"));
		i=0;
	}
//	printf_P(PSTR("%d\r\n"),i);
}

char tbuflen(void) 
{
	return(t_in - t_out);
}

int UART_putchar(char c) 
{
	/* Fills the transmit buffer, if it is full wait */
	while((TBUFSIZE - tbuflen()) <= 2);
	
	/* Add data to the transmit buffer, enable TXCIE */
	tbuf[t_in & TMASK] = c;
	t_in++;
	
	UCSRB |= (1<<UDRIE);			// Enable UDR empty interrupt
	
	return(0);
}

char rbuflen(void) 
{
	return(r_in - r_out);
}

int UART_getchar(void) 
{
	unsigned char c;

	while(rbuflen() == 0);
	
	c = rbuf[r_out & RMASK];
	r_out++;
	
	return(c);
}

void UART_first_init(void) 
{
	/* First init for the UART */

	UBRRL = 25;										// 19200 BPS
	UCSRB = (1<<RXCIE)|(1<<TXEN)|(1<<RXEN);		// 8 Databits, receive and transmit enabled, 
													//receive and transmit complete interrupt enabled
	
	fdevopen(UART_putchar, UART_getchar, 0);
	sei();												// Global interrupt enable
}

void function (void) 
{
	unsigned char name[20] ;
	printf_P(PSTR("Enter your name\r\n"));
	while (scanf("%s",name)==0);
	printf_P(PSTR("\r\nYour name is '%s'\r\n"),name);
}




int main(void) 
{
//add by narongsak for set external interrupt
    MCUCR = 0x28;			//Int 1 on Low level ??? May need to change to low for key press > 3 sec
    GICR = 0x80;           // enable external int1
	
//add by narongsak for set direction exterrupt and LED GREEN 	
	DDRD  =	 ~(1<<DDD3);			//set derecttion input int1
	PORTD =   (1<<PD3);				// pull up enable int1
    DDRC  =    1<<DDC2;
	PORTC =  ~(1<<PC2);
	PORTC =   (1<<PC2);
//add by narongsak for set timer interrupt
	TCCR0  = 0x05;						// Clock/1024 for time0
	TIMSK  = TIMSK | ( 1<< TOIE0);
	
	
	unsigned char s[20];
	UART_first_init();				// First init UART
//	send_boot();
	for(;;) 
	{
		/* Same function as printf() but the format string resides in flash */
		printf_P(PSTR("Enter text\r\n"));
		
		while(scanf("%s",s) == 0);

		printf_P(PSTR("\r\nYou entered '%s'\r\n"),s);
//		function ();

	}
}



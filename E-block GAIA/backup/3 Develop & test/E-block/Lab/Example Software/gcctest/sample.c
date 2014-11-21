/************************************************
MIT Solar Electric Vehicle Team
Michael Seeman '04

AVR Sample Code : Blink LED with Timer

-- sample.c --

Created: 9/30/02
Last Modified: 10/7/02
************************************************/

#include <io.h>
#include <sig-avr.h>
#include <interrupt.h>
#include "sample.h"

/* global variable definitions */

/* volatile means value can change without explicit assignment */
volatile unsigned char  timer_step = 0;

int main (void)
{
  // char    error = 0;

  /* Initialize port directions */
  outp(port_a_io, DDRA);
  outp(port_b_io, DDRB);
  outp(port_c_io, DDRC);
  outp(port_d_io, DDRD);

  /* Initialize timer */
  timer_init();
  /* Enable interrupts */
  sei();
  
  /* Define local variables */
  unsigned char  save=0;
  char state=0;

  /* do an infinite loop */
  for (;;)
  {
	/* check to see if 8 timer_step ticks have elapsed (about .5 sec) */
	if (((unsigned char)(timer_step - save)) > 8) 
	{
	/* reset save */
		save = timer_step;
	/* toggle state of LED */
		if (state)
		{
			cbi(PORTD, io_led_heartbeat);
			state=0;
		}	
		else
		{
			sbi(PORTD, io_led_heartbeat);
			state=1;
		}
	}
  }
  /* note: the AVR doesn't like falling off the end of the program
     thus, it is good to have an infinite loop somewhere in your code */
}

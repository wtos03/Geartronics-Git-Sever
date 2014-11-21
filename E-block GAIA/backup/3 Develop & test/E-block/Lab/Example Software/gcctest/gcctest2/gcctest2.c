/*
    Title:    AVR-GCC test program #2 for the STK200 eva board
    Author:   Volker Oth
    Date:     4/1999
    Purpose:  Flashes the LEDs on Port B by using a timer interrupt
    needed
    Software: AVR-GCC to compile
    needed
    Hardware: ATS90S8515/8535/2313/mega on STK200/300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#include <io.h>
#include <interrupt.h>
#include <signal.h>

unsigned char led;


SIGNAL(SIG_OVERFLOW0)        /* signal handler for tcnt0 overflow interrupt */
{
    outp(~led, PORTB);       /* invert the output since a zero means: LED on */
    led <<= 1;               /* move to next LED */
    if (!led)                /* overflow: start with Port B0 again */
        led = 1;
    outp(0, TCNT0);          /* reset counter to get this interrupt again */
}


int main(void)
{
    outp(0xff,DDRB);         /* use all pins on PortB for output */

    outp((1<<TOIE0), TIMSK); /* enable TCNT0 overflow */
    outp(0,   TCNT0);        /* reset TCNT0 */
    outp(5,   TCCR0);        /* count with cpu clock/1024 */

    led = 1;                 /* init variable representing the LED state */
    sei();                   /* enable interrupts */
    for (;;) {}              /* loop forever */
}
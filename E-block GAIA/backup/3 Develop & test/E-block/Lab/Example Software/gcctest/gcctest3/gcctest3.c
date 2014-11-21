/*
    Title:    AVR-GCC test program #3 for the STK200 eva board
    Author:   Volker Oth
    Date:     4/1999
    Purpose:  Falling edge on Port D2 creates the external interrupt INT0
              which toggles 4 LEDs on PortB. Riding edge on Port D3
              creates external INT1 which toggles the other 4 LEDs.
              Mega: INT0, INT are always low activated and triggered by
              input on Port D0, D1.
    needed
    Software: AVR-GCC to compile
    needed
    Hardware: ATS90S8515/8535/Mega on STK200/STK300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#include <io.h>
#include <signal.h>
#include <interrupt.h>

#ifdef AVR_ATmega103
  #define AVR_MEGA 1
#else
  #ifdef AVR_ATmega603
    #define AVR_MEGA 2
  #else
    #ifdef AVR_ATmega161
      #define AVR_MEGA 3
    #else
      #define AVR_MEGA 0
    #endif
  #endif
#endif


typedef unsigned char  u08;


SIGNAL(SIG_INTERRUPT0)     /* signal handler for external interrupt int0 */
{
    register u08 led = inp(PORTB);

    if (led & 1)
        led &= ~0x0f;
    else
        led |=  0x0f;
    outp(led, PORTB);     /* turn LEDs on */
}


SIGNAL(SIG_INTERRUPT1)     /* signal handler for external interrupt int1 */
{
    register u08 led = inp(PORTB);

    if (led & 0x80)
        led &= ~0xf0;
    else
        led |=  0xf0;
    outp(led, PORTB);     /* turn LEDs on */
}


int main(void)
{
    outp(0xff, DDRB);      /* use PortB for output (LEDs) */
    outp(0x00, DDRD);      /* use PortD for input (switches) */
#if AVR_MEGA
    outp((1<<INT0)|(1<<INT1), EIMSK); /* enable external int0, int1 */
#else
    outp((1<<INT0)|(1<<INT1), GIMSK); /* enable external int0, int1 */
    outp((1<<ISC01)|(1<<ISC10)|(1<<ISC11), MCUCR);
                           /* falling egde: int0, rising edge: int 1 */
#endif
    sei();                 /* enable interrupts */
    for (;;) {}            /* loop forever */
}
/*
    Title:    timer library
    Author:   Volker Oth
    Date:     5/1999
    Purpose:  starting 16 bit timer.
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
#include "global.h"
#include "timer.h"

u16 timer_save;

void timer_init(void) 
/* initialize timer 1 */
{
    outp(0,   TIMSK);                      /* disable timer interrupts */
    outp(0,   TCCR1A);                     /* disable PWM and stuff */
    outp(0,   TCNT1H);                     /* reset TCNT1 */
    outp(0,   TCNT1L);                     /* reset TCNT1 */
    outp(PRSC1_SELECT, TCCR1B);            /* count with cpu clock/PRSC1 */
}
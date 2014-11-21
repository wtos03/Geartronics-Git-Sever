/* timer.c */

/* include these header files */
#include <sig-avr.h>  /* system header files are surrounded by <>'s */
#include <io.h>
#include "sample.h"   /* user header files are surrounded by "'s */

/* external global variable: defined in sample.c, but used here */
extern volatile unsigned char timer_step;

/* An interrupt routine -- you can call it, but the AVR processor calls
   it when the timer expires -- see <io-xxxx.h> for interrupt types */
SIGNAL(SIG_OVERFLOW1)
{
  __outw_atomic(0, TCNT1L);      /* reset TCNT1 */
                                 /* (maybe should use __outw_atomic */
  outp(PRSC1_SELECT, TCCR1B);    /* set count rate */

  timer_step++;                  /* increment slow counter */
}

/* function */
void timer_init(void)
{
  outp((1<<TOIE1), TIMSK);       /* enable timer interrupt */
  outp(0, TCCR1A);               /* disable PWM */
  __outw_atomic(0, TCNT1L);      /* reset TCNT1 */
  outp(PRSC1_SELECT, TCCR1B);    /* set count rate */
}

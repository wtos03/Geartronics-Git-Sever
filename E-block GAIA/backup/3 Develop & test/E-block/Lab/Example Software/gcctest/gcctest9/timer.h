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

#ifndef TIMER_H
#define TIMER_H


/* constants/macros */
#define F_CPU        4000000               /* 4MHz processor */

#define PRSC1              1  
#define PRSC1_SELECT       1               /* divide by 1 */

#define F_T1     (F_CPU/PRSC1)             /* TCNT1 frequency */

#define T_T1     (1000000*PRSC1/F_CPU)     /* us per timer tick */
#define TICKS_T1 (F_CPU/(1000000*PRSC1))   /* ticks per us */

#define get_ticks() __inw_atomic(TCNT1)

#define timer_start(t) (t) = get_ticks()
#define timer_delta(t) ( (u16)get_ticks() - (u16)(t) )

extern u16 timer_save;

extern void timer_init(void);

#endif
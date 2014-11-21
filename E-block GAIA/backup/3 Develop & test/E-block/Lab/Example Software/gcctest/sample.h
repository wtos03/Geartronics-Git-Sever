/* sample.h */
/* Header files specify constants and function prototypes (descriptions)
   used by multiple files */

/* Pre-processor directives to ensure header is #included only once */
#ifndef __sample_h__
#define __sample_h__

/* I/O constants */
#define port_a_io    0x00  /* in-out port specification for ports A-D */
#define port_b_io    0xBF  /* a '1' bit indicates a output pin */
#define port_c_io    0xFF
#define port_d_io    0xB1

/* pin numbers */
#define io_led_heartbeat    4

/* constants */
#define F_CPU        8000000            /* 8.000 MHz clock */

#define PRSC1        8
#define PRSC1_SELECT 2                  /* divide by 8 */

#define F_T1         (F_CPU/PRSC1)      /* timer frequency */

/* macros */
/* macros look like functions, but their definitions are substituted 
   (literally) into the code when compiled */

#define get_ticks() __inw_atomic(TCNT1L)

#define timer_start(t) (t)=get_ticks()
#define timer_delta(t) ((u16)get_ticks()-(u16)(t))

/* Function prototypes */
/* prototypes tell the compiler what the function looks like, even though
   it may be located in a different file -- prototypes are usually required */
void timer_init(void);

#endif


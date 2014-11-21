/*
    Title:    AVR-GCC test program #9 for the STK200 eva board
    Author:   Volker Oth
              Thanks to Jack Tidwell and Mike Dvorsky for the ASM 
              SQRT routines.
    Date:     7/2000
    Purpose:  Calculates the square root of unsigned 16bit values
              and outputs the 8bit results bitwise using the LEDs on
              PortB. 
              Also the results and calculation times are printed via 
              UART (9600 baud, 8 data bits, 1 stop bit, no parity)
              -shows how to inteface an assembler module in C.
              -implements simplified sprintf
              -implements buffered UART I/O
              -implememnts highres time measurement using 16bit TCNT1
    needed
    Software: AVR-GCC to compile
    needed
    Hardware: ATS90S8515/8535/2313/mega on STK200/300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/


#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <interrupt.h>
#include "global.h"
#include "timer.h"
#include "uart.h"


#define SCRATCH 16


/* function prototype for external asm functions */
extern u08 sqrtJack(u16 num);
extern u08 sqrtMike(u16 num);


int sprintf(u08 *buf, const u08 *format, ...)
/* simplified sprintf */
{
  u08 scratch[SCRATCH];
  u08 format_flag;
  u16 u_val=0, base;
  u08 *ptr;
  va_list ap;

  va_start (ap, format);
  for (;;){
    while ((format_flag = *format++) != '%'){      /* Until '%' or '\0' */
      if (!format_flag){va_end (ap); return (0);}
      *buf = format_flag; buf++; *buf=0;
    }

    switch (format_flag = *format++){

    case 'c':
      format_flag = va_arg(ap,int);
    default:
      *buf = format_flag; buf++; *buf=0;
      continue;
    case 'S':
    case 's':
      ptr = va_arg(ap,char *);
      strcat(buf, ptr);
      continue;
    case 'o':
      base = 8;
      *buf = '0'; buf++; *buf=0;
      goto CONVERSION_LOOP;
    case 'i':
      if (((int)u_val) < 0){
        u_val = - u_val;
        *buf = '-'; buf++; *buf=0;
      }
      /* no break -> run into next case */
    case 'u':
      base = 10;
      goto CONVERSION_LOOP;
    case 'x':
      base = 16;

    CONVERSION_LOOP:
      u_val = va_arg(ap,int);
      ptr = scratch + SCRATCH;
      *--ptr = 0;
      do {
        char ch = u_val % base + '0';
        if (ch > '9')
          ch += 'a' - '9' - 1;
        *--ptr = ch;
        u_val /= base;
      } while (u_val);
      strcat(buf, ptr);
      buf += strlen(ptr);
    }
  }
}



int main(void)
{
    u08 mike,jack;                  /* to store sqrt results */
    u16 num;                        /* number to calculate sqrt of */
    u16 ticks;                      /* timer 1 ticks needed for sqrt */
    u08 out[32];                    /* uart output buffer */

    timer_init();                   /* init timer1 */
    uart_init();                    /* init the UART transmit buffer */
    outp(0xff,DDRB);                /* use all pins on PortB for output */
    sei();                          /* enable interrupts */
   
    uart_putstr("GccTest9 started...\r\n");    

    for (;;)                        /* loop forever */
        for (num=0; num<0xffff; num++) {
            sprintf(out, "SQRT(%u) = \r\n", num);
            uart_putstr(out);

            ticks = get_ticks();    /* start measurement */
            mike = sqrtMike(num);   /* calculate sqrt using algorithm 1 */
            ticks = timer_delta(ticks);
            sprintf(out, "    Mike: %u (%u us)\r\n", mike, (u16)(ticks/TICKS_T1));
            uart_putstr(out);       /* print via UART */

            ticks = get_ticks();    /* start measurement */
            jack = sqrtJack(num);   /* calculate sqrt using algorithm 2 */
            ticks = timer_delta(ticks);
            sprintf(out, "    Jack: %u (%u us)\r\n", jack, (u16)(ticks/TICKS_T1));
            uart_putstr(out);       /* print via UART */

            if (mike==jack)
                outp(~mike, PORTB); /* invert the output since a zero means: LED on */
            else
                outp(0, PORTB);     /* error: all LEDs on */
        }
}
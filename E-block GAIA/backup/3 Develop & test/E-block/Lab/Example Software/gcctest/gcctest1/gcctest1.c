/*
    Title:    AVR-GCC test program #1 for the STK200 eva board
    Author:   Volker Oth
    Date:     4/1999
    Purpose:  Flashes the LEDs on Port B with a hard coded delay loop
    needed
    Software: AVR-GCC
    needed
    Hardware: ATS90S8515/8535/2313/mega on STK200/STK300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#include <AVR/io.h>

typedef unsigned char  u08;

int main( void )
{
   u08 led, i, j, k;
   DDRC	= 0x0E;

//   outp(0xff,DDRC);            /* use all pins on PortB for output */

   led = 1;                    /* init variable representing the LED state */

   for (;;) 
   {

		PORTD = ~led;
//       outp(~led, PORTC);      /* invert the output since a zero means: LED on */
       led <<= 1;              /* move to next LED */
       if (!led)               /* overflow: start with Port B0 again */
           led = 1;
       for (i=0; i<255; i++)   /* outer delay loop */
           for(j=0; j<255;j++) /* inner delay loop */
               k++;            /* just do something - could also be a NOP */
   }
}

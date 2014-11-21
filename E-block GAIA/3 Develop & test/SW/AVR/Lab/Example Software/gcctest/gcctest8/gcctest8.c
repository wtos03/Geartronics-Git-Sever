/*
    Title:    AVR-GCC test program #8 for the STK200 eva board
    Author:   Volker Oth
    Date:     1/2000
    Purpose:  demonstrates how to access data in program memory
              which is not copied to RAM.
    needed    
    Software: AVR-GCC
    needed
    Hardware: ATS90S8515/8535/2313/mega on STK200/STK300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#include <io.h>
#include <progmem.h>


typedef unsigned char  u08;

u08 __attribute__ ((progmem)) leds[]={0xff, 0xe7, 0xc3, 0x81, 0x00, 0x81, 0xc3, 0xe7};        

int main(void)
{
    u08 i, j, k, l;

    outp(0xff,DDRB);                /* use all pins on PortB for output */

    for (;;) {
        for (l=0; l<sizeof(leds); l++) {
            outp(PRG_RDB(&leds[l]), PORTB);

            for (i=0; i<255; i++)   /* outer delay loop */
                for(j=0; j<255;j++) /* inner delay loop */
                    k++;            /* just do something - could also be a NOP */
       }
    }
}

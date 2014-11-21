//
// ledtest.c
//
// simple test program for the bootloader 
//
// Copyright 2006 by Erik Lins, www.chip45.com
//
// Subversion keywords
//
// $Rev: 11 $
// $Date: 2006-05-23 16:04:42 +0200 (Di, 23 Mai 2006) $
// $Author: linserik $
//


// includes
#include <avr/io.h>


// defines
#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED_PIN  PINB
#define LED      PINB2


//
// main function
//
int main(void) {

    uint32_t l;

    // set LED pin to output and switch on LED
    LED_DDR |= _BV(LED);
    LED_PORT &= ~_BV(LED);

    // endless loop, blink LED
    while(1) {
	LED_PORT &= ~_BV(LED);
	for(l = 0; l < (F_CPU / 5); ++l);
	LED_PORT |= _BV(LED);
	for(l = 0; l < (2 * F_CPU); ++l);
    }
}


//
// end of file ledtest.c
////////////////////////////////////////////////////////////

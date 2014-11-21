#include <avr/io.h>
#include <avr/interrupt.h>
#include "twi.h"

#define DS1631 0x48						// address of the DS1631 thermometer

void wait(void)
{
	asm volatile("nop"::);
}

int main(void)
{
	unsigned char temp[2];
	unsigned int j;

	DDRB = 0x3F;

	TWI_first_init();
	sei();
		
	TWI_write(DS1631,"\xAC\x0C",2);  	// Continues conversion mode, 12 bit precision
	TWI_write(DS1631,"\x51",1);		// Start conversion
	TWI_write(DS1631,"\xAA",1);		// Start reading temperature
	
	for(;;) {
		for(j=0; j<50000; j++) wait();// Wait a while, the GCC optimizer trashes empty loops so a volatile nop is inserted 
		TWI_read(DS1631,temp,2);		// Read the temperature

		PORTB = ~temp[0];
	}
}








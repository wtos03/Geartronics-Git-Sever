#include <avr/io.h>
#include <studio.h>

int main (void)
{
	UCR = 0x18;
	UBBR = 0x33;
	
	printf ("\n\rYabba DAbba Do");
	while (1)
	;
}
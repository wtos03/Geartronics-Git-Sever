#include "LPC213x.h>

// this functione use to initial pin
// set column to output
// set row to input
void init_keypad(void)
{
	PINSEL0 &= 0x3FFFFFFF;  // use pin 0.15 
	PINSEL2 &= 0xFFFFF000;	// use pin 1.16 - pin 1.21
	IODIR1 |= 0x00380000;	// use pin 1.19 - pin 1.21 as output 
	IODIR1 &= 0xFFF8FFFF;	// use pin 1.16 - pin 1.18 as input 
	IODIR0 &= 0xFFFF7FFF;   // use pin 0.15 as input
	IOSET1 |= 0x00070000;	// set logic pin 1.16 - pin 1.18 to 1
	IOSET0 |= 0x00004000;	// set logic pin 0.15 to 1
}

// this function use to delay program by ms millisecond
void delay_ms(long int ms)
{
	long int i,j;
	for(i=0;i<ms;i++)
	for(j=0;j<6666;j++);
}


// this function use to detect key that has been press
// program will send column pin logic 0
// if key has been press, row of pressed key will change logic to 0
// then program will know which row has press
// program will check column by change logic 0 each a round
// which round make that row change logic 0
// ,that mean key is in that column in round
// after know row and column will know key too
char keypad(void)
{
	char key = '$'; // set default key to $

	IOCLR1 |= 0x00380000; // set all column change logic to 0

	if( (IOPIN1 & 0x00010000) == 0) // row 0 has been press
	{ 	
		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00080000; // set column 0 change logic to 0
		if((IOPIN1 & 0x00010000) == 0) // column 0 has been press then key is 4
		{
			key = '4'; // set key is 4
			while((IOPIN1 & 0x00010000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		}
	
		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00100000; // set column 1 change logic to 0 	
		if((IOPIN1 & 0x00010000) == 0) // column 1 has been press then key is 5
		{
			key = '5'; // set key is 5
			while((IOPIN1 & 0x00010000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		} 

		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00200000; // set column 2 change logic to 0 	
		if((IOPIN1 & 0x00010000) == 0) // column 2 has been press then key is 6
		{
			key = '6'; // set key is 6
			while((IOPIN1 & 0x00010000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		}
	}

	if( (IOPIN1 & 0x00020000) == 0 ) // row 1 has been press
	{	
		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00080000; // set column 0 change logic to 0 	
		if((IOPIN1 & 0x00020000) == 0) // column 0 has been press then key is 7
		{
			key = '7'; // set key is 7
			while((IOPIN1 & 0x00020000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		}

		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00100000; // set column 1 change logic to 0 	
		if((IOPIN1 & 0x00020000) == 0) // column 1 has been press then key is 8
		{
			key = '8'; // set key is 8
			while((IOPIN1 & 0x00020000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		} 

		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00200000; // set column 2 change logic to 0 	
		if((IOPIN1 & 0x00020000) == 0) // column 2 has been press then key is 9
		{
			key = '9'; // set key is 9
			while((IOPIN1 & 0x00020000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		}
	}
		
	if( (IOPIN1 & 0x00040000) == 0 ) // row 2 has been press
	{	
		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00080000; // set column 0 change logic to 0 	
		if((IOPIN1 & 0x00040000) == 0) // column 0 has been press then key is *
		{
			key = '*'; // set key is *
			while((IOPIN1 & 0x00040000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		}

		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00100000; // set column 1 change logic to 0 	
		if((IOPIN1 & 0x00040000) == 0) // column 1 has been press then key is 0
		{
			key = '0'; // set key is 0
			while((IOPIN1 & 0x00040000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		} 

		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00200000; // set column 2 change logic to 0 	
		if((IOPIN1 & 0x00040000) == 0) // column 2 has been press then key is #
		{
			key = '#';
			while((IOPIN1 & 0x00040000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		}
	}
			
	if( (IOPIN0 & 0x00008000) == 0 ) // row 3 has been press
	{
		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00080000; // set column 0 change logic to 0 	
		if((IOPIN0 & 0x00008000) == 0) // column 0 has been press then key is 1
		{
			key = '1'; // set key is 1
			while((IOPIN0 & 0x00008000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		}

		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00100000; // set column 1 change logic to 0 	
		if((IOPIN0 & 0x00008000) == 0) // column 1 has been press then key is 2
		{
			key = '2'; // set key is 2
			while((IOPIN0 & 0x00008000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		} 

		IOSET1 |= 0x00380000; // set all column change logic to 1
		IOCLR1 |= 0x00200000; // set column 2 change logic to 0 	
		if((IOPIN0 & 0x00008000) == 0) // column 2 has been press then key is 3
		{
			key = '3'; // set key is 3
			while((IOPIN0 & 0x00008000) == 0); // wait until press key has been release
			delay_ms(10); // debounce 
		}
	}
	return key;	// return press key	
}




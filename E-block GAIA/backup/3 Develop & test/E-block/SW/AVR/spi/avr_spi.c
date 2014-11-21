/*  

 Module 				:   Module name
 Description 			:   Module Description
 Original written for 	:   CPU/Microcontroller
 CPU port/tested		:   CPU/Microcontroller
 Hardware use			:   IC Chip
 Port use				:   Port name / Function
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Name / Email
 Date					:  DD/MM/20XX
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
*/



// Include file






// Start routine here


void avr_spi_init(uint8_t bit_rate)
{
// setup SPI I/O pins
	sbi(PORTB, 1);	// set SCK hi
	sbi(DDRB, 1);	// set SCK as output
	cbi(DDRB, 3);	// set MISO as input
	sbi(DDRB, 2);	// set MOSI as output
	sbi(DDRB, 0);	// SS must be output for Master mode to work


// setup SPI interface :
// Set up master/slave  mode
	sbi(SPCR, MSTR);
// Set clock frequency
	// clock = f/4
//	cbi(SPCR, SPR0);
//	cbi(SPCR, SPR1);
	// clock = f/16
	cbi(SPCR, SPR0);
	sbi(SPCR, SPR1);
	// select clock phase positive-going in middle of data
	cbi(SPCR, CPOL);
	// Data order MSB first
	cbi(SPCR,DORD);
	// enable SPI
	sbi(SPCR, SPE);

// clear status
	inb(SPSR);
	spiTransferComplete = TRUE;

	// enable SPI interrupt
	#ifdef SPI_USEINT
	sbi(SPCR, SPIE);
	#endif


// Set Bit rate using variable  bit_rate)
 
	outb(SPCR, (inb(SPCR) & ((1<<SPR0)|(1<<SPR1))) | (bit_rate&((1<<SPR0)|(1<<SPR1)))));
 
/*

  SPI send bytes and receive byte at the same time
  Input  Data (8 bits) to be sent
 Output  Get data 8 bits

*/
 
uint8_t avr_spi_transferByte(uint8_t data)
{
// Use interrupt
 	#ifdef SPI_USEINT
		// send the given data
		while(!spiTransferComplete);
		spiTransferComplete = FALSE;

		outb(SPDR, data);
		// wait for transfer to complete
		while(!spiTransferComplete);
	#else
		while(!(inb(SPSR) & (1<<SPIF)));
		// send the given data
		outb(SPDR, data);
		// wait for transfer to complete
		while(!(inb(SPSR) & (1<<SPIF)));
	#endif
	// return the received data
	return inb(SPDR);

}




/* See need often ...
u16 spiTransferWord(u16 data)
{
	u16 rxData = 0;

	// send MS byte of given data
	rxData = (spiTransferByte((data>>8) & 0x00FF))<<8;
	// send LS byte of given data
	rxData |= (spiTransferByte(data & 0x00FF));

	// return the received data
	return rxData;
}
*/






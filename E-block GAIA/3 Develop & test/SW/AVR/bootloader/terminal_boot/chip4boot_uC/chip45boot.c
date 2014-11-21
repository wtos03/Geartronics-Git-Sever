//
// chip45boot.c
// ------------
//
// chip45boot .srec bootloader for Atmel AVR microcontrollers
//
// Copyright 2006 by Erik Lins, www.chip45.com
//
// License
// -------
// chip45boot is distributed under the Lesser General Public License (LGPL).
// You should have received a copy (lgpl.txt) of the license along with
// this file or go to http://www.gnu.org/licenses/lgpl.html.
//
// Description
// -----------
// The bootloader can receive a Motorola s-record file through USART.
// An interactive mode can be entered after reset and provides simple
// commands.
//
// Usage
// -----
// The bootloader waits a short while for receiving an 'i' from USART.
// (In a terminal program, hold the 'i' key pressed during reset.)
// If so, a prompt "> " will be shown.
// Available commands are:
// 'f' starts the flash programming (the character f is echoed plus an '\r')
//     Now an s-record file (see above) can be uploaded to the controller.
//     During programming a line of '.' is shown, each '.' marks the end of
//     a valid s-record line.
//     When programming has been completed successfully 'OK' is shown and the
//     bootloader starts again.
//     Note: It will start the application, if it doesn't receive an 'i' again!
//     Possible errors during programming are:
//     'B' wrong byte count in s-record
//     'C' wrong checksum in s-record
//     'P' wrong page count, i.e. s-record number of data bytes are not an
//          integer factor of flash page size
// 'g' starts the application, i.e. interrupt vector table is rearranged to
//     bottom of flash and bootloader jumps to 0x0000
// 'e' starts the programming of the EEPROM
//     This command is only available if you #define EEPROM_CODE!
//     It increases bootloader code by about 200 bytes, keep this in mind when
//     setting the fuse bits for boot block size!
//
// S-Record Upload with Terminal Program
// -------------------------------------
// Any terminal program can be used for uploading the .srec file to
// the MCU, e.g. HyperTerminal, which comes with any Windows system or
// TeraTermPro (http://hp.vector.co.jp/authors/VA002416/teraterm.html),
// which I prefer. Since the bootloader receives an s-record (one line
// of the file), then programs the s-record to flash, then receives the
// next s-record, programs it and so on, it is necessary to delay the 
// transmission of each line for the time the bootloader needs to
// program the flash page. For an ATmega8 (64 byte flash page size) running
// at 14.7456MHz a delay of 100msec after each line is sufficient. I will do
// further tests and add the experiences in this text.
// To set this delay with the terminal program do this:
// TeraTermPro: Menu Setup -> Serial Port -> Transmit Delay -> "100" msec/line
// HyperTerminal: Datei -> Eigenschaften -> Einstellungen ->
// ASCII-Konfiguration -> Zeilenverz�gerung "100" Millisekunden
//
// S-Record Upload with chip45prog
// -------------------------------
// chip45prog is a simple command line tool, which can be used to progam
// an MCU target with chip45boot installed.
// See chip45prog.c (included in the ZIP) for details.
// The necessary changes to a regular WinAVR Makefile are listed in
// the chip45prog.c source code.
//
// S-record File Criteria
// ----------------------
// The maximum number of data bytes per s-record (each line of the
// s-record file) is 64. avr-objcopy usually generates s-record files
// with 32 data bytes / s-record -> good for all AVRs!
// The data bytes of the s-record must not be larger than the flash
// page size of the target AVR MCU, but must be an integer factor of
// the flash page size (e.g. 64 byte flash page size and 16, 32 or 64
// data bytes per s-record). Since all major AVRs have at least 64 bytes
// flash page size and common s-records have 32 data bytes per record,
// everything should fine most of the time!
//
// In case you need to generate an s-record file from the normally used
// Intel hex file, srec_cat will do the job. srec_cat comes with WinAVR:
//
// srec_cat input.hex -Intel -Output output.srec -Motorola
//
// You can specify the line length for the s-records:
// srec_cat input.hex -Intel -Output output.srec -Motorola -Line_Length 142
// (the line length of 142 results in a byte count of 64, which is the maximum)
//
// Cheers, ER!K ;-)
//
//
// ToDo
// ----
// - XON/XOFF protocol implementation would avoid line delay in terminal upload
//
// Change History
// --------------
// 17-Feb-2007
// - added todo and change history section in header
// - clear myUCSRB register before startApplication();
// - corrected srecord length calculation for type 2 srecs
//   now code size >64k works
//


// includes
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/boot.h>
#include <util/delay.h>


// include additional MCU definitions to allow compiling
// for different MCUs from same source code
#include "mcudefs.h"


// definitions

// #define this to include code for programming the EEPROM
// it reduced bootloader code size by 124 bytes.
// code size with eeprom code:   1022 bytes
// code size without eeprom code: 898 bytes
// #undef EEPROM_CODE
#define EEPROM_CODE

// CPU frequency is normally defined in the Makefile
#ifndef F_CPU
#define F_CPU 14745600
#endif

// TRUE and FALSE
#define TRUE  1
#define FALSE 0

// UART baudrate
#define BAUDRATE 115200

// memory type to program
#define NONE 0
#define FLASH 1
#define EEPROM 2

// function prototypes
void uartPutChar(char);  // put a character over USART
char uartGetChar(void);  // read (wait for) a character from USART
void uartPutHex(uint8_t);  // print hex byte over USART
void uartPutString(char *);
uint8_t hexCharToInt(char);  // convert hex character to unsigned integer
uint8_t hexByteToInt(char, char);  // convert hex byte (two characters) to unsigned integer
uint8_t parseSrecBuffer(char *);  // parse the current s-record in the buffer
void (*startApplication)( void ) = 0x0000;  // pointer to application at flash start
void (*startBootloader)( void ) = BOOTADR;  // pointer to the bootloader start address


// global variables
char receiveBuffer[150];
char *receiveBufferPointer;
uint8_t receiveBufferFull;
uint32_t writeBaseAddress = 0;
myCOUNTERTYPE flashPageSizeCounter = SPM_PAGESIZE + 2;  // added two, to avoid the (unsigned) counter variable crossing zero
#ifdef EEPROM_CODE
char eepromWriteBuffer[SPM_PAGESIZE+2];  // SPM_PAGESIZE used as eeprom buffer size
#endif
uint8_t srecEndOfFile = FALSE;
uint8_t programThisMemory = NONE;


//
// interrupt routine called when a character was received from USART
//
ISR(myUSART_ReceiveCompleteVect) {

    char c = myUDR;  // save the character

    if((c != '\r') && (c != '\n')) {
	*receiveBufferPointer++ = c;  // write character to the buffer
    } else {
	// if end of line is reached, set buffer full flag
	// this starts s-record processing in the main program
	receiveBufferFull = TRUE;
    }
}


//
// main function
//
int main(void) {

    char c;
    uint16_t loop = 0;
    uint8_t bootloaderEnableFlag = FALSE;


    // relocate interrupt vector table to bottom of flash
    // in case the bootloader will not be started
    myIVSELREG = _BV(IVCE);
    myIVSELREG = 0;        

    // init USART
    myUBRRH = (F_CPU/(BAUDRATE*16L)-1) >> 8;             // calculate baudrate and set high byte
    myUBRRL = (uint8_t)(F_CPU/(BAUDRATE*16L)-1);         // and low byte
    myUCSRB = _BV(myTXEN) | _BV(myRXEN) | _BV(myRXCIE);  // enable transmitter and receiver and receiver interrupt
    myUCSRC = myURSEL | _BV(myUCSZ1) | _BV(myUCSZ0);     // 8 bit character size, 1 stop bit, no parity

    // the bootloader may be activated either if
    // the character 'i' (interactive mode) was received from USART
    // or the flash is (still) empty

    // poll USART receive complete flag 64k times to catch the 'i' reliably
    do {
	if(bit_is_set(myUCSRA, myRXC))
	    if(myUDR == 'i')
		bootloaderEnableFlag = TRUE;
    } while(--loop);

    // test if flash is empty (i.e. flash content is 0xff)
    if(pgm_read_byte_near(0x0000) == 0xFF) {
	bootloaderEnableFlag = TRUE;  // set enable flag
    }

    // check enable flag and start application if FALSE
    if(bootloaderEnableFlag == FALSE) {
        myUCSRB = 0;  // clear USART register to reset default
        startApplication();  // start application code
    }


    //
    // now the bootloader code begins
    //


    // welcome message and prompt
    uartPutChar('\r');
    uartPutChar('>');

    // loop until a valid character is received
    do {

	c = uartGetChar();  // read a character

	if(c == 'f') {  // 'f' selects flash programming
	    uartPutChar('f');           // echo the 'f'
	    programThisMemory = FLASH;  // set flag
	}

#ifdef EEPROM_CODE
	if(c == 'e') {  // 'e' selects eeprom programming
	    uartPutChar('e');            // echo the 'e'
	    programThisMemory = EEPROM;  // set flag
	}
#endif

	if(c == 'g') {  // 'g' starts the application
	    uartPutChar('g');    // echo the 'g'
	    startApplication();  // and jump to 0x0000
	}

    } while(!programThisMemory);  // exit loop when a valid key was pressed


    // move interrupt vector table to boot loader area
    myIVSELREG = _BV(IVCE);
    myIVSELREG = _BV(IVSEL);
 
    uartPutChar('\r');  // set cursor to next line

    receiveBufferFull = FALSE;  // reset full flag
    receiveBufferPointer = (char *)receiveBuffer;  // reset buffer pointer to start of buffer

    // enable interrupts
    sei();

    // endless loop
    while(1) {

	// if buffer is full, parse the buffer and write to flash
	if(receiveBufferFull) {

	    cli();  // disable interrupts

	    // if parsing produced an error, restart bootloader
	    if(!parseSrecBuffer(receiveBuffer)) {
		uartPutChar('\r');  // set cursor to next line
		startBootloader();  // restart the bootloader
	    }

	    // was an end-of-file s-record found?
	    if(srecEndOfFile) {
		uartPutChar('O');  // 'OK' indicates successful programming
		uartPutChar('K');
		loop_until_bit_is_set(myUCSRA, myUDRE);  // wait until character is transmitted
		startBootloader();       // restart the bootloader
	    }

	    receiveBufferFull = FALSE;  // reset full flag
	    receiveBufferPointer = (char *)receiveBuffer;  // reset buffer pointer to start of buffer

	    sei();  // enable interrupts
	}
    }
}


//
// parse the current s-record in the buffer
//
uint8_t parseSrecBuffer(char *thisBuffer) {

    uint8_t srecBytecount, srecChecksum, srecType;
    uint32_t srecAddress;
    uint8_t tmpAddress;
    char hi, lo;
    char c1, c2;
    uint16_t i;

    // check if current buffer is a data record (starts with S1, S2 or S3)
    if(*thisBuffer++ == 'S' ) {

	// get s-record type
	srecType = hexCharToInt(*thisBuffer++);

	// only process the record in case it's a data record
	if((srecType == 1) || (srecType == 2) || (srecType == 3)) {

	    // get the byte count
	    hi = *thisBuffer++;
	    lo = *thisBuffer++;
	    srecBytecount = hexByteToInt(hi, lo);
	    // one could directly put *thisBuffer++ into the arguments,
	    // but the arguments are put on stack last first, i.e. the
	    // lo character is fetched from the *thisBuffer first and
	    // this changes lo and hi character! Using seperate variables
	    // hi and lo is more clear and readable than changing the 
	    // sequence in the hexByteToInt function.

	    // check if byte count is larger than 0x43, i.e. we have more
	    // than 64 bytes in the record -> not allowed
	    if(srecBytecount > (0x43 + srecType - 1)) {
		uartPutChar('B');  // 'B' indicates this error
		return FALSE;
	    }
	    srecChecksum = srecBytecount;  // add byte count to checksum
	    srecAddress = 0;  // reset s-record address

	    // extract the address depending of s-record type
	    for(i = 0; i <= srecType; ++i) {
		
		hi = *thisBuffer++;
		lo = *thisBuffer++;
		tmpAddress = hexByteToInt(hi, lo);  // get next address byte
		srecAddress <<= 8;  // shift existing address one byte left
		srecAddress += tmpAddress;  // add new lower address byte
		srecChecksum += tmpAddress;  // add address portion to checksum
	    }

	    // read all data bytes
	    for(i = 0; i < (srecBytecount - 3 - (srecType - 1)); i += 2) {

		// assemble a 16 bit little endian data word and calculate checksum
		hi = *thisBuffer++;
		lo = *thisBuffer++;
		c1 = hexByteToInt(hi, lo);
		srecChecksum += c1;

		hi = *thisBuffer++;
		lo = *thisBuffer++;
		c2 = hexByteToInt(hi, lo);
		srecChecksum += c2;

#ifdef EEPROM_CODE
		if(programThisMemory == FLASH) {
#endif
		    // write word to flash write buffer
		    boot_page_fill(srecAddress + i, (c2 << 8) + c1);
#ifdef EEPROM_CODE
		} else {
		    // write hi and lo byte into eeprom buffer
		    eepromWriteBuffer[srecAddress + i] = c1;
		    eepromWriteBuffer[srecAddress + i + 1] = c2;
		}
#endif

		// This counter decrements from SPM_PAGESIZE to two,
		// when two is reached, the flash page may be written.
		// If it goes below two, the bytecount of an s-record
		// was not a integer factor of the flash page size.
		// See description on top of this file.
		if((flashPageSizeCounter -= 2) == 0) {
		    uartPutChar('P');  // 'P' indicates this error
		    return FALSE;
		}
	    }

	    // get checksum and compare to 0xff
	    hi = *thisBuffer++;
	    lo = *thisBuffer++;
	    srecChecksum += hexByteToInt(hi, lo);  

	    // compare checksum to 0xff
	    if(srecChecksum != 0xff) {
		uartPutChar('C');  // if checksum is wrong, 'C' indicates this error
		return FALSE;
	    }

	    uartPutChar('.');  // '.' indicates some progress
	}

	// check if end of file record
	if((srecType == 9) || (srecType == 8) || (srecType == 7)) {
	    srecEndOfFile = TRUE;
	}
    }


    // check if either page size counter is two (i.e. buffer is full)
    // or end of file was reached (i.e. the previously received
    // bytes must be written to flash)
    if((flashPageSizeCounter == 2) || (srecEndOfFile == TRUE)){

#ifdef EEPROM_CODE
	if(programThisMemory == FLASH) {
#endif

	    boot_page_erase(writeBaseAddress);  // do a page erase
	    boot_spm_busy_wait();  // wait for page erase done

	    boot_page_write(writeBaseAddress);  // do a page write
	    boot_spm_busy_wait();  // wait for write completed

	    boot_rww_enable();  // reenable rww section again
#ifdef EEPROM_CODE
	}

	if(programThisMemory == EEPROM) {

	    eeprom_busy_wait();  // wait for eeprom no longer busy
	    eeprom_write_block(eepromWriteBuffer,
			       (void *)(uint16_t)writeBaseAddress,
			       SPM_PAGESIZE);  // write the block to eeprom
	}
#endif

	flashPageSizeCounter = SPM_PAGESIZE + 2;  // set byte counter to correct value (see declaration at beginning)
	writeBaseAddress += SPM_PAGESIZE;
    }

    return TRUE;
}


//
// convert a hex number character into 4 bit unsigned integer
//
uint8_t hexCharToInt(char c) {

    // test if character is letter or number
    if(c <= '9')
	return (c - '0');
    else
	return (c - 'A' + 0xa);

}


//
// convert a hex byte (two characters) into 8 bit unsigned integer
//
uint8_t hexByteToInt(char hi, char lo) {

    return ( (hexCharToInt(hi) << 4) + hexCharToInt(lo) );  // return the unsigned integer
}


//
// send a character through the USART
//
void uartPutChar(char c) {

    if(c == '\r')                            // if character is 'caraige return', then
	uartPutChar('\n');                   // send an additional 'new line'
    loop_until_bit_is_set(myUCSRA, myUDRE);  // wait until transmit buffer is empty
    myUDR = c;                               // write character to transmit buffer
}


//
// receive a character from the USART
//
char uartGetChar(void) {

    loop_until_bit_is_set(myUCSRA, myRXC);  // wait until character is received
    return myUDR;                           // return the character
}


//
// end of file chip45boot.c
////////////////////////////////////////////////////////////

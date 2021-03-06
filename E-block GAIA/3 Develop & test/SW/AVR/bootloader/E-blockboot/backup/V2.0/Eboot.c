/******************************************************************************
 * Filename: EBoot.c
 *
 * Modified  from Boot.c
 * Author: Preston K. Manwaring
 *
 * Date: Monday, August 22, 2005 13:38:36 -0600
 *
 * License: Free to use. Please mention the contributing authors in the source
 * code.
 *
 * Description: This is a bootloader designed for the AVR processor family
 * which allows new firmware to be uploaded via the UART in Intel Hex format.
 * An interface for uploading such as Hyperterm is envisioned.
 * This code correctly parses the Intel Hex strings and upon reception of
 * a complete page (processor specific) of data, writes the data to flash.
 * Minimal error handling is implemented. Errors during program upload are
 *
 * ERR 1 - File upload synchronization is off (i.e. the ':' was not found)
 * ERR 2 - the checksum failed.
 *
 * Typical Intel hex format is supported in unix mode (LF not LF,CR). So
 * be sure to strip the extra CR from the hex file or have this program
 * strip the CR.
 *
 * The user menu is as follows:
 * U - upload the hex file. A 'READY' will return when the MCU is ready to
 *     receive the file. As the file uploads, a '*' prints for every page
 *     written to flash. When complete, 'OK' is transmitted. If an error
 *     occurs, 'ERR' followed by a number is printed.
 *
 * X - exit this program and start the user application.
 *
 * NOTE: This software is not optimized. It fits as it currently stands
 * in a BLS of 1024. Optimizations could be easily made, but the code is 
 * much easier to  understand with the way it currently stands. I spent
 * a lot of time reading manuals and looking at other's code to figure out
 * all I wanted to know. So I hope this is easily understood. Due to the 
 * lack of optimization, the most common error you will get using this
 * code will be ERR 1 as synchronization fails. This failure will occur
 * because your BAUD rate setting is too high and the program cannot
 * keep up the incoming data. Slowing down the data transfer rate should
 * fix this or you can optimize the code.
 *
 * A special thanks needs to go to Martin Thomas for writing some excelent code
 * which helped me get started. In fact, you will see some files and macros 
 * of his which I use including chipdef.h. His project can be found at
 * http://www.siwawi.arubi.uni-kl.de/avr_projects/
 *
 * If you have any questions or comments, feel free to write.
 * manjagu@byu.edu
 
 * Modified by Wichai Tossamartvorakul
 * Date 2 Aug 2007 
 * Test with ATMEGA32 at 16 Mhz
 *
 * To remove pin for select bootloader use keypress from terminal program
 * Enter boot mode condition
 *   	1.No program at 0x0000 ( Read 0xFF )
 * 		2.Press 2 buttons at E_IO while press reset key
 * Enter program mode condition
 * 	    1  There are program at 0x0000  (Checked when press Reset)
 *     	2  Press x  at bootmode  
 *  
 *
 ******************************************************************************/

/*--- INCLUDE FILES ----------------------------------------------------------*/
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include "avr_serial.h"


/* Select Boot Size (select one, comment out the others) */
// MAYBE: #define _B512 
//#define _B1024 
#define _B2048

/* From Martin Thomas */
#include "chipdef.h"

 

/* Bootloader Version */
#define VER_HIGH_BYTE '0'
#define VER_LOW_BYTE  '1'

// TRUE and FALSE
#define TRUE  1
#define FALSE 0

/*--- GLOBAL VARIABLES -------------------------------------------------------*/
unsigned char page_data[SPM_PAGESIZE]; // create temporary location for program 
				       // data
unsigned long buf_address;

/*--- FUNCTION PROTOTYPES ----------------------------------------------------*/
uint8_t get_hex_value(void);
char ihex_load(void);
void (*jump_to_app)(void) = 0x0000;

  

/*--- MAIN -------------------------------------------------------------------*/
int main(void)
{
    char temp_char,tmp;
 	uint8_t bootFlag = TRUE;
    
    cli();
	
    /* the following code moves the interrupt vector pointer to the bootloader
       section. The problem is the compiler doesn't understand where to put
       the IV table. */
    GICR = _BV(IVCE);       
    GICR = _BV(IVSEL); //move interruptvectors to the Boot sector  ** WRITE 0 TO IVCE use = not |
 
    /* The slow baud rate is required because of the intel hex parsing overhead.
       If I could figure out how to enable interrupts in the BLS (compiler issue)
       then a higher speed could be used by switching to an interrupt based
       UART ISR. The code could also be optimized. */
       uart_init( UART_INTERRUPT,UART_8_N_1,UART_38400);   
 
	   // poll USART receive complete flag 64k times to catch the 'i' reliably
 
 
	bootFlag = TRUE;
// test if flash is empty (i.e. flash content is 0xff)
	if(pgm_read_byte_near(0x0000) != 0xFF) {
		bootFlag = FALSE; 
	}
	while (!(tmp  = uart_read()));
	if (tmp == 'b')
	     bootFlag = TRUE;
		 
		 
    uart_write_str ("\nBoot V ");
	uart_write_char (VER_HIGH_BYTE);
	uart_write_char (VER_LOW_BYTE);
	uart_write_str ("\r\n");
    
 
    
    /* this main loop is the user 'menu'. */
    while(bootFlag)                             
    {
			   
	if( (tmp = uart_read()) ) 
	{
 	   switch(tmp)
	   {
		case 'u': // download new program
		{
		    /* erase the main flash excepting BLS */
		    buf_address = 0; 
		    while ( APP_END > buf_address )
		    {
			boot_page_erase(buf_address);	// Perform page erase
			boot_spm_busy_wait();		// Wait until the memory is erased.
			buf_address += SPM_PAGESIZE;
		    }
		    buf_address = 0;

		    /* load new program */
			uart_write_str("READY \n");
		    if(( temp_char = ihex_load()))
		    {
				uart_write_str("ERR ");
				uart_write_char(temp_char + '0' );
				uart_write_char('\n');
		    }
		    else
			{
				bootFlag = FALSE ;  // Exit to run
			}
		}
		break;
		
		case 'x':                   //Exit upgrade
		{
			GICR = _BV(IVCE); 
			GICR &= ~(_BV(IVSEL) | _BV(IVCE)); //move interruptvectors to the Application sector  Write 0 to IVCE
		    jump_to_app(); // Jump to application sector
//		    wdt_enable(WDTO_15MS); // Enable Watchdog Timer to give reset
		}
		break;
		case '?':
			uart_write_str(" u - Upload or x - Execute \n");
		break;
		default:
		{
		   uart_write_char(tmp);
		}
	   } // end switch
	}  
    }	// end while(1)
// Start to application

	 GICR = _BV(IVCE); 
	 GICR &= ~(_BV(IVSEL) | _BV(IVCE)); //move interruptvectors to the Application sector  Write 0 to IVCE
	 jump_to_app(); // Jump to application sector

    return 0;
}

/*--- char ihex_load( void ) -------------------------------------------------*/
/* This function pulls intel hex formatted code from the serial port and loads 
   it into a temporary location. Once a complete SPM page length is stored,
   it executes a flash memory write.
    Return Error
	* ERR 1 - File upload synchronization is off (i.e. the ':' was not found)
	* ERR 2 - the checksum failed.

 */
char ihex_load(void)
{
    uint8_t
	i,temp_char,
	byte_count = 0,
	data_pairs,
	data_type,
	temp_byte,
	temp_store,
	address_lo,
	address_hi;

    unsigned int data; // temporary location to store program words
    
    unsigned long temp_address = 0ul;   //Tmp page fill address ??
	
	
    do
    {
	// Wait for characters 
	    do
		{
			temp_char = uart_read();
			if ( (temp_char == 13)  || (temp_char == 10))   // get and dispose of the CR or LF
					temp_char = 0;                           // Wait for first character of line
		} while (!temp_char);
	   if(temp_char != ':') // check to make sure the first character is ':'
	   {
	    return(1);
	   }
	
	
	/* get the count of data pairs */
 
	data_pairs  = get_hex_value();
	
	/* get the address to put the data */
	/* although we collect this data, we do not use it.  All data
	   programmed through this bootloader starts at application program
	   space location 0x0000. The collection is neccessary only for
	   the checksum calculation. */
	address_hi = get_hex_value();
	address_lo = get_hex_value();
	
	/* get the data type */
	data_type  = get_hex_value();
	
	temp_store = data_pairs + address_hi + address_lo + data_type;
	
	for( i = 0; i < data_pairs; i++ )
	{

		temp_byte = get_hex_value();		
	    page_data[byte_count] = temp_byte;	
		byte_count++;
	    temp_store += temp_byte;
	}
	    
	/* get the checksum */
    temp_byte = get_hex_value();
	

	

	/* check the data and checksum */
	if( (char)(temp_store + temp_byte) )
	{
	    return(2);
	}

	/* fill the rest of the page buffer with 0xFF if the last records are not 
	   a full page in length */
	if( data_type)   // End of File record 
	{
	
//???		for (i= 0 ; i < 10 ;i++);
		
		for(byte_count; byte_count < SPM_PAGESIZE; byte_count++ )
	    {
			page_data[byte_count] = 0xFF;
	    }
	}

	/* if the page buffer is full, write the buffer to the temp flash buffer */
	if( byte_count >= SPM_PAGESIZE )
	{
	    byte_count = 0;

	    /* store data in temp buffer and write to flash */
	    for( i = 0; i < SPM_PAGESIZE; i += 2 )
	    {
		/* swap the bytes for little endian format */
		data = page_data[i];
		data |= ( page_data[ i + 1 ] << 8 );
		boot_page_fill( temp_address, data ); // call asm routine to load temporary flash buffer
		temp_address += 2; // select next word in temporary buffer
	    }

	    /* write to the flash */
	    boot_page_write( temp_address - SPM_PAGESIZE );
	    boot_spm_busy_wait();	
	    boot_rww_enable();				//Re-enable the RWW section
	    uart_write_char('*');
	}
 
   } while( data_type != 1);            // Do while not end of File
	
	uart_write_char('E');                // End of file
    return(0);
}


/* This fuction returns the hex value of the  2 consequence received ascii characters (HEX) */
uint8_t get_hex_value()
{
     uint8_t data;
	 uint8_t temp_byte;
    while (!(data = uart_read()));
// Put here to save calling subroutine overhead	
	 if( data < 'A' )
    {
	   data  -= '0';
    }
    else
    {
	
   data -=  55 ;
    }
	temp_byte = data << 4;
	 while (!(data = uart_read()));
// Put here to save calling subroutine overhead	
	 if( data < 'A' )
    {
	   data  -= '0';
    }
    else
    {
	   data -=  55 ;
    }
	
	temp_byte |= data;
    return temp_byte;
    
}



/* 
 Include file for led_matrix.c
 
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   21/3/2006
 
 Summary for include files
 -------------------------
 Constant variable				X
 Function prototype				-
 Variable (Flash memory)		-
 Memory usage					0 bytes
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
*/



/* External Dependents    Need outside parameters 
CPU_CLOCK_SPEED

include  file 			chargen.h
*/


#include "cpu.h"

 
// MMC card chip select pin defines
#define SD_CS_PORT			PORTB
#define SD_CS_DDR			DDRB
#define SD_CS_PIN			0



// Function prototypes
 
// function prototypes
// SPI interface initializer
void avr_spi_init(uint8_t bit_rate)

// spi TransferByte waits until the SPI interface is ready
// and then sends a single byte over the SPI port.  The function also
// returns the byte that was received during transmission.
uint8_t avr_spi_transferByte(uint8_t data)

#endif




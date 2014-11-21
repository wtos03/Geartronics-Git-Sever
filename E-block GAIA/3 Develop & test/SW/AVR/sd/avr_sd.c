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

 


//----- Include Files ---------------------------------------------------------
#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support


#include "avr_spi.h"		// include spi bus support
#include "avr_sd.h"
 

// Global variables

// Functions
uint8_t avr_sd_init(void)
{
	uint8_t i;
	uint8_t retry =0;
	uint8_t r1=0;
	
// initialize SPI interface
	avr_spi_init();
// release chip select
	sbi(SD_CS_DDR, SD_CS_PIN);
	sbi(SD_CS_PORT,SD_CS_PIN);

	do
	{
// send dummy bytes with CS high before accessing
	for(i=0;i<10;i++) spiTransferByte(0xFF);
// resetting card, go to SPI mode
	r1 = mmcSendCommand(SD_GO_IDLE_STATE, 0);
 
// do retry counter
	retry++;
	if(retry>10) return -1;
	}  while(r1 != 0x01);

// TODO: check card parameters for voltage compliance
// before issuing initialize command

	retry = 0;
	do
	{
// initializing card for operation
	r1 = mmcSendCommand(MMC_SEND_OP_COND, 0); 
// do retry counter
	retry++;
	if(retry > 50) return -1;
	} while(r1);
		
// turn off CRC checking to simplify communication
	r1 = mmcSendCommand(MMC_CRC_ON_OFF, 0);
// set block length to 512 bytes
	r1 = mmcSendCommand(MMC_SET_BLOCKLEN, 512);
// return success
	return 0;
}

uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg)
{
	uint8_t r1;

	uint8_t retry=0;
// assert chip select
	cbi(MMC_CS_PORT,MMC_CS_PIN);

// send command
	spiTransferByte(cmd | 0x40);
	spiTransferByte(arg>>24);
	spiTransferByte(arg>>16);
	spiTransferByte(arg>>8);
	spiTransferByte(arg);
	spiTransferByte(0x95);	
// crc valid only for MMC_GO_IDLE_STATE
// end command
// wait for response
// if more than 8 retries, card has timed-out
// return the received 0xFF

	while((r1 = spiTransferByte(0xFF)) == 0xFF)
		if(retry++ > 8) break;
 
// release chip select
	sbi(MMC_CS_PORT,MMC_CS_PIN);

	return r1;
}

 



 
uint8_t sd_read(uint32_t sector, uint8_t* buffer)
{
	uint8_t r1;
	u16 i;

// assert chip select
	cbi(MMC_CS_PORT,MMC_CS_PIN);
// issue command
	r1 = mmcCommand(MMC_READ_SINGLE_BLOCK, sector<<9);
 // check for valid response
	if(r1 != 0x00)
		return r1;
// wait for block start
	while(spiTransferByte(0xFF) != MMC_STARTBLOCK_READ);
// read in data
	for(i=0; i<0x200; i++)
	{
		*buffer++ = spiTransferByte(0xFF);
	}
// read 16-bit CRC
	spiTransferByte(0xFF);
	spiTransferByte(0xFF);
// release chip select
	sbi(MMC_CS_PORT,MMC_CS_PIN);
// return success
	return 0;
}

uint8_t sd_write(uint8_t sector, uint32_t * buffer)
{
	uint8_t r1;
	uint16_t i;

// assert chip select
	cbi(MMC_CS_PORT,MMC_CS_PIN);
// issue command
	r1 = mmcCommand(MMC_WRITE_BLOCK, sector<<9);
// check for valid response
	if(r1 != 0x00)
		return r1;
// send dummy
	spiTransferByte(0xFF);
// send data start token
	spiTransferByte(MMC_STARTBLOCK_WRITE);
// write data
	for(i=0; i<0x200; i++)
	{
		spiTransferByte(*buffer++);
	}
// write 16-bit CRC (dummy values)
	spiTransferByte(0xFF);
	spiTransferByte(0xFF);
// read data response token
	r1 = spiTransferByte(0xFF);
	if( (r1&MMC_DR_MASK) != MMC_DR_ACCEPT)
		return r1;
// wait until card not busy
	while(!spiTransferByte(0xFF));
// release chip select
	sbi(MMC_CS_PORT,MMC_CS_PIN);
// return success
	return 0;
}




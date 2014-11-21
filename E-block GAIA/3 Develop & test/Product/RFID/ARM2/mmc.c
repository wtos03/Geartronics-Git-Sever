#include <LPC213X.H> /* LPC214x definitions */

#include "type.h"
#include "mmc.h"
#include "spi.h"

BYTE MMCCmd[MMC_CMD_SIZE];
BYTE MMCStatus = 0;

/************************** MMC Init *********************************/
/*
* Initialises the MMC into SPI mode and sets block size(512), returns
* 0 on success
*
*/
int mmc_init()
{
	DWORD i;
	BYTE MMCRDData[10];

	MMCStatus = 0;
	

	PINSEL1 &= 0xFFFFFCFF; // use pin 0.20 as GPIO

	IODIR0 |= SPI_SEL; /* SSEL is output */
	IOSET0 |= SPI_SEL; /* set SSEL to high */

	/* initialise the MMC card into SPI mode by sending 80 clks on */
	for(i=0; i<10; i++)
	{
		MMCRDData[i] = 0xFF;
	}
	SPI_Send( MMCRDData, 10 );
	/* Use MMCRDData as a temporary buffer for SPI_Send() */

	IOCLR0 |= SPI_SEL; /* clear SPI SSEL */

	/* send CMD0(RESET or GO_IDLE_STATE) command, all the arguments
	are 0x00 for the reset command, precalculated checksum */
	MMCCmd[0] = 0x40;
	MMCCmd[1] = 0x00;
	MMCCmd[2] = 0x00;
	MMCCmd[3] = 0x00;
	MMCCmd[4] = 0x00;
	MMCCmd[5] = 0x95;
	SPI_Send( MMCCmd, MMC_CMD_SIZE );
	/* if = 1 then there was a timeout waiting for 0x01 from the MMC */
	if( mmc_response(0x01) == 1 )
	{
		MMCStatus = IDLE_STATE_TIMEOUT;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	} 
	
	/* Send some dummy clocks after GO_IDLE_STATE */
	IOSET0 |= SPI_SEL; /* set SPI SSEL */
	SPI_ReceiveByte();
	IOCLR0 |= SPI_SEL; /* clear SPI SSEL */
	
	/* must keep sending command until zero response ia back. */
	i = MAX_TIMEOUT;
	do
	{
		/* send mmc CMD1(SEND_OP_COND) to bring out of idle state */
		/* all the arguments are 0x00 for command one */
		MMCCmd[0] = 0x41;
		MMCCmd[1] = 0x00;
		MMCCmd[2] = 0x00;
		MMCCmd[3] = 0x00;
		MMCCmd[4] = 0x00;
		/* checksum is no longer required but we always send 0xFF */
		MMCCmd[5] = 0xFF;
		SPI_Send( MMCCmd, MMC_CMD_SIZE );
		i--;
	} while ( (mmc_response(0x00) != 0) && (i>0) );
	
	/* timeout waiting for 0x00 from the MMC */
	if ( i == 0 )
	{
		MMCStatus = OP_COND_TIMEOUT;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}
	
	/* Send some dummy clocks after SEND_OP_COND */
	IOSET0 |= SPI_SEL; /* set SPI SSEL */
	SPI_ReceiveByte();
	IOCLR0 |= SPI_SEL; /* clear SPI SSEL */
	
	/* send MMC CMD16(SET_BLOCKLEN) to set the block length */
	MMCCmd[0] = 0x50;
	MMCCmd[1] = 0x00; /* 4 bytes from here is the block length */
					/* LSB is first */
					/* 00 00 00 10 set to 16 bytes */
					/* 00 00 02 00 set to 512 bytes */
	MMCCmd[2] = 0x00;
	/* high block length bits - 512 bytes */
	MMCCmd[3] = 0x02;
	/* low block length bits */
	MMCCmd[4] = 0x00;
	/* checksum is no longer required but we always send 0xFF */
	MMCCmd[5] = 0xFF;
	SPI_Send( MMCCmd, MMC_CMD_SIZE );

	if( (mmc_response(0x00))==1 )
	{
		MMCStatus = SET_BLOCKLEN_TIMEOUT;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}

	IOSET0 |= SPI_SEL; /* set SPI SSEL */
	SPI_ReceiveByte();
	return 0;
}

/************************** MMC Write Block ***************************/
/* write a block of data based on the length that has been set
* in the SET_BLOCKLEN command.
* Send the WRITE_SINGLE_BLOCK command out first, check the
* R1 response, then send the data start token(bit 0 to 0) followed by
* the block of data. The test program sets the block length to 512
* bytes. When the data write finishs, the response should come back
* as 0xX5 bit 3 to 0 as 0101B, then another non-zero value indicating
* that MMC card is in idle state again.
*
*/
int mmc_write_block_int(WORD block_number,int MMCWRData[MMC_DATA_SIZE/4])
{  
	DWORD blockaddress;
	BYTE Status;

	IOCLR0 |= SPI_SEL; /* clear SPI SSEL */

	/* block size has been set in mmc_init() */
	blockaddress=(block_number<<9);

	/* send mmc CMD24(WRITE_SINGLE_BLOCK) to write the data to MMC card */
	MMCCmd[0] = 0x58;
	/* high block address bits, varh HIGH and LOW */
	MMCCmd[1] = blockaddress >> 24;
	MMCCmd[2] = blockaddress >> 16;
	/* low block address bits, varl HIGH and LOW */
	MMCCmd[3] = blockaddress >> 8;
	MMCCmd[4] = blockaddress;
	/* checksum is no longer required but we always send 0xFF */
	MMCCmd[5] = 0xFF;
	SPI_Send(MMCCmd, MMC_CMD_SIZE );

	/* if mmc_response returns 1 then we failed to get a 0x00 response */
	if((mmc_response(0x00))==1)
	{
		MMCStatus = WRITE_BLOCK_TIMEOUT;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}

	/* Set bit 0 to 0 which indicates the beginning of the data block */
	MMCCmd[0] = 0xFE;
	SPI_Send( MMCCmd, 1 );

	/* send data */
	SPI_Send( (char *) &MMCWRData[0], MMC_DATA_SIZE );

	/* Send dummy checksum */
	/* when the last check sum is sent, the response should come back
	immediately. So, check the SPI FIFO MISO and make sure the status
	return 0xX5, the bit 3 through 0 should be 0x05 */
	MMCCmd[0] = 0xFF;
	MMCCmd[1] = 0xFF;
	SPI_Send( MMCCmd, 2 );
	
	Status = SPI_ReceiveByte();
	if ( (Status & 0x0F) != 0x05 )
	{
		MMCStatus = WRITE_BLOCK_FAIL;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}

	/* if the status is already zero, the write hasn't finished
	yet and card is busy */
	if(mmc_wait_for_write_finish()==1)
	{
		MMCStatus = WRITE_BLOCK_FAIL;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}

	IOSET0 |= SPI_SEL; /* set SPI SSEL */
	SPI_ReceiveByte();
	return 0;
}

int mmc_write_block_char(WORD block_number,char MMCWRData[MMC_DATA_SIZE])
{  
	DWORD blockaddress;
	BYTE Status;

	IOCLR0 |= SPI_SEL; /* clear SPI SSEL */

	/* block size has been set in mmc_init() */
	blockaddress=(block_number<<9);

	/* send mmc CMD24(WRITE_SINGLE_BLOCK) to write the data to MMC card */
	MMCCmd[0] = 0x58;
	/* high block address bits, varh HIGH and LOW */
	MMCCmd[1] = blockaddress >> 24;
	MMCCmd[2] = blockaddress >> 16;
	/* low block address bits, varl HIGH and LOW */
	MMCCmd[3] = blockaddress >> 8;
	MMCCmd[4] = blockaddress;
	/* checksum is no longer required but we always send 0xFF */
	MMCCmd[5] = 0xFF;
	SPI_Send(MMCCmd, MMC_CMD_SIZE );

	/* if mmc_response returns 1 then we failed to get a 0x00 response */
	if((mmc_response(0x00))==1)
	{
		MMCStatus = WRITE_BLOCK_TIMEOUT;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}

	/* Set bit 0 to 0 which indicates the beginning of the data block */
	MMCCmd[0] = 0xFE;
	SPI_Send( MMCCmd, 1 );

	/* send data */
	SPI_Send(&MMCWRData[0], MMC_DATA_SIZE );

	/* Send dummy checksum */
	/* when the last check sum is sent, the response should come back
	immediately. So, check the SPI FIFO MISO and make sure the status
	return 0xX5, the bit 3 through 0 should be 0x05 */
	MMCCmd[0] = 0xFF;
	MMCCmd[1] = 0xFF;
	SPI_Send( MMCCmd, 2 );
	
	Status = SPI_ReceiveByte();
	if ( (Status & 0x0F) != 0x05 )
	{
		MMCStatus = WRITE_BLOCK_FAIL;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}

	/* if the status is already zero, the write hasn't finished
	yet and card is busy */
	if(mmc_wait_for_write_finish()==1)
	{
		MMCStatus = WRITE_BLOCK_FAIL;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}

	IOSET0 |= SPI_SEL; /* set SPI SSEL */
	SPI_ReceiveByte();
	return 0;
}

/************************** MMC Read Block ****************************/
/*
* Reads a 512 Byte block from the MMC
* Send READ_SINGLE_BLOCK command first, wait for response come back
* 0x00 followed by 0xFE. The call SPI_Receive() to read the data
* block back followed by the checksum.
*
*/
int mmc_read_block_int(WORD block_number,int MMCRDData[MMC_DATA_SIZE/4])
{
	WORD Checksum;
	DWORD blockaddress;

	IOCLR0 |= SPI_SEL; /* clear SPI SSEL */

	blockaddress=(block_number<<9);

	/* send MMC CMD17(READ_SINGLE_BLOCK) to read the data from MMC card */
	MMCCmd[0] = 0x51;
	/* high block address bits, varh HIGH and LOW */
	MMCCmd[1] = blockaddress >> 24;
	MMCCmd[2] = blockaddress >> 16;
	/* low block address bits, varl HIGH and LOW */
	MMCCmd[3] = blockaddress >> 8;
	MMCCmd[4] = blockaddress;
	/* checksum is no longer required but we always send 0xFF */
	MMCCmd[5] = 0xFF;
	SPI_Send(MMCCmd, MMC_CMD_SIZE );
	/* if mmc_response returns 1 then we failed to get a 0x00 response */
	
	if((mmc_response(0x00))==1)
	{
		MMCStatus = READ_BLOCK_TIMEOUT;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}
	
	/* wait for data token */
	if((mmc_response(0xFE))==1)
	{
		MMCStatus = READ_BLOCK_DATA_TOKEN_MISSING;
		IOSET0 |= SPI_SEL;
		return MMCStatus;
	}

	/* Get the block of data based on the length */
	SPI_Receive((char *) &MMCRDData[0], MMC_DATA_SIZE );
	/* CRC bytes that are not needed */

	Checksum = SPI_ReceiveByte();
	Checksum = Checksum << 0x08 | SPI_ReceiveByte();

	IOSET0 |= SPI_SEL; /* set SPI SSEL */
	SPI_ReceiveByte();
	return 0;
}

int mmc_read_block_char(WORD block_number,char MMCRDData[MMC_DATA_SIZE])
{
	WORD Checksum;
	DWORD blockaddress;

	IOCLR0 |= SPI_SEL; /* clear SPI SSEL */

	blockaddress=(block_number<<9);

	/* send MMC CMD17(READ_SINGLE_BLOCK) to read the data from MMC card */
	MMCCmd[0] = 0x51;
	/* high block address bits, varh HIGH and LOW */
	MMCCmd[1] = blockaddress >> 24;
	MMCCmd[2] = blockaddress >> 16;
	/* low block address bits, varl HIGH and LOW */
	MMCCmd[3] = blockaddress >> 8;
	MMCCmd[4] = blockaddress;
	/* checksum is no longer required but we always send 0xFF */
	MMCCmd[5] = 0xFF;
	SPI_Send(MMCCmd, MMC_CMD_SIZE );
	/* if mmc_response returns 1 then we failed to get a 0x00 response */
	
	if((mmc_response(0x00))==1)
	{
		MMCStatus = READ_BLOCK_TIMEOUT;
		IOSET0 |= SPI_SEL; /* set SPI SSEL */
		return MMCStatus;
	}
	
	/* wait for data token */
	if((mmc_response(0xFE))==1)
	{
		MMCStatus = READ_BLOCK_DATA_TOKEN_MISSING;
		IOSET0 |= SPI_SEL;
		return MMCStatus;
	}

	/* Get the block of data based on the length */
	SPI_Receive(&MMCRDData[0], MMC_DATA_SIZE );
	/* CRC bytes that are not needed */

	Checksum = SPI_ReceiveByte();
	Checksum = Checksum << 0x08 | SPI_ReceiveByte();

	IOSET0 |= SPI_SEL; /* set SPI SSEL */
	SPI_ReceiveByte();
	return 0;
}

/***************** MMC get response *******************/
/*
* Repeatedly reads the MMC until we get the
* response we want or timeout
*/
int mmc_response( BYTE response)
{
	DWORD count = 0xFFF;
	BYTE result;

	while( count > 0 )
	{
		result = SPI_ReceiveByte();
		if ( result == response )
		{
			break;
		}
		count--;
	}
	if ( count == 0 )
		return 1; /* Failure, loop was exited due to timeout */
	else
		return 0; /* Normal, loop was exited before timeout */
}

/***************** MMC wait for write finish *******************/
/*
* Repeatedly reads the MMC until we get a non-zero value (after
* a zero value) indicating the write has finished and card is no
* longer busy.
*
*/
int mmc_wait_for_write_finish( void )
{
	DWORD count = 0xFFFF; /* The delay is set to maximum considering
	the longest data block length to handle */

	BYTE result = 0;

	while( (result == 0) && count )
	{
		result = SPI_ReceiveByte();
		count--;
	}
	if ( count == 0 )
		return 1; /* Failure, loop was exited due to timeout */
	else
		return 0; /* Normal, loop was exited before timeout */
}
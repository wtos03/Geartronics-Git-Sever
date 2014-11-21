#include <avr/io.h>
#include <avr/twi.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "twi.h"

volatile unsigned char twi_address;
volatile unsigned char *twi_data;
volatile unsigned char twi_ddr;
volatile unsigned char twi_bytes;
volatile unsigned char twi_stop;

volatile unsigned char status;
/* Bit definitions for the tw_status register */
#define BUSY 7

volatile unsigned char retry_cnt;

void TWI_first_init(void)
{
#if defined(TWPS0)
	TWSR = 0;
#endif

	TWBR = (FOSC / 100000UL - 16)/2;
}

int TWI_write(unsigned char address, unsigned char *data, unsigned char bytes)
{
	while(status & (1<<BUSY));							// Bus is busy wait (or exit with error code)
	while(TWCR & (1<<TWSTO));
	
	twi_address = address;
	twi_data = data;
	twi_bytes = bytes;
	twi_ddr = TW_WRITE;

	retry_cnt = 0;
	
	/* Generate start condition, the remainder of the transfer is interrupt driven and
	   will be performed in the background */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
	
	status |= (1<<BUSY);
	return 0;
}

int TWI_read(unsigned char address, unsigned char *data, unsigned char bytes)
{
	while(status & (1<<BUSY));									// Bus is busy wait (or exit with error code)

	twi_address = address;
	twi_data = data;
	twi_bytes = bytes;
	twi_ddr = TW_READ;

	retry_cnt = 0;
	
	/* Generate start condition, the remainder of the transfer is interrupt driven and
	   will be performed in the background */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE);
	
	status |= (1<<BUSY);
	return 0;
}

SIGNAL(SIG_2WIRE_SERIAL)
{
	unsigned char TWI_status = TWSR & TW_STATUS_MASK;			// Mask out prescaler bits to get TWI status
	
	switch(TWI_status) {
	case TW_START:												// Start condition
	case TW_REP_START:											// Repeated start condition
		if(retry_cnt > 2) {
			TWCR |= (1<<TWINT)|(1<<TWSTO);					// Generate stop condition
			status &= ~(1<<BUSY);								// Bus no longer busy
			return;												// If 3 times NACK abort
		}
		TWDR = (twi_address<<1) + twi_ddr;						// Transmit SLA + Read or Write
		TWCR &= ~(1<<TWSTA);									// TWSTA must be cleared by software! This also clears TWINT!!!
		break;

	case TW_MT_SLA_ACK:											// Slave acknowledged address,
		retry_cnt = 0;											// so clear retry counter
		TWDR = *twi_data;										// Transmit data,
		twi_data++;												// increment pointer
		TWCR |= (1<<TWINT);									// and clear TWINT to continue
		break;

	case TW_MT_SLA_NACK:										// Slave didn't acknowledge address,
	case TW_MR_SLA_NACK:
		retry_cnt++;											// this may mean that the slave is disconnected
		TWCR |= (1<<TWINT)|(1<<TWSTA)|(1<<TWSTO);			// retry 3 times
		break;

	case TW_MT_DATA_ACK:										// Slave Acknowledged data,
		if(--twi_bytes > 0) {									// If there is more data to send,
			TWDR = *twi_data;									// Send it,
			twi_data++;											// increment pointer
			TWCR |= (1<<TWINT);								// and clear TWINT to continue
		}
		else {
			TWCR |= (1<<TWSTO)|(1<<TWINT);					// Generate the stop condition if desired
			status &= ~(1<<BUSY);								// Bus no longer busy
		}
		break;

	case TW_MT_DATA_NACK:										// Slave didn't acknowledge data
		TWCR |= (1<<TWINT)|(1<<TWSTO);						// Send stop condition
		status &= ~(1<<BUSY);									// Bus no longer busy
		break;

	case TW_MT_ARB_LOST: 										// Single master this can't be!!!
		break;

	case TW_MR_SLA_ACK:                             			// Slave acknowledged address
		if(--twi_bytes > 0) TWCR |= (1<<TWEA)|(1<<TWINT);	// If there is more than one byte to read acknowledge
			else TWCR |= (1<<TWINT);							// else do not acknowledge
		break;

	case TW_MR_DATA_ACK: 										// Master acknowledged data
		*twi_data = TWDR;										// Read received data byte
		twi_data++;												// Increment pointer
		if(--twi_bytes > 0) TWCR |= (1<<TWEA)|(1<<TWINT);	// Get next databyte and acknowledge	
			else TWCR &= ~(1<<TWEA);							// Enable Acknowledge must be cleared by software, this also clears TWINT!!!
		break;

	case TW_MR_DATA_NACK:										// Master didn't acknowledge data -> end of read process
		*twi_data = TWDR;										// Read received data byte
		TWCR |= (1<<TWSTO)|(1<<TWINT); 						// Generate stop condition
		status &= ~(1<<BUSY);									// Bus no longer busy		
	}
}










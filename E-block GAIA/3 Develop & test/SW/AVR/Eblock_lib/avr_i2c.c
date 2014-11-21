 /*!  
	\file   	avr_i2c.c
	\brief  	I2c module for AVR 
	\ingroup	avr_i2c  
	\par  Interface device using i2C protocol. This module implements for Master mode only.
    \n Communication Module use in AVR
 	\n		- I2C  
 	\n Original written for 	:   \b for bold AVR
 	\n CPU port/tested			:   ATMEGA32
 	\n Hardware use				:   E_MEGA32
 	\n Port use					:   SCL,SDA
	\n Memory	FLASH			:   XXX
 	\n		RAM					:   XXX
 	\n		EEPROM				:   XXX
 	\n Reference Document		:   Document describe algorithm

	\version 0.1
 	\author  wichai@geartronics.net
	\date 	 04/09/08  

	\par Update History
 	\n		Date			By 								Comments
 	\n		----			--								--------
	\n	2002.06.25		 	Pascal Stang  			Take some part   
	\n	2003.03.02		 	Pascal Stang  			Copyright (C) 2002-2003 Revised	
	\note   When write and read eeprom immediatly need to put delay between read and write ??
*/


//! Include file
#include <stdlib.h> 
#include "avr_i2c.h"



static volatile eI2cStateType I2cState; 

inline void i2c_send_start(void)
{
	// send start condition	Clear TWINT Flag by writing logic 1. When TWINT is cleared
	//  Start bit will be sent
    TWCR  =  ((TWCR&TWCR_CMD_MASK)| _BV(TWINT)| _BV(TWEA) |_BV(TWSTA));
}

inline void i2c_send_stop(void)
{
	// transmit stop condition
 	TWCR =  ((TWCR & TWCR_CMD_MASK)| _BV(TWINT)| _BV(TWEA) | _BV(TWSTO));
}

inline void i2c_wait_for_complete(void)
{
	// wait for i2c interface to complete operation
	while( !( TWCR & _BV(TWINT)) );
}

inline void i2c_send_byte(uint8_t data)
{
	// save data to the TWDR
	TWDR = data;
	// begin send
	TWCR  = ((TWCR&TWCR_CMD_MASK) | _BV(TWINT));
}

inline void i2c_set_receive_mode(uint8_t ackFlag)
{
	// begin receive over i2c
	if( ackFlag )    	// ackFlag = TRUE: ACK the recevied data
	{
		TWCR = ((TWCR & TWCR_CMD_MASK) | _BV(TWINT)| _BV(TWEA));
	}
	else  	// ackFlag = FALSE: NACK the recevied data
	{
		TWCR = ((TWCR & TWCR_CMD_MASK) | _BV(TWINT));
	}	
}


/*!	\brief  i2c send byte in Master mode using non interrut
	\param   - deviceAddr of the slave to be sent data
			 - length length of data
			 - *data  Pointer to data buffer
	\return Send status
	\par Port/Periheral Usage 
	\n   SCL,SDA
	\par Theory of Operation   
	\n   Send data in buffer to Slave device which specific in device Addrress.
	\n   To send data. Need to send start signal, slave device address and data. For data,
	\n	 you can send many bytes in one time and end with stop bits
	\par Pseudo code
	\n      -	Send start signal
	\n		-   Wait for complete
	\n		-	Send device address ( Write bit marks)
	\n		-   Wait for complete
	\n		-   Send data until end of buffer
	\n		- 	Send stop bit
	\note    
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
      
 
uint8_t i2c_master_sendni(uint8_t deviceAddr, uint8_t length, uint8_t* data)
{
	uint8_t retval = I2C_OK;

 	// send start condition
	i2c_send_start();
	i2c_wait_for_complete();

	// send device address with write
	i2c_send_byte( deviceAddr & 0xFE );
	i2c_wait_for_complete();

	// check if device is present and live
	if( TWSR == TW_MT_SLA_ACK)
	{
		// send data
		while(length)
		{
			i2c_send_byte( *data++ );
			i2c_wait_for_complete();
			length--;
		}
	}
	else
	{
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = I2C_ERROR_NODEV;
	}

	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2c_send_stop();
	while( !(TWCR & _BV(TWSTO)) );    // ?
	return retval;
}



/*!	\brief  Initial i2C  
	\param   - factor  Division factor for bit rate generator  (SCL Clock Frequency)
			   See avr_i2c.h for Constant
			 - deviceAddr of the slave (Slave mode)
			 - genCallEn General Call recognition over 2 wire bus enable  ??
	\return None
	\par Port/Periheral Usage 
	\n   SCL,SDA
	\par Theory of Operation   
	\n   Init i2c to use in polling mode, Not interrupt mode. Because most communication
	\n   must initial for Master
	\par Pseudo code
	\n      Enable pull up resistor on SCL, SDA
	\n		Set division factor to set bus speed
	\n		Enable i2C inteface and acknowledge bit
	\note    
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
void i2c_master_init(uint8_t factor)
{
	// set pull-up resistors on I2C bus pins
     E_OUT_SCL(1);   		// i2c SCL 
	 E_OUT_SDA(1);   		// i2c SDL
	 
 	// set i2c clock speed	 
	TWBR = factor;
	// enable Acknowledge bit,TWI (two-wire interface)
    TWCR  |=  (_BV(TWEN) | _BV(TWEA));
	// set state
	I2cState = I2C_IDLE;
}



/*!	\brief  Read data from i2c
	\param  deviceAddr 	Device Address to read
	\n		length      Data length to read
	\n		data		ptr to rx buffer
	\return Read Status 
	\par Port/Periheral Usage 
	\n   SCL,SDA  TWI Module
	\par Theory of Operation   
	\n   Read data from i2C by sending start bit, device addrees to read and read data
	\n   until reach length. Then send stop bit
	\par Pseudo code
	\n      Send start bit
	\n		Send device address which mark read bit on
	\n		Read data length bytes
	\n		Send stop bit
	\note    
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
uint8_t i2c_master_read(uint8_t deviceAddr, uint8_t length, uint8_t *data)
{

	uint8_t retval = I2C_OK;
	// send start condition
	i2c_send_start();
	i2c_wait_for_complete();

	// send device address with read
	i2c_send_byte( deviceAddr | 0x01 );
	i2c_wait_for_complete();

	// check if device is present and live
	if( TWSR == TW_MR_SLA_ACK)
	{
		// accept receive data and ack it
		while(length > 1)
		{
			i2c_set_receive_mode(TRUE);   // Set acknowledge 
			i2c_wait_for_complete();
	 		*data++ = TWDR;
			// decrement length
			length--;
		}

		// accept receive data and nack it (last-byte signal)
		i2c_set_receive_mode(FALSE);
		i2c_wait_for_complete();
		*data++ = TWDR;
	}
	else
	{
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = I2C_ERROR_NODEV;
	}

	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2c_send_stop();
 
	return retval;
 
}

/*!	\brief  Write data to i2C port
	\param  deviceAddr  	Device address 
	\n		length			Length of  data to send
	\n		data 			Pointer to write buffer
	\return Error state
	\par Port/Periheral Usage 
	\n   SDA,SCL
	\par Theory of Operation   
	\n   Before start the write operation. Send start command, Device address and data 
	\par Pseudo code
	\n
	\note   (Pseudo code)
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
uint8_t i2c_master_write(uint8_t deviceAddr, uint8_t length, uint8_t* data)
{

	uint8_t retval = I2C_OK;

 	// send start condition
	i2c_send_start();
	i2c_wait_for_complete();

	// send device address with write
	i2c_send_byte( deviceAddr & 0xFE );
	i2c_wait_for_complete();

	// check if device is present and live
	if( TWSR == TW_MT_SLA_ACK)
	{
		// send data
		while(length)
		{
			i2c_send_byte( *data++ );
			i2c_wait_for_complete();
			length--;
		}
	}
	else
	{
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = I2C_ERROR_NODEV;
	}

	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2c_send_stop();
	while( !(TWCR & _BV(TWSTO)) ); // Wait for STOP condition on the bus ???
	return retval;
}
 
 
 
 

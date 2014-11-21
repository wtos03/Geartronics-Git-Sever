/*! \file i2c.c \brief I2C interface using AVR Two-Wire Interface (TWI) hardware. */
//*****************************************************************************
//
// File Name	: 'i2c.c'
// Title		: I2C interface using AVR Two-Wire Interface (TWI) hardware
// Author		: Pascal Stang - Copyright (C) 2002-2003
// Created		: 2002.06.25
// Revised		: 2003.03.02
// Version		: 0.9
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

/* 
 Module 				:   I2C Interface
 Description 			:   Read /Write data via I2C Not using interrupt
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Port use				:	PC0 (SCL)  Serial Clock line)   
							PC1 (SDA)  Serial Data Access 
 Document				:
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   3/4/2007
 
 
 Update History
 
 Date			By 					Comments
 ----			--					---------
2003.03.02   	Pascal Stang      This code is distributed under the GNU Public License
				Copyright (C) 2003				 
 
2007.04.03		Wichai			  Modified to use with E-block project 
 
*/

  

#include <avr/io.h>
//#include <avr/signal.h>
//#include <avr/interrupt.h>

#include "E_i2c.h"



// Standard I2C bit rates are:
// 100KHz for slow speed
// 400KHz for high speed



// I2C state and address variables
static volatile eI2cStateType I2cState;
  
 
// functions
void i2c_init(void)
{
	// set pull-up resistors on I2C bus pins
	// TODO: should #ifdef these
//	sbi(I2C_PORT, 0);	// i2c SCL on ATmega163,323,16,32,etc
//	sbi(I2C_PORT, 1);	// i2c SDA on ATmega163,323,16,32,etc
//	sbi(PORTD, 0);	// i2c SCL on ATmega128,64
//	sbi(PORTD, 1);	// i2c SDA on ATmega128,64

     E_I2C_PORT   |=  _BV(0);	// i2c SCL 
	 E_I2C_PORT   |=  _BV(1);	// i2c SDL
	 
 	// set i2c bit rate to 100KHz
	i2c_set_bitrate(100);
	// enable TWI (two-wire interface)
//	sbi(TWCR, TWEN);
    TWCR  |=  _BV(TWEN);
	// set state
	I2cState = I2C_IDLE;
	// enable TWI interrupt and slave address ACK
//	sbi(TWCR, TWIE);
//	sbi(TWCR, TWEA);
//    TWCR  |=  _BV(TWIE);
// Enavle Acknowledge bit
    TWCR  |=  _BV(TWEA);
// enable interrupts
//	sei();
}

void i2c_set_bitrate(uint16_t bitrateKHz)
{
	uint8_t bitrate_div;
	// set i2c bitrate
	// SCL freq = F_CPU/(16+2*TWBR))
//	
//	#ifdef TWPS0
		// for processors with additional bitrate division (mega128)
		// SCL freq = F_CPU/(16+2*TWBR*4^TWPS)
		// set TWPS to zero
//		cbi(TWSR, TWPS0);
//		cbi(TWSR, TWPS1);
//	#endif
	// calculate bitrate division	
	bitrate_div = ((F_CPU/1000l)/bitrateKHz);
	if(bitrate_div >= 16)
		bitrate_div = (bitrate_div-16)/2;
//	outb(TWBR, bitrate_div);
	TWBR = bitrate_div;
}

void i2c_set_localdeviceaddr(uint8_t deviceAddr, uint8_t genCallEn)
{
	// set local device address (used in slave mode only)
//	outb(TWAR, ((deviceAddr&0xFE) | (genCallEn?1:0)) );
	TWAR  =  (((deviceAddr&0xFE) | (genCallEn?1:0)) );

}
 
 

inline void i2c_send_start(void)
{
	// send start condition	
//	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)| _BV(TWINT)|BV(TWSTA));
    TWCR  =  ((TWCR&TWCR_CMD_MASK)| _BV(TWINT)|_BV(TWSTA));
}

inline void i2c_send_stop(void)
{
	// transmit stop condition
	// leave with TWEA on for slave receiving
//	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)| _BV(TWINT)|BV(TWEA)|BV(TWSTO));
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
//	outb(TWDR, data);
	TWDR = data;
	// begin send
//	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)| _BV(TWINT));
	TWCR  = ((TWCR&TWCR_CMD_MASK) | _BV(TWINT));
}

inline void i2c_receive_byte(uint8_t ackFlag)
{
	// begin receive over i2c
	if( ackFlag )
	{
		// ackFlag = TRUE: ACK the recevied data
//		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|_BV(TWINT)|_BV(TWEA));
	TWCR = ((TWCR & TWCR_CMD_MASK) | _BV(TWINT)| _BV(TWEA));
	
	}
	else
	{
		// ackFlag = FALSE: NACK the recevied data
//		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)| _BV(TWINT));
		TWCR = ((TWCR & TWCR_CMD_MASK) | _BV(TWINT));
	}
}

 

/* I2C Master send Not using interrupt

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
	while( !(TWCR & _BV(TWSTO)) );
 
	return retval;
}

uint8_t i2c_master_receiveni(uint8_t deviceAddr, uint8_t length, uint8_t *data)
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
			i2c_receive_byte(TRUE);
			i2c_wait_for_complete();
	 		*data++ = TWDR;
			// decrement length
			length--;
		}

		// accept receive data and nack it (last-byte signal)
		i2c_receive_byte(FALSE);
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



eI2cStateType i2cGetState(void)
{
	return I2cState;
}

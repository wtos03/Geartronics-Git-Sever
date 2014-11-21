 /*!  \file  	avr_i2c.h			 
      \brief    Include file for i2c (TWI) module for Master 
      \defgroup  avr_i2c
      \ingroup   Eblock_lib
	\nSummary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable							X   
	\n Function prototype				 			3
 	\n Memory usage									?? bytes
	\n  
 
    \par About I2C
	\n	I2C (pronounced "eye-squared-see") is a two-wire bidirectional
	\n	network designed for easy transfer of information between a wide variety
	\n	of intelligent devices.  There are thousands of other parts made by
	\n	manufacturers like Maxim, National, TI, etc that use I2C as
	\n	their primary means of communication and control.  Common device types
	\n	are A/D & D/A converters, temp sensors, intelligent battery monitors,
	\n	MP3 decoder chips, EEPROM chips, multiplexing switches, etc.

	\n	I2C uses only two wires (SDA and SCL) to communicate bidirectionally
	\n	between devices.  I2C is a multidrop network, meaning that you can have
	\n	several devices on a single bus.  Because I2C uses a 7-bit number to
	\n	identify which device it wants to talk to, you cannot have more than
	\n	127 devices on a single bus.

	\n	I2C ordinarily requires two 4.7K pull-up resistors to power (one each on
	\n	SDA and SCL), but for small numbers of devices (maybe 1-4), it is enough
	\n	to activate the internal pull-up resistors in the AVR processor.  To do
	\n	this, set the port pins, which correspond to the I2C pins SDA/SCL, high.
	\n	For example, on the mega163, sbi(PORTC, 0); sbi(PORTC, 1);.
	\n	for complete information about I2C, see the Philips Semiconductor
	\n	website.  They created I2C and have the largest family of devices that
	\n	work with I2C.
	\n  Many manufacturers market I2C bus devices under a different or generic
	\n	bus name like "Two-Wire Interface".  This is because Philips still holds
	\n	"I2C" as a trademark.  For example, SMBus and SMBus devices are hardware
	\n	compatible and closely related to I2C.  They can be directly connected
	\n	to an I2C bus along with other I2C devices are are generally accessed in
	\n	the same way as I2C devices.  SMBus is often found on modern motherboards
	\n	for temp sensing and other low-level control tasks.
	\n  There are 2 modes in operation. Master mode and Slave mode. This library only
	\n  support uC sd Master for this moments.
	
    \code #include <avr_i2c.h> \endcode
 
    \version	0.1
    \author 	wichai@geartronics.net
    \date		5/9/08

    \n \b Update \b History
	\n Date                 By                     Comments
	\n ----                 --                     ---------
    \n  2003.03.03   	Pascal Stang      This code is distributed under the GNU Public License
    \n					  				  Copyright (C) 2003				 

 
 
 
 
*/
 
 

//! External include file
#include "cpu.h"

 




//! Constant variable, For comment in doxygen before the variable declaration ???? 
// 100KHz for slow speed
// 400KHz for high speed 

//! Set frequency
#define  BIT_RATE_100K          72   //   SCL Frequency = (CPU Clock)/(16+2(TWBR).4^TWPS (TWPS0 TWPS1 bit)
		                             //  Clock 16 Mhz, TWPS = 0  
#define  BIT_RATE_400K          12   //   SCL Frequency = (CPU Clock)/(16+2(TWBR).4^TWPS (TWPS0 TWPS1 bit)
		                             //  Clock 16 Mhz, TWPS = 0  
 
 

// TWSR values (not bits)
// (taken from avr-libc twi.h - thank you Marek Michalkiewicz)
// Master
#define TW_START					0x08
#define TW_REP_START				0x10

// Master Transmitter
#define TW_MT_SLA_ACK				0x18
#define TW_MT_SLA_NACK				0x20
#define TW_MT_DATA_ACK				0x28
#define TW_MT_DATA_NACK				0x30
#define TW_MT_ARB_LOST				0x38

// Master Receiver
#define TW_MR_ARB_LOST				0x38
#define TW_MR_SLA_ACK				0x40
#define TW_MR_SLA_NACK				0x48
#define TW_MR_DATA_ACK				0x50
#define TW_MR_DATA_NACK				0x58

// Slave Transmitter
#define TW_ST_SLA_ACK				0xA8
#define TW_ST_ARB_LOST_SLA_ACK		0xB0
#define TW_ST_DATA_ACK				0xB8
#define TW_ST_DATA_NACK				0xC0
#define TW_ST_LAST_DATA				0xC8

// Slave Receiver
#define TW_SR_SLA_ACK				0x60
#define TW_SR_ARB_LOST_SLA_ACK		0x68
#define TW_SR_GCALL_ACK				0x70
#define TW_SR_ARB_LOST_GCALL_ACK	0x78
#define TW_SR_DATA_ACK				0x80
#define TW_SR_DATA_NACK				0x88
#define TW_SR_GCALL_DATA_ACK		0x90
#define TW_SR_GCALL_DATA_NACK		0x98
#define TW_SR_STOP					0xA0

// Misc
#define TW_NO_INFO					0xF8
#define TW_BUS_ERROR				0x00

// defines and constants
#define TWCR_CMD_MASK		0x0F
#define TWSR_STATUS_MASK	0xF8

// return values
#define I2C_OK				0x00
#define I2C_ERROR_NODEV		0x01

#define  TRUE  1
#define  FALSE 0
 
//! types
typedef enum
{
	I2C_IDLE = 0, I2C_BUSY = 1,
	I2C_MASTER_TX = 2, I2C_MASTER_RX = 3,
	I2C_SLAVE_TX = 4, I2C_SLAVE_RX = 5
} eI2cStateType;

//! functions Prototypes

//! Initialize I2C (TWI) interface
void i2c_master_init(uint8_t);
uint8_t i2c_master_read(uint8_t, uint8_t, uint8_t *);
uint8_t i2c_master_write(uint8_t, uint8_t, uint8_t *);
 
 
 
 
 
 

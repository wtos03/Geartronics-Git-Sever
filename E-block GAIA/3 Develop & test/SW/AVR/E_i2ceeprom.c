
/* 
 Module 				:   I2C Eeprom
 Description 			:   Read /Write data from Eeprom 24XX series 
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
2003.04.23   	Pascal Stang      This code is distributed under the GNU Public License
				Copyright (C) 2003				 
 
2007.04.03		Wichai			  Modified to use with E-block project 
 
*/

#include <avr/io.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

#include "E_i2c.h"
#include "E_i2ceeprom.h"

// Standard I2C bit rates are:
// 100KHz for slow speed
// 400KHz for high speed

// functions
void i2ceeprom_init(void)
{
	// although there is no code here
	// don't forget to initialize the I2C interface itself
	  i2c_init();
}

uint8_t i2ceeprom_read_byte(uint8_t i2cAddr, uint32_t memAddr)
{
	uint8_t packet[2];
	// prepare address
	packet[0] = (memAddr>>8);
	packet[1] = (memAddr&0x00FF);
	// send memory address we wish to access to the memory chip
	i2c_master_sendni(i2cAddr, 2, packet);
	// retrieve the data at this memory address
	i2c_master_receiveni(i2cAddr, 1, packet);
	// return data
	return packet[0];
}

void i2ceeprom_write_byte(uint8_t i2cAddr, uint32_t memAddr, uint8_t data)
{
	uint8_t packet[3];
	// prepare address + data
	packet[0] = (memAddr>>8);
	packet[1] = (memAddr&0x00FF);
	packet[2] = data;
	// send memory address we wish to access to the memory chip
	// along with the data we wish to write
	i2c_master_sendni(i2cAddr, 3, packet);
}

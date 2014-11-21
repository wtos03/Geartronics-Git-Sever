/*! \file i2ceeprom.h \brief Interface for standard I2C EEPROM memories. */
//*****************************************************************************
//
// File Name	: 'i2ceeprom.h'
// Title		: Interface for standard I2C EEPROM memories
// Author		: Pascal Stang - Copyright (C) 2003
// Created		: 2003.04.23
// Revised		: 2003.04.23
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
///	\ingroup driver_hw
/// \defgroup i2ceeprom Interface for standard I2C EEPROM memories (i2ceeprom.c)
/// \code #include "i2ceeprom.h" \endcode
/// \par Overview
///		This library provides functions for reading and writing standard
///	24Cxxx/24LCxxx I2C EEPROM memories.  Memory sizes up to 64Kbytes are
///	supported.  Future revisions may include page-write support.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

/* 
 Include file for E_i2ceeprom.c
 Description			:   Include file for i2c EEProm interface
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   3/4/2007 
 
 Summary for include files
 -------------------------
 Constant variable				X
 Function prototype				-
 Variable (Flash memory)		X
 Memory usage					XXXX bytes
 
 
 Update History
 
 Date			By 					Comments
 ----			--					---------
 2003.04.23   	Pascal Stang      This code is distributed under the GNU Public License
				Copyright (C) 2003				 

 2007.04.03		Wichai			  Updated to use with E-block project
 
*/

//#ifndef I2CEEPROM_H
//#define I2CEEPROM_H

#include "cpu.h"

// functions

//! Initialize I2C EEPROM interface
void i2ceeprom_init(void);

//! In the I2C EEPROM at [i2cAddr], read a byte from memory location [memAddr]
uint8_t i2ceeprom_read_byte(uint8_t i2cAddr, uint32_t memAddr);

//! In the I2C EEPROM at [i2cAddr], write a byte [data] to the memory location [memAddr]
void i2ceeprom_write_byte(uint8_t i2cAddr, uint32_t memAddr, uint8_t data);

//#endif

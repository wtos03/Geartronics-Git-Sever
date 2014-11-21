/*! \file sta013.c \brief STMicroelectronics STA013 MP3 player driver. */
//*****************************************************************************
//
// File Name	: 'sta013.c'
// Title		: STMicroelectronics STA013 MP3 player driver
// Author		: Pascal Stang
// Created		: 10/22/2000
// Revised		: 7/11/2003
// Version		: 0.3
// Target MCU	: ATmega103 (should work for Atmel AVR Series)
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "global.h"
#include "i2csw.h"
#include "timer.h"
//#include "procyon.h"

#include "sta013.h"

// bitrate and sampling frequency mappings
//static int __attribute__ ((progmem)) MP3_Bitrates[] =
//  { 0,  8, 16, 24, 32, 40, 48, 56,  64,  80,  96, 112, 128, 144, 160, 0,
//     0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0};
static unsigned char __attribute__ ((progmem)) MP3_Bitrates[] =
	{ 0,  4,  8, 12, 16, 20, 24, 28, 32, 40, 48, 56,  64,  72,  80, 0,
	  0, 16, 20, 24, 28, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 0};

static unsigned char __attribute__ ((progmem)) MP3_SamplingFrequencies[] =
   { 11, 12,  8,  0,      // MPEG 2.5 rates
      0,  0,  0,  0,      // reserved rates
     22, 24, 16,  0,      // MPEG 2   rates
     44, 48, 32,  0       // MPEG 1   rates
   };

// STA013 firmware update and configuration data
static unsigned char __attribute__ ((progmem)) STA013_UpdateData[] = 
   {
0x3a, 0x01, 0x2a, 0x04, 0x28, 0x00, 0x29, 0x00, 0x20, 0x00, 
0x21, 0x00, 0x22, 0x00, 0x23, 0x00, 0x24, 0x00, 0x25, 0x00, 
0x26, 0x00, 0x27, 0x00, 0x28, 0x01, 0x28, 0x02, 0x21, 0x8f, 
0x28, 0x03, 0x21, 0x00, 0x28, 0x04, 0x28, 0x05, 0x28, 0x06, 
0x28, 0x07, 0x28, 0x08, 0x28, 0x09, 0x28, 0x0a, 0x28, 0x0b, 
0x28, 0x0c, 0x20, 0x80, 0x21, 0x90, 0x28, 0x0d, 0x20, 0x00, 
0x21, 0x00, 0x28, 0x0e, 0x20, 0x81, 0x21, 0x91, 0x28, 0x0f, 
0x20, 0x00, 0x21, 0x92, 0x28, 0x10, 0x21, 0x00, 0x28, 0x11, 
0x21, 0x93, 0x28, 0x12, 0x21, 0x00, 0x28, 0x13, 0x28, 0x14, 
0x28, 0x15, 0x20, 0x82, 0x28, 0x16, 0x20, 0x00, 0x28, 0x17, 
0x28, 0x18, 0x28, 0x19, 0x21, 0x94, 0x28, 0x1a, 0x21, 0x95, 
0x28, 0x1b, 0x21, 0x96, 0x28, 0x1c, 0x21, 0x00, 0x28, 0x1d, 
0x20, 0x83, 0x28, 0x1e, 0x20, 0x00, 0x28, 0x1f, 0x21, 0x97, 
0x28, 0x20, 0x21, 0x00, 0x28, 0x21, 0x28, 0x22, 0x28, 0x23, 
0x28, 0x24, 0x28, 0x25, 0x28, 0x26, 0x28, 0x27, 0x28, 0x28, 
0x28, 0x29, 0x28, 0x2a, 0x20, 0x84, 0x28, 0x2b, 0x20, 0x00, 
0x28, 0x2c, 0x28, 0x2d, 0x28, 0x2e, 0x28, 0x2f, 0x28, 0x30, 
0x28, 0x31, 0x28, 0x32, 0x20, 0x85, 0x28, 0x33, 0x20, 0x00, 
0x28, 0x34, 0x28, 0x35, 0x28, 0x36, 0x28, 0x37, 0x21, 0x98, 
0x28, 0x38, 0x21, 0x00, 0x28, 0x39, 0x28, 0x3a, 0x28, 0x3b, 
0x28, 0x3c, 0x28, 0x3d, 0x28, 0x3e, 0x28, 0x3f, 0x28, 0x40, 
0x28, 0x41, 0x28, 0x42, 0x28, 0x43, 0x28, 0x44, 0x28, 0x45, 
0x28, 0x46, 0x28, 0x47, 0x28, 0x48, 0x28, 0x49, 0x28, 0x4a, 
0x28, 0x4b, 0x28, 0x4c, 0x28, 0x4d, 0x28, 0x4e, 0x28, 0x4f, 
0x28, 0x50, 0x28, 0x51, 0x28, 0x52, 0x28, 0x53, 0x28, 0x54, 
0x28, 0x55, 0x28, 0x56, 0x28, 0x57, 0x28, 0x58, 0x28, 0x59, 
0x28, 0x5a, 0x28, 0x5b, 0x28, 0x5c, 0x28, 0x5d, 0x28, 0x5e, 
0x28, 0x5f, 0x28, 0x60, 0x28, 0x61, 0x28, 0x62, 0x21, 0x99, 
0x28, 0x63, 0x21, 0x00, 0x28, 0x64, 0x28, 0x65, 0x28, 0x66, 
0x28, 0x67, 0x28, 0x68, 0x28, 0x69, 0x28, 0x6a, 0x28, 0x6b, 
0x28, 0x6c, 0x28, 0x6d, 0x28, 0x6e, 0x28, 0x6f, 0x28, 0x70, 
0x28, 0x71, 0x28, 0x72, 0x28, 0x73, 0x28, 0x74, 0x28, 0x75, 
0x28, 0x76, 0x28, 0x77, 0x28, 0x78, 0x28, 0x79, 0x28, 0x7a, 
0x28, 0x7b, 0x28, 0x7c, 0x28, 0x7d, 0x28, 0x7e, 0x28, 0x7f, 
0x28, 0x80, 0x28, 0x81, 0x28, 0x82, 0x28, 0x83, 0x28, 0x84, 
0x28, 0x85, 0x28, 0x86, 0x28, 0x87, 0x28, 0x88, 0x28, 0x89, 
0x28, 0x8a, 0x28, 0x8b, 0x28, 0x8c, 0x28, 0x8d, 0x28, 0x8e, 
0x28, 0x8f, 0x28, 0x90, 0x28, 0x91, 0x20, 0x86, 0x28, 0x92, 
0x20, 0x87, 0x28, 0x93, 0x20, 0x00, 0x28, 0x94, 0x28, 0x95, 
0x28, 0x96, 0x28, 0x97, 0x28, 0x98, 0x28, 0x99, 0x28, 0x9a, 
0x28, 0x9b, 0x28, 0x9c, 0x28, 0x9d, 0x28, 0x9e, 0x28, 0x9f, 
0x21, 0x9a, 0x28, 0xa0, 0x21, 0x00, 0x28, 0xa1, 0x28, 0xa2, 
0x28, 0xa3, 0x28, 0xa4, 0x28, 0xa5, 0x28, 0xa6, 0x28, 0xa7, 
0x28, 0xa8, 0x28, 0xa9, 0x28, 0xaa, 0x28, 0xab, 0x28, 0xac, 
0x28, 0xad, 0x28, 0xae, 0x28, 0xaf, 0x28, 0xb0, 0x28, 0xb1, 
0x28, 0xb2, 0x28, 0xb3, 0x28, 0xb4, 0x28, 0xb5, 0x28, 0xb6, 
0x28, 0xb7, 0x28, 0xb8, 0x28, 0xb9, 0x20, 0x88, 0x28, 0xba, 
0x20, 0x00, 0x28, 0xbb, 0x20, 0x89, 0x28, 0xbc, 0x20, 0x00, 
0x28, 0xbd, 0x28, 0xbe, 0x28, 0xbf, 0x28, 0xc0, 0x28, 0xc1, 
0x28, 0xc2, 0x28, 0xc3, 0x21, 0x9b, 0x28, 0xc4, 0x21, 0x00, 
0x28, 0xc5, 0x28, 0xc6, 0x28, 0xc7, 0x28, 0xc8, 0x28, 0xc9, 
0x28, 0xca, 0x28, 0xcb, 0x28, 0xcc, 0x28, 0xcd, 0x28, 0xce, 
0x28, 0xcf, 0x28, 0xd0, 0x28, 0xd1, 0x28, 0xd2, 0x28, 0xd3, 
0x28, 0xd4, 0x28, 0xd5, 0x28, 0xd6, 0x28, 0xd7, 0x28, 0xd8, 
0x28, 0xd9, 0x28, 0xda, 0x28, 0xdb, 0x28, 0xdc, 0x28, 0xdd, 
0x28, 0xde, 0x28, 0xdf, 0x28, 0xe0, 0x28, 0xe1, 0x20, 0x8a, 
0x28, 0xe2, 0x20, 0x8b, 0x28, 0xe3, 0x20, 0x00, 0x28, 0xe4, 
0x28, 0xe5, 0x28, 0xe6, 0x28, 0xe7, 0x28, 0xe8, 0x28, 0xe9, 
0x28, 0xea, 0x28, 0xeb, 0x28, 0xec, 0x28, 0xed, 0x28, 0xee, 
0x28, 0xef, 0x28, 0xf0, 0x28, 0xf1, 0x28, 0xf2, 0x28, 0xf3, 
0x28, 0xf4, 0x28, 0xf5, 0x28, 0xf6, 0x28, 0xf7, 0x28, 0xf8, 
0x20, 0x8c, 0x28, 0xf9, 0x20, 0x00, 0x21, 0x9c, 0x28, 0xfa, 
0x21, 0x9d, 0x28, 0xfb, 0x20, 0x8d, 0x21, 0x9e, 0x28, 0xfc, 
0x20, 0x8e, 0x21, 0x9f, 0x28, 0xfd, 0x20, 0x00, 0x21, 0x00, 
0x28, 0xfe, 0x28, 0xff, 0x2a, 0x01, 0x28, 0x00, 0x22, 0x01, 
0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0xa4, 0x27, 0x07, 
0x28, 0x01, 0x21, 0xc7, 0x22, 0x00, 0x23, 0x80, 0x26, 0xc4, 
0x27, 0x0c, 0x28, 0x02, 0x20, 0x09, 0x21, 0x1c, 0x22, 0x04, 
0x26, 0xaa, 0x27, 0x0a, 0x28, 0x03, 0x20, 0x00, 0x21, 0x00, 
0x22, 0x00, 0x23, 0xa6, 0x26, 0xa0, 0x27, 0x07, 0x28, 0x04, 
0x28, 0x05, 0x20, 0x05, 0x23, 0x84, 0x26, 0xb4, 0x27, 0x09, 
0x28, 0x06, 0x20, 0x00, 0x22, 0x03, 0x23, 0x00, 0x24, 0x81, 
0x25, 0xc0, 0x26, 0xab, 0x27, 0x0a, 0x28, 0x07, 0x21, 0xae, 
0x22, 0x00, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0xa4, 
0x27, 0x00, 0x28, 0x08, 0x21, 0x48, 0x22, 0x01, 0x23, 0x80, 
0x26, 0xc4, 0x27, 0x0c, 0x28, 0x09, 0x20, 0x09, 0x21, 0x04, 
0x22, 0x04, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x0a, 0x20, 0x00, 
0x21, 0x00, 0x22, 0x00, 0x23, 0xa8, 0x26, 0xa4, 0x27, 0x07, 
0x28, 0x0b, 0x28, 0x0c, 0x21, 0x40, 0x22, 0x20, 0x23, 0x80, 
0x26, 0xc4, 0x27, 0x0c, 0x28, 0x0d, 0x22, 0x24, 0x26, 0xc6, 
0x28, 0x0e, 0x21, 0x9e, 0x22, 0x00, 0x26, 0xc8, 0x28, 0x0f, 
0x20, 0x09, 0x21, 0x02, 0x22, 0x14, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0x10, 0x20, 0x05, 0x21, 0x00, 0x22, 0x00, 0x23, 0x84, 
0x26, 0xb4, 0x27, 0x09, 0x28, 0x11, 0x20, 0x00, 0x21, 0x01, 
0x23, 0x00, 0x24, 0x01, 0x25, 0xc0, 0x26, 0xab, 0x27, 0x0a, 
0x28, 0x12, 0x21, 0xc3, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x20, 0x27, 0x00, 0x28, 0x13, 0x20, 0x03, 0x21, 0xc2, 
0x23, 0x83, 0x26, 0x26, 0x27, 0x0d, 0x28, 0x14, 0x21, 0xb3, 
0x22, 0x08, 0x28, 0x15, 0x20, 0x00, 0x21, 0xc6, 0x22, 0x00, 
0x23, 0x95, 0x26, 0x00, 0x27, 0x08, 0x28, 0x16, 0x21, 0x00, 
0x23, 0x00, 0x24, 0x96, 0x25, 0x03, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0x17, 0x20, 0x42, 0x21, 0xb6, 0x23, 0x80, 0x24, 0x89, 
0x25, 0x07, 0x28, 0x18, 0x20, 0x00, 0x21, 0x00, 0x23, 0x94, 
0x26, 0x0e, 0x28, 0x19, 0x20, 0x0f, 0x23, 0x84, 0x26, 0xb4, 
0x27, 0x09, 0x28, 0x1a, 0x20, 0x00, 0x23, 0x93, 0x26, 0xa8, 
0x27, 0x03, 0x28, 0x1b, 0x26, 0x28, 0x27, 0x00, 0x28, 0x1c, 
0x21, 0x01, 0x23, 0x00, 0x24, 0x01, 0x25, 0xc0, 0x26, 0xab, 
0x27, 0x0a, 0x28, 0x1d, 0x21, 0xc5, 0x23, 0x95, 0x24, 0x89, 
0x25, 0x07, 0x26, 0x20, 0x27, 0x00, 0x28, 0x1e, 0x21, 0x00, 
0x23, 0x94, 0x26, 0x00, 0x27, 0x0a, 0x28, 0x1f, 0x26, 0x0e, 
0x28, 0x20, 0x20, 0x03, 0x21, 0x79, 0x22, 0x01, 0x23, 0x83, 
0x26, 0x26, 0x27, 0x0d, 0x28, 0x21, 0x21, 0x32, 0x28, 0x22, 
0x20, 0x00, 0x21, 0x04, 0x22, 0x00, 0x23, 0x80, 0x24, 0x00, 
0x25, 0xfc, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x23, 0x21, 0x00, 
0x23, 0x00, 0x25, 0x00, 0x28, 0x24, 0x21, 0xa3, 0x22, 0x0d, 
0x23, 0x80, 0x24, 0x9e, 0x25, 0x3b, 0x28, 0x25, 0x20, 0x42, 
0x21, 0x57, 0x22, 0x01, 0x24, 0x89, 0x25, 0x07, 0x28, 0x26, 
0x20, 0x00, 0x21, 0x43, 0x22, 0x0d, 0x24, 0x00, 0x25, 0x38, 
0x28, 0x27, 0x21, 0x08, 0x22, 0x98, 0x23, 0x95, 0x24, 0x89, 
0x25, 0x07, 0x26, 0x24, 0x27, 0x00, 0x28, 0x28, 0x20, 0x42, 
0x21, 0x93, 0x22, 0x01, 0x23, 0x80, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0x29, 0x20, 0x03, 0x21, 0x7e, 0x22, 0x04, 0x23, 0x83, 
0x26, 0x26, 0x27, 0x0d, 0x28, 0x2a, 0x20, 0x00, 0x21, 0xb0, 
0x22, 0x00, 0x23, 0x95, 0x26, 0xa0, 0x27, 0x07, 0x28, 0x2b, 
0x21, 0x00, 0x23, 0x00, 0x24, 0x0c, 0x25, 0x04, 0x26, 0xaa, 
0x27, 0x0a, 0x28, 0x2c, 0x21, 0x02, 0x23, 0x80, 0x24, 0x86, 
0x25, 0xc3, 0x26, 0xab, 0x28, 0x2d, 0x20, 0x42, 0x21, 0x97, 
0x22, 0x01, 0x24, 0x89, 0x25, 0x07, 0x26, 0xaa, 0x28, 0x2e, 
0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 0x24, 0x1f, 0x25, 0x04, 
0x28, 0x2f, 0x21, 0xb2, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x20, 0x27, 0x04, 0x28, 0x30, 0x20, 0x42, 0x21, 0x8b, 
0x22, 0x04, 0x23, 0x80, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x31, 
0x20, 0x00, 0x21, 0xb1, 0x22, 0x00, 0x23, 0x95, 0x26, 0xa0, 
0x27, 0x07, 0x28, 0x32, 0x20, 0x03, 0x21, 0x75, 0x22, 0x04, 
0x23, 0x83, 0x26, 0x26, 0x27, 0x0d, 0x28, 0x33, 0x20, 0x05, 
0x21, 0x00, 0x22, 0x00, 0x23, 0x84, 0x26, 0xb4, 0x27, 0x09, 
0x28, 0x34, 0x20, 0x00, 0x21, 0x60, 0x23, 0x00, 0x24, 0x01, 
0x25, 0xc0, 0x26, 0xab, 0x27, 0x0a, 0x28, 0x35, 0x21, 0x08, 
0x22, 0x98, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0x24, 
0x27, 0x00, 0x28, 0x36, 0x21, 0xbc, 0x22, 0x00, 0x28, 0x37, 
0x21, 0xa3, 0x22, 0x0d, 0x23, 0x00, 0x24, 0x01, 0x25, 0xc0, 
0x26, 0xab, 0x27, 0x0a, 0x28, 0x38, 0x21, 0x60, 0x22, 0x00, 
0x23, 0x80, 0x24, 0x00, 0x25, 0x38, 0x26, 0xaa, 0x28, 0x39, 
0x21, 0x06, 0x22, 0x98, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x24, 0x27, 0x00, 0x28, 0x3a, 0x21, 0xbe, 0x22, 0x00, 
0x28, 0x3b, 0x20, 0x05, 0x21, 0x00, 0x23, 0x84, 0x26, 0xb4, 
0x27, 0x09, 0x28, 0x3c, 0x20, 0x00, 0x21, 0x60, 0x23, 0x00, 
0x24, 0x01, 0x25, 0xc0, 0x26, 0xab, 0x27, 0x0a, 0x28, 0x3d, 
0x21, 0x42, 0x22, 0x01, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x24, 0x27, 0x00, 0x28, 0x3e, 0x21, 0x0c, 0x22, 0x98, 
0x26, 0xa4, 0x27, 0x07, 0x28, 0x3f, 0x21, 0x08, 0x28, 0x40, 
0x21, 0x60, 0x22, 0x00, 0x23, 0x00, 0x24, 0x01, 0x25, 0xc0, 
0x26, 0xab, 0x27, 0x0a, 0x28, 0x41, 0x21, 0x0c, 0x22, 0x98, 
0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0x24, 0x27, 0x00, 
0x28, 0x42, 0x20, 0x05, 0x21, 0x00, 0x22, 0x00, 0x23, 0x84, 
0x26, 0xb4, 0x27, 0x09, 0x28, 0x43, 0x20, 0x00, 0x23, 0x80, 
0x24, 0x00, 0x25, 0x10, 0x26, 0xab, 0x27, 0x0a, 0x28, 0x44, 
0x21, 0x22, 0x22, 0x01, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x24, 0x27, 0x00, 0x28, 0x45, 0x21, 0x00, 0x22, 0x00, 
0x23, 0x00, 0x24, 0x04, 0x25, 0x12, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0x46, 0x20, 0x03, 0x21, 0xd4, 0x22, 0x05, 0x23, 0x83, 
0x24, 0x89, 0x25, 0x07, 0x26, 0x26, 0x27, 0x0d, 0x28, 0x47, 
0x20, 0x00, 0x21, 0x60, 0x22, 0x00, 0x23, 0x00, 0x24, 0x01, 
0x25, 0xc0, 0x26, 0xab, 0x27, 0x0a, 0x28, 0x48, 0x21, 0xb3, 
0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0x00, 0x27, 0x08, 
0x28, 0x49, 0x21, 0x00, 0x23, 0x00, 0x24, 0x09, 0x25, 0x00, 
0x26, 0xaa, 0x27, 0x0a, 0x28, 0x4a, 0x21, 0x02, 0x23, 0x80, 
0x24, 0x86, 0x25, 0x3b, 0x28, 0x4b, 0x20, 0x42, 0x21, 0xdd, 
0x22, 0x05, 0x24, 0x89, 0x25, 0x07, 0x28, 0x4c, 0x20, 0x00, 
0x21, 0x00, 0x22, 0x00, 0x24, 0x1f, 0x25, 0x00, 0x28, 0x4d, 
0x21, 0xb3, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0x20, 
0x27, 0x00, 0x28, 0x4e, 0x21, 0x00, 0x23, 0x00, 0x24, 0x97, 
0x25, 0x03, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x4f, 0x20, 0x42, 
0x21, 0xe2, 0x22, 0x05, 0x23, 0x80, 0x24, 0x89, 0x25, 0x07, 
0x28, 0x50, 0x20, 0x00, 0x21, 0x10, 0x22, 0x00, 0x23, 0x00, 
0x24, 0x80, 0x25, 0xcc, 0x26, 0xab, 0x28, 0x51, 0x21, 0x00, 
0x22, 0x40, 0x23, 0xb4, 0x24, 0x89, 0x25, 0x07, 0x26, 0xaa, 
0x27, 0x04, 0x28, 0x52, 0x22, 0x00, 0x23, 0xb0, 0x26, 0x92, 
0x27, 0x09, 0x28, 0x53, 0x21, 0x14, 0x22, 0x01, 0x23, 0x95, 
0x26, 0x00, 0x27, 0x08, 0x28, 0x54, 0x21, 0x48, 0x23, 0xa4, 
0x26, 0x2a, 0x27, 0x00, 0x28, 0x55, 0x21, 0x00, 0x22, 0x00, 
0x23, 0x00, 0x24, 0x09, 0x25, 0x00, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0x56, 0x21, 0x08, 0x23, 0x80, 0x24, 0x86, 0x25, 0x3b, 
0x28, 0x57, 0x20, 0x42, 0x21, 0x11, 0x22, 0x07, 0x24, 0x89, 
0x25, 0x07, 0x28, 0x58, 0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 
0x24, 0x1f, 0x25, 0x00, 0x28, 0x59, 0x21, 0x14, 0x22, 0x01, 
0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0x20, 0x27, 0x00, 
0x28, 0x5a, 0x21, 0x16, 0x26, 0x02, 0x27, 0x09, 0x28, 0x5b, 
0x21, 0x00, 0x22, 0x00, 0x23, 0x00, 0x24, 0x80, 0x25, 0x48, 
0x26, 0xaa, 0x27, 0x0a, 0x28, 0x5c, 0x23, 0xa0, 0x24, 0x89, 
0x25, 0x07, 0x26, 0x14, 0x27, 0x09, 0x28, 0x5d, 0x24, 0x80, 
0x25, 0x50, 0x26, 0xa4, 0x27, 0x04, 0x28, 0x5e, 0x21, 0x16, 
0x22, 0x01, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x27, 0x00, 
0x28, 0x5f, 0x21, 0x04, 0x22, 0x00, 0x23, 0x00, 0x24, 0x9c, 
0x25, 0x78, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x60, 0x21, 0x43, 
0x22, 0x0d, 0x24, 0x01, 0x25, 0xc4, 0x26, 0xab, 0x28, 0x61, 
0x21, 0x03, 0x22, 0x00, 0x24, 0x15, 0x28, 0x62, 0x21, 0x00, 
0x24, 0x00, 0x25, 0x00, 0x26, 0xaa, 0x28, 0x63, 0x21, 0x5e, 
0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0x10, 0x27, 0x09, 
0x28, 0x64, 0x21, 0x03, 0x23, 0x00, 0x24, 0x15, 0x25, 0xc4, 
0x26, 0xab, 0x27, 0x0a, 0x28, 0x65, 0x21, 0x00, 0x23, 0x80, 
0x24, 0x00, 0x25, 0x06, 0x26, 0xaa, 0x28, 0x66, 0x24, 0x8f, 
0x25, 0xe3, 0x26, 0xab, 0x28, 0x67, 0x20, 0x42, 0x21, 0xcd, 
0x22, 0x07, 0x24, 0x89, 0x25, 0x07, 0x26, 0xaa, 0x28, 0x68, 
0x20, 0x00, 0x21, 0x60, 0x22, 0x00, 0x23, 0x00, 0x24, 0x01, 
0x25, 0xc0, 0x26, 0xab, 0x28, 0x69, 0x21, 0x00, 0x23, 0x80, 
0x24, 0x8f, 0x25, 0x03, 0x26, 0xaa, 0x28, 0x6a, 0x20, 0x42, 
0x21, 0xdd, 0x22, 0x07, 0x24, 0x89, 0x25, 0x07, 0x28, 0x6b, 
0x20, 0x00, 0x21, 0x43, 0x22, 0x0d, 0x23, 0x00, 0x24, 0x01, 
0x25, 0xc0, 0x26, 0xab, 0x28, 0x6c, 0x21, 0x00, 0x22, 0x00, 
0x24, 0x00, 0x25, 0x04, 0x26, 0xaa, 0x28, 0x6d, 0x20, 0x03, 
0x21, 0x7f, 0x22, 0x01, 0x23, 0x83, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x26, 0x27, 0x0d, 0x28, 0x6e, 0x21, 0x62, 0x28, 0x6f, 
0x20, 0x05, 0x21, 0x00, 0x22, 0x00, 0x23, 0x84, 0x26, 0xb4, 
0x27, 0x09, 0x28, 0x70, 0x20, 0x00, 0x21, 0x06, 0x23, 0x95, 
0x26, 0x00, 0x27, 0x08, 0x28, 0x71, 0x21, 0x01, 0x23, 0x00, 
0x24, 0x97, 0x25, 0x3b, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x72, 
0x20, 0x42, 0x21, 0x0e, 0x22, 0x08, 0x23, 0x80, 0x24, 0x89, 
0x25, 0x07, 0x28, 0x73, 0x20, 0x03, 0x21, 0x3c, 0x22, 0x0b, 
0x23, 0x83, 0x26, 0x26, 0x27, 0x0d, 0x28, 0x74, 0x20, 0x00, 
0x21, 0x03, 0x22, 0x00, 0x23, 0x95, 0x26, 0x02, 0x27, 0x08, 
0x28, 0x75, 0x21, 0x27, 0x23, 0x00, 0x24, 0x81, 0x25, 0xc4, 
0x26, 0xab, 0x27, 0x0a, 0x28, 0x76, 0x21, 0x00, 0x23, 0x80, 
0x24, 0x09, 0x25, 0x48, 0x26, 0xaa, 0x28, 0x77, 0x21, 0x58, 
0x22, 0x34, 0x23, 0xa3, 0x24, 0x89, 0x25, 0x07, 0x26, 0x2a, 
0x27, 0x00, 0x28, 0x78, 0x21, 0x00, 0x22, 0x00, 0x23, 0xa0, 
0x26, 0x00, 0x27, 0x09, 0x28, 0x79, 0x21, 0x07, 0x23, 0x80, 
0x24, 0x03, 0x25, 0x39, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x7a, 
0x21, 0x04, 0x23, 0x00, 0x24, 0x1c, 0x25, 0x38, 0x28, 0x7b, 
0x21, 0x07, 0x23, 0x80, 0x24, 0x83, 0x25, 0x39, 0x28, 0x7c, 
0x21, 0x03, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0x02, 
0x27, 0x08, 0x28, 0x7d, 0x21, 0x27, 0x23, 0x00, 0x24, 0x81, 
0x25, 0xc4, 0x26, 0xab, 0x27, 0x0a, 0x28, 0x7e, 0x21, 0x00, 
0x23, 0x80, 0x24, 0x09, 0x25, 0x48, 0x26, 0xaa, 0x28, 0x7f, 
0x21, 0x58, 0x22, 0x34, 0x23, 0xa3, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x2a, 0x27, 0x00, 0x28, 0x80, 0x21, 0x06, 0x22, 0x00, 
0x23, 0x00, 0x24, 0x81, 0x25, 0xc0, 0x26, 0x93, 0x27, 0x01, 
0x28, 0x81, 0x21, 0x00, 0x23, 0x80, 0x24, 0x89, 0x25, 0x48, 
0x26, 0xaa, 0x27, 0x0a, 0x28, 0x82, 0x20, 0x02, 0x21, 0x66, 
0x22, 0x08, 0x23, 0x00, 0x24, 0x00, 0x25, 0x66, 0x28, 0x83, 
0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 0x23, 0x80, 0x24, 0x1f, 
0x25, 0x02, 0x28, 0x84, 0x21, 0x06, 0x24, 0x89, 0x25, 0x07, 
0x26, 0xdc, 0x27, 0x0c, 0x28, 0x85, 0x21, 0x00, 0x23, 0xaa, 
0x26, 0xaa, 0x27, 0x0a, 0x28, 0x86, 0x23, 0xb6, 0x26, 0x00, 
0x27, 0x08, 0x28, 0x87, 0x23, 0x80, 0x24, 0x86, 0x25, 0x3b, 
0x26, 0xaa, 0x27, 0x0a, 0x28, 0x88, 0x21, 0x05, 0x24, 0x89, 
0x25, 0x07, 0x26, 0xdc, 0x27, 0x0c, 0x28, 0x89, 0x21, 0x00, 
0x23, 0xaa, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x8a, 0x23, 0xb6, 
0x26, 0x00, 0x27, 0x08, 0x28, 0x8b, 0x23, 0x80, 0x24, 0x86, 
0x25, 0x3b, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0x8c, 0x20, 0x42, 
0x21, 0x7b, 0x22, 0x08, 0x24, 0x89, 0x25, 0x07, 0x28, 0x8d, 
0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 0x23, 0x00, 0x24, 0x01, 
0x25, 0x82, 0x28, 0x8e, 0x21, 0x05, 0x24, 0x81, 0x25, 0xc2, 
0x26, 0xab, 0x28, 0x8f, 0x20, 0x03, 0x21, 0x93, 0x22, 0x08, 
0x23, 0x83, 0x24, 0x89, 0x25, 0x07, 0x26, 0x26, 0x27, 0x0d, 
0x28, 0x90, 0x20, 0x00, 0x21, 0x05, 0x22, 0x00, 0x23, 0x00, 
0x24, 0x81, 0x25, 0xc0, 0x26, 0x13, 0x27, 0x01, 0x28, 0x91, 
0x21, 0x00, 0x23, 0x80, 0x24, 0x89, 0x25, 0x48, 0x26, 0xaa, 
0x27, 0x0a, 0x28, 0x92, 0x20, 0x02, 0x21, 0x7e, 0x22, 0x08, 
0x23, 0x00, 0x24, 0x00, 0x25, 0x66, 0x28, 0x93, 0x20, 0x00, 
0x21, 0x00, 0x22, 0x00, 0x23, 0x80, 0x24, 0x1f, 0x25, 0x02, 
0x28, 0x94, 0x21, 0x05, 0x24, 0x89, 0x25, 0x07, 0x26, 0xdc, 
0x27, 0x0c, 0x28, 0x95, 0x21, 0x00, 0x23, 0xaa, 0x26, 0xaa, 
0x27, 0x0a, 0x28, 0x96, 0x23, 0xb6, 0x26, 0x00, 0x27, 0x08, 
0x28, 0x97, 0x23, 0x80, 0x24, 0x86, 0x25, 0x3b, 0x26, 0xaa, 
0x27, 0x0a, 0x28, 0x98, 0x21, 0x05, 0x24, 0x89, 0x25, 0x07, 
0x26, 0xdc, 0x27, 0x0c, 0x28, 0x99, 0x21, 0x00, 0x23, 0xaa, 
0x26, 0xaa, 0x27, 0x0a, 0x28, 0x9a, 0x23, 0x80, 0x24, 0x1f, 
0x25, 0x02, 0x28, 0x9b, 0x21, 0x01, 0x23, 0x00, 0x24, 0x81, 
0x25, 0xc2, 0x26, 0xab, 0x28, 0x9c, 0x20, 0x03, 0x21, 0x93, 
0x22, 0x08, 0x23, 0x83, 0x24, 0x89, 0x25, 0x07, 0x26, 0x26, 
0x27, 0x0d, 0x28, 0x9d, 0x20, 0x00, 0x21, 0x04, 0x22, 0x00, 
0x23, 0x95, 0x26, 0x12, 0x27, 0x08, 0x28, 0x9e, 0x21, 0x60, 
0x23, 0xc4, 0x26, 0xaa, 0x27, 0x04, 0x28, 0x9f, 0x21, 0x00, 
0x23, 0xc0, 0x26, 0x00, 0x27, 0x09, 0x28, 0xa0, 0x20, 0x42, 
0x21, 0xcb, 0x22, 0x08, 0x23, 0x80, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0xa1, 0x20, 0x70, 0x21, 0x54, 0x22, 0xb8, 0x23, 0x95, 
0x26, 0x12, 0x27, 0x08, 0x28, 0xa2, 0x20, 0x02, 0x21, 0xcc, 
0x22, 0x08, 0x23, 0x80, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0xa3, 
0x20, 0x03, 0x21, 0xca, 0x22, 0x0f, 0x23, 0x83, 0x26, 0x26, 
0x27, 0x0d, 0x28, 0xa4, 0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 
0x23, 0x00, 0x24, 0x8c, 0x25, 0x0c, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0xa5, 0x21, 0x04, 0x24, 0x82, 0x25, 0x78, 0x28, 0xa6, 
0x21, 0x00, 0x23, 0x80, 0x24, 0x89, 0x25, 0x48, 0x26, 0x12, 
0x27, 0x04, 0x28, 0xa7, 0x20, 0x03, 0x21, 0xde, 0x22, 0x0f, 
0x23, 0x83, 0x25, 0x07, 0x26, 0x26, 0x27, 0x0d, 0x28, 0xa8, 
0x20, 0x00, 0x21, 0x01, 0x22, 0x00, 0x23, 0x00, 0x24, 0x9c, 
0x25, 0x78, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0xa9, 0x21, 0x00, 
0x23, 0x80, 0x24, 0x81, 0x25, 0x40, 0x28, 0xaa, 0x20, 0x03, 
0x21, 0xcf, 0x22, 0x0f, 0x23, 0x83, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x26, 0x27, 0x0d, 0x28, 0xab, 0x20, 0x70, 0x21, 0x08, 
0x22, 0xb8, 0x23, 0x95, 0x26, 0xa0, 0x27, 0x04, 0x28, 0xac, 
0x20, 0x03, 0x21, 0xd4, 0x22, 0x0f, 0x23, 0x83, 0x26, 0x26, 
0x27, 0x0d, 0x28, 0xad, 0x21, 0x27, 0x22, 0x0b, 0x28, 0xae, 
0x20, 0x70, 0x21, 0x63, 0x22, 0xb8, 0x23, 0x95, 0x26, 0x12, 
0x27, 0x08, 0x28, 0xaf, 0x20, 0x00, 0x21, 0x01, 0x22, 0x00, 
0x23, 0x80, 0x24, 0x86, 0x25, 0xcb, 0x26, 0xab, 0x27, 0x0a, 
0x28, 0xb0, 0x20, 0x42, 0x21, 0xdd, 0x22, 0x08, 0x24, 0x89, 
0x25, 0x07, 0x26, 0xaa, 0x28, 0xb1, 0x20, 0x00, 0x21, 0x18, 
0x22, 0x80, 0x23, 0x95, 0x26, 0xa4, 0x27, 0x00, 0x28, 0xb2, 
0x20, 0x03, 0x21, 0xd9, 0x22, 0x0f, 0x23, 0x83, 0x26, 0x26, 
0x27, 0x0d, 0x28, 0xb3, 0x20, 0x70, 0x21, 0x0a, 0x22, 0xb8, 
0x23, 0x95, 0x26, 0xa0, 0x27, 0x00, 0x28, 0xb4, 0x20, 0x02, 
0x21, 0xe0, 0x22, 0x08, 0x23, 0x80, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0xb5, 0x20, 0x70, 0x21, 0x0a, 0x22, 0xb8, 0x23, 0x95, 
0x26, 0xa0, 0x27, 0x00, 0x28, 0xb6, 0x21, 0x54, 0x26, 0x02, 
0x27, 0x08, 0x28, 0xb7, 0x20, 0x00, 0x21, 0x18, 0x22, 0x80, 
0x26, 0xa4, 0x27, 0x00, 0x28, 0xb8, 0x21, 0x00, 0x22, 0x00, 
0x23, 0x80, 0x24, 0x80, 0x25, 0x48, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0xb9, 0x20, 0x70, 0x21, 0x5e, 0x22, 0xb8, 0x23, 0x95, 
0x24, 0x89, 0x25, 0x07, 0x26, 0x12, 0x27, 0x08, 0x28, 0xba, 
0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 0x23, 0x80, 0x24, 0x09, 
0x25, 0x4b, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0xbb, 0x20, 0x70, 
0x21, 0x5f, 0x22, 0xb8, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 
0x26, 0x10, 0x27, 0x08, 0x28, 0xbc, 0x20, 0x00, 0x21, 0x00, 
0x22, 0x00, 0x23, 0x00, 0x24, 0x1c, 0x25, 0x83, 0x26, 0xab, 
0x27, 0x0a, 0x28, 0xbd, 0x20, 0x03, 0x21, 0x32, 0x22, 0x09, 
0x23, 0x83, 0x24, 0x89, 0x25, 0x07, 0x26, 0x26, 0x27, 0x0d, 
0x28, 0xbe, 0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 0x23, 0x00, 
0x24, 0x82, 0x25, 0x8c, 0x26, 0xab, 0x27, 0x0a, 0x28, 0xbf, 
0x20, 0x03, 0x21, 0x25, 0x22, 0x09, 0x23, 0x83, 0x24, 0x89, 
0x25, 0x07, 0x26, 0x26, 0x27, 0x0d, 0x28, 0xc0, 0x20, 0x42, 
0x21, 0xbc, 0x23, 0x80, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0xc1, 
0x20, 0x00, 0x21, 0x01, 0x22, 0x00, 0x23, 0x00, 0x24, 0x01, 
0x25, 0xc0, 0x26, 0xab, 0x28, 0xc2, 0x20, 0x70, 0x21, 0x0f, 
0x22, 0xb8, 0x23, 0x95, 0x24, 0x89, 0x25, 0x07, 0x26, 0x20, 
0x27, 0x00, 0x28, 0xc3, 0x20, 0x03, 0x21, 0xe3, 0x22, 0x0f, 
0x23, 0x83, 0x26, 0x26, 0x27, 0x0d, 0x28, 0xc4, 0x20, 0x05, 
0x21, 0x00, 0x22, 0x00, 0x23, 0x84, 0x26, 0xb4, 0x27, 0x09, 
0x28, 0xc5, 0x20, 0x00, 0x21, 0x03, 0x23, 0x95, 0x26, 0x14, 
0x27, 0x08, 0x28, 0xc6, 0x21, 0xa6, 0x23, 0xa4, 0x26, 0x2a, 
0x27, 0x05, 0x28, 0xc7, 0x21, 0x00, 0x23, 0xa0, 0x26, 0x44, 
0x27, 0x09, 0x28, 0xc8, 0x21, 0x18, 0x22, 0x6d, 0x23, 0x80, 
0x24, 0x9e, 0x25, 0x7b, 0x26, 0xaa, 0x27, 0x0a, 0x28, 0xc9, 
0x20, 0x42, 0x21, 0x13, 0x22, 0x0b, 0x24, 0x89, 0x25, 0x07, 
0x28, 0xca, 0x20, 0x00, 0x21, 0xae, 0x22, 0x00, 0x23, 0x95, 
0x26, 0x24, 0x27, 0x00, 0x28, 0xcb, 0x21, 0x00, 0x22, 0x03, 
0x23, 0x80, 0x24, 0x87, 0x25, 0x7b, 0x26, 0xaa, 0x27, 0x0a, 
0x28, 0xcc, 0x20, 0x42, 0x21, 0x16, 0x22, 0x0b, 0x24, 0x89, 
0x25, 0x07, 0x28, 0xcd, 0x20, 0x00, 0x21, 0xae, 0x22, 0x00, 
0x23, 0x95, 0x26, 0x24, 0x27, 0x00, 0x28, 0xce, 0x20, 0x05, 
0x21, 0x00, 0x23, 0x84, 0x26, 0xb4, 0x27, 0x09, 0x28, 0xcf, 
0x20, 0x03, 0x21, 0x0d, 0x22, 0x0b, 0x23, 0x83, 0x26, 0x26, 
0x27, 0x0d, 0x28, 0xd0, 0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 
0x23, 0x00, 0x24, 0x80, 0x26, 0x02, 0x27, 0x00, 0x28, 0xd1, 
0x23, 0x80, 0x24, 0x09, 0x25, 0x48, 0x26, 0x1e, 0x28, 0xd2, 
0x21, 0x58, 0x22, 0x34, 0x23, 0x33, 0x24, 0x80, 0x25, 0x07, 
0x26, 0x2a, 0x28, 0xd3, 0x21, 0x00, 0x22, 0x00, 0x23, 0x00, 
0x26, 0x80, 0x27, 0x10, 0x28, 0xd4, 0x21, 0x10, 0x23, 0x63, 
0x26, 0x2a, 0x27, 0x00, 0x28, 0xd5, 0x21, 0x00, 0x23, 0x60, 
0x26, 0xa8, 0x27, 0x12, 0x28, 0xd6, 0x21, 0xaf, 0x22, 0x4c, 
0x23, 0x00, 0x26, 0xc8, 0x27, 0x0c, 0x28, 0xd7, 0x21, 0x00, 
0x22, 0x00, 0x26, 0x80, 0x27, 0x05, 0x28, 0xd8, 0x23, 0x80, 
0x24, 0x86, 0x25, 0x3b, 0x26, 0x1e, 0x27, 0x00, 0x28, 0xd9, 
0x20, 0x42, 0x21, 0x1c, 0x22, 0x0e, 0x23, 0x00, 0x24, 0x80, 
0x25, 0x07, 0x28, 0xda, 0x20, 0x00, 0x21, 0x35, 0x22, 0x29, 
0x26, 0xc4, 0x27, 0x0c, 0x28, 0xdb, 0x20, 0x02, 0x21, 0x1d, 
0x22, 0x0e, 0x26, 0x1e, 0x27, 0x00, 0x28, 0xdc, 0x20, 0x00, 
0x21, 0x7f, 0x22, 0x34, 0x26, 0xc4, 0x27, 0x0c, 0x28, 0xdd, 
0x21, 0x00, 0x22, 0x00, 0x23, 0x90, 0x24, 0x9f, 0x25, 0x04, 
0x26, 0x10, 0x27, 0x0a, 0x28, 0xde, 0x23, 0x00, 0x24, 0x84, 
0x25, 0x00, 0x26, 0x80, 0x27, 0x10, 0x28, 0xdf, 0x21, 0x06, 
0x23, 0x63, 0x24, 0x80, 0x25, 0x07, 0x26, 0x2a, 0x27, 0x00, 
0x28, 0xe0, 0x21, 0x00, 0x23, 0x80, 0x24, 0x89, 0x26, 0xaa, 
0x27, 0x0a, 0x28, 0xe1, 0x20, 0x05, 0x23, 0x84, 0x26, 0xb4, 
0x27, 0x09, 0x28, 0xe2, 0x20, 0x70, 0x21, 0x61, 0x22, 0xb8, 
0x23, 0x95, 0x26, 0x12, 0x27, 0x08, 0x28, 0xe3, 0x20, 0x00, 
0x21, 0x01, 0x22, 0x00, 0x23, 0x80, 0x24, 0x86, 0x25, 0xc3, 
0x26, 0xab, 0x27, 0x0a, 0x28, 0xe4, 0x20, 0x42, 0x21, 0xce, 
0x22, 0x0f, 0x24, 0x89, 0x25, 0x07, 0x26, 0xaa, 0x28, 0xe5, 
0x20, 0x70, 0x21, 0x50, 0x22, 0xb8, 0x23, 0x95, 0x26, 0x12, 
0x27, 0x08, 0x28, 0xe6, 0x20, 0x05, 0x21, 0x00, 0x22, 0x00, 
0x23, 0x84, 0x26, 0xb4, 0x27, 0x09, 0x28, 0xe7, 0x20, 0x70, 
0x21, 0x64, 0x22, 0xb8, 0x23, 0x95, 0x26, 0x12, 0x27, 0x08, 
0x28, 0xe8, 0x20, 0x00, 0x21, 0x01, 0x22, 0x00, 0x23, 0x80, 
0x24, 0x86, 0x25, 0xc3, 0x26, 0xab, 0x27, 0x0a, 0x28, 0xe9, 
0x20, 0x42, 0x21, 0xd3, 0x22, 0x0f, 0x24, 0x89, 0x25, 0x07, 
0x26, 0xaa, 0x28, 0xea, 0x20, 0x70, 0x21, 0x51, 0x22, 0xb8, 
0x23, 0x95, 0x26, 0x12, 0x27, 0x08, 0x28, 0xeb, 0x20, 0x05, 
0x21, 0x00, 0x22, 0x00, 0x23, 0x84, 0x26, 0xb4, 0x27, 0x09, 
0x28, 0xec, 0x20, 0x70, 0x21, 0x65, 0x22, 0xb8, 0x23, 0x95, 
0x26, 0x12, 0x27, 0x08, 0x28, 0xed, 0x20, 0x00, 0x21, 0x01, 
0x22, 0x00, 0x23, 0x80, 0x24, 0x86, 0x25, 0xc3, 0x26, 0xab, 
0x27, 0x0a, 0x28, 0xee, 0x20, 0x42, 0x21, 0xd8, 0x22, 0x0f, 
0x24, 0x89, 0x25, 0x07, 0x26, 0xaa, 0x28, 0xef, 0x20, 0x70, 
0x21, 0x52, 0x22, 0xb8, 0x23, 0x95, 0x26, 0x12, 0x27, 0x08, 
0x28, 0xf0, 0x20, 0x05, 0x21, 0x00, 0x22, 0x00, 0x23, 0x84, 
0x26, 0xb4, 0x27, 0x09, 0x28, 0xf1, 0x20, 0x70, 0x21, 0x61, 
0x22, 0xb8, 0x23, 0x95, 0x26, 0x02, 0x27, 0x08, 0x28, 0xf2, 
0x20, 0x00, 0x21, 0x01, 0x22, 0x00, 0x23, 0x80, 0x24, 0x86, 
0x25, 0xc3, 0x26, 0xab, 0x27, 0x0a, 0x28, 0xf3, 0x20, 0x42, 
0x21, 0xdd, 0x22, 0x0f, 0x24, 0x89, 0x25, 0x07, 0x26, 0xaa, 
0x28, 0xf4, 0x20, 0x70, 0x21, 0x50, 0x22, 0xb8, 0x23, 0x95, 
0x26, 0x02, 0x27, 0x08, 0x28, 0xf5, 0x20, 0x05, 0x21, 0x00, 
0x22, 0x00, 0x23, 0x84, 0x26, 0xb4, 0x27, 0x09, 0x28, 0xf6, 
0x20, 0x00, 0x21, 0x01, 0x23, 0x80, 0x24, 0x86, 0x25, 0xcb, 
0x26, 0xab, 0x27, 0x0a, 0x28, 0xf7, 0x20, 0x42, 0x21, 0xe1, 
0x22, 0x0f, 0x24, 0x89, 0x25, 0x07, 0x26, 0xaa, 0x28, 0xf8, 
0x20, 0x00, 0x21, 0x02, 0x22, 0x00, 0x23, 0x00, 0x24, 0x81, 
0x25, 0xc4, 0x26, 0xab, 0x28, 0xf9, 0x21, 0x00, 0x23, 0x80, 
0x24, 0x89, 0x25, 0x48, 0x26, 0xaa, 0x28, 0xfa, 0x20, 0x05, 
0x23, 0x84, 0x25, 0x07, 0x26, 0xb4, 0x27, 0x09, 0x28, 0xfb, 
0x20, 0x70, 0x21, 0x4e, 0x22, 0xb8, 0x23, 0x95, 0x26, 0x10, 
0x27, 0x08, 0x28, 0xfc, 0x20, 0x00, 0x21, 0x00, 0x22, 0x00, 
0x23, 0x80, 0x24, 0x86, 0x25, 0xc3, 0x26, 0xab, 0x27, 0x0a, 
0x28, 0xfd, 0x20, 0x42, 0x21, 0xe7, 0x22, 0x0f, 0x24, 0x89, 
0x25, 0x07, 0x26, 0xaa, 0x28, 0xfe, 0x20, 0x00, 0x21, 0xbb, 
0x22, 0x00, 0x23, 0x95, 0x26, 0x20, 0x27, 0x00, 0x28, 0xff, 
0x20, 0x05, 0x21, 0x00, 0x23, 0x84, 0x26, 0xb4, 0x27, 0x09, 
0x2a, 0x08, 0x10, 0x01, 0x3a, 0x00, 0x64, 0x3a, 0x65, 0xbb, 
0x08, 0x3a, 0x09, 0xbb, 0x50, 0x10, 0x52, 0x67, 0x51, 0x77, 
0x05, 0xa1, 0x18, 0x04, 0xff, 0xff
   };

void sta013HWReset(void)
{
	sbi(DDRD, 2);		// set reset pin to output
	sbi(PORTD, 2);		// clock RESET low
	cbi(PORTD, 2);
	timerPause(10);
	sbi(PORTD, 2);

	// give the sta013 a little time to come out of reset
	timerPause(50);
}

u08 sta013ReadReg(u08 reg)
{
	u08 data;
	i2cReceive(STA_I2C_DEV, reg, 1, &data);
	return data;
}

void sta013WriteReg(u08 reg, u08 data)
{
	i2cSend(STA_I2C_DEV, reg, 1, &data);
}

void sta013DownloadUpdate(void)
{
	u16 i;
	u08 reg, data;

	i=0;
	// get first reg/data pair
	reg = pgm_read_byte(STA013_UpdateData + i++);
	data = pgm_read_byte(STA013_UpdateData + i++);
	// loop until end of update
	while( (reg != 0xff) )
	{
		sta013WriteReg(reg, data);
		reg = pgm_read_byte(STA013_UpdateData + i++);
		data = pgm_read_byte(STA013_UpdateData + i++);
	}
}


u08 sta013Init(void)
{
	// reset STA013 device
	sta013HWReset();

	// identify STA013 device
	if(sta013ReadReg(STA_REG_IDENT) != STA_IDENT)
	{
		return FALSE;
	}

	// do firmware configuration and update
	sta013DownloadUpdate();
	// start decoder
	sta013StartDecoder();
	
	return TRUE;
}


void sta013StartDecoder(void)
{
	// Soft reset
	sta013WriteReg(STA_REG_SOFT_RESET,			0x01);
	sta013WriteReg(STA_REG_SOFT_RESET,			0x00);

	// Mute and configure DAC output
	sta013WriteReg(STA_REG_MUTE,				0x01);
	sta013WriteReg(STA_REG_PCMDIVIDER,			0x01);	// 32-bit mode, O_FAC = 256
	sta013WriteReg(STA_REG_PCMCONF,				0x31);	// 18-bit mode & more

	// Configure PLL for MP3 rates
	sta013WriteReg(STA_REG_PLLFRAC_441_H,		0x67);
	sta013WriteReg(STA_REG_PLLFRAC_441_L,		0x77);
	sta013WriteReg(STA_REG_PLLFRAC_H,			0xbb);
	sta013WriteReg(STA_REG_PLLFRAC_L,			0x3a);
	sta013WriteReg(STA_REG_MFSDF_441,			0x10);
	sta013WriteReg(STA_REG_MFSDF,				0x0F);

	// Configure interface polarities, etc
	sta013WriteReg(STA_REG_PLLCTL_2,			0x0C);
	sta013WriteReg(STA_REG_PLLCTL_3,			0x00);
	sta013WriteReg(STA_REG_PLLCTL_1,			0xA1);
	sta013WriteReg(STA_REG_SCLK_POL,			0x00);	// data sampled on rising edge
	sta013WriteReg(STA_REG_REQ_POL,				0x01);	// REQ line active high
	sta013WriteReg(STA_REG_DATA_REQ_ENABLE,		0x04);
	sta013WriteReg(STA_REG_PLLCTL_1,			0xA1);

	// Set audio tone controls
	sta013SetTone(0, 0, 0, 0);

	// Unmute and start running
	sta013WriteReg(STA_REG_RUN,  0x01);
	sta013WriteReg(STA_REG_PLAY, 0x01);
	sta013WriteReg(STA_REG_MUTE, 0x00);
}


void sta013StopDecoder(void)
{
	// mute output
	sta013WriteReg(STA_REG_MUTE, 0x01);
	// soft reset
	sta013WriteReg(STA_REG_SOFT_RESET, 0x01);
	sta013WriteReg(STA_REG_SOFT_RESET, 0x00);
}


void sta013PauseDecoder(void)
{
	// enable mute
	sta013WriteReg(STA_REG_MUTE, 0x01);
	// stop the decoder
	sta013WriteReg(STA_REG_PLAY, 0x00);
}


void sta013ResumeDecoder(void)
{
	// run the decoder
	sta013WriteReg(STA_REG_PLAY, 0x01);
	// disable mute
	sta013WriteReg(STA_REG_MUTE, 0x00);
}

void sta013GetMP3Info(u16 *bitrate, u08 *sampFreq, u08 *mode)
{
	u08 headL, headM, headH;
	u08 mpegID, bitrateIndex, sampFreqIndex;
	
	// get the MP3 header info
	headH = sta013ReadReg(STA_REG_HEAD_H);
	headM = sta013ReadReg(STA_REG_HEAD_M);
	headL = sta013ReadReg(STA_REG_HEAD_L);

	// IDex:ID is in head[20:19]
	// 00 - MPEG2.5
	// 01 - reserved
	// 10 - MPEG2
	// 11 - MPEG1
	mpegID = (headH & 0x18)>>3;

	// sampling frequency is in head[11:10]
	sampFreqIndex = ((headM & 0x0C)>>2) | (mpegID<<2);

	// bitrate index is in head[15:12]
	bitrateIndex = ((headM & 0xF0)>>4) | ((mpegID & 0x01)<<4);
	//bitrateIndex = ((headM & 0xF0)>>4) | (1<<4);
	
	// mode is in head[7:6]
	// 00 - stereo
	// 01 - joint stereo
	// 10 - dual channel
	// 11 - single channel (mono)
	*mode = (headL & 0xC0)>>6;
	
	*bitrate =	2 * pgm_read_byte( MP3_Bitrates + bitrateIndex );
	*sampFreq = pgm_read_byte( MP3_SamplingFrequencies + sampFreqIndex );

/*
   header = (unsigned long)sta013ReadReg(STA_REG_HEAD_H) << 16 |
       (unsigned long)sta013ReadReg(STA_REG_HEAD_M) << 8  |
       (unsigned long)sta013ReadReg(STA_REG_HEAD_L);

//   hdr->word           = l;
//   hdr->emphasis       = l & 0x03;
//   hdr->isOriginal     = (l >> 2) & 0x01;
//   hdr->isCopyrighted  = (l >> 3) & 0x01;
//   hdr->modeExtension  = (l >> 4) & 0x03;
//   hdr->mode           = (l >> 6) & 0x03;
//   hdr->private        = (l >> 8) & 0x01;
//   hdr->padding        = (l >> 9) & 0x01;
//   hdr->frequencyIndex = (l >> 10) & 0x03;
//   hdr->bitrateIndex   = (l >> 12) & 0x0f;
//   hdr->protection     = (l >> 16) & 0x01;
//   hdr->layer          = (l >> 17) & 0x03;
//   hdr->ID             = (l >> 19) & 0x01;
//   hdr->ID_ex          = (l >> 20) & 0x01;
*/
}

u16 sta013GetAverageBitrate(void)
{
	return (2 * sta013ReadReg(STA_REG_AVERAGE_BITRATE));
}

void sta013SetVolume(u08 volume, s08 balance)
{
	char attenL, attenR;

	// volume is expected as 0-100 value
	// Note:
	//	#define MIN_VOLUME_ATTENUATION     0
	//	#define MAX_VOLUME_ATTENUATION     96

	if( balance > 0)
	{	// balance to the left, attenuate right
		attenL = (100 - volume);
		attenR = (100 - volume) - (balance);
	}
	else
	{	// balance to the right, attenuate left
		attenL = (100 - volume) + (balance);
		attenR = (100 - volume);
	}
	// respect limits
	attenL = MIN(attenL,MAX_VOLUME_ATTENUATION);
	attenL = MAX(attenL,MIN_VOLUME_ATTENUATION);
	attenR = MIN(attenR,MAX_VOLUME_ATTENUATION);
	attenR = MAX(attenR,MIN_VOLUME_ATTENUATION);

	// set volume
	sta013WriteReg(STA_REG_DLA, attenL);
	sta013WriteReg(STA_REG_DLB, MAX_VOLUME_ATTENUATION);
	sta013WriteReg(STA_REG_DRA, attenR);
	sta013WriteReg(STA_REG_DRB, MAX_VOLUME_ATTENUATION);
}


void sta013SetTone(s08 bassEnh, u16 bassFreq, s08 trebleEnh, u16 trebleFreq)
{
	// set bass enhancement
	sta013WriteReg(STA_REG_BASS_FREQUENCY_LOW,  (bassFreq   ) & 0xFF );
	sta013WriteReg(STA_REG_BASS_FREQUENCY_HIGH, (bassFreq>>8) & 0xFF );
	// respect limits
	bassEnh = MIN(bassEnh,MAX_BASS_ENHANCE);
	bassEnh = MAX(bassEnh,MIN_BASS_ENHANCE);
	sta013WriteReg(STA_REG_BASS_ENHANCE, bassEnh);

	// set treble enhancement
	sta013WriteReg(STA_REG_TREBLE_FREQUENCY_LOW,  (trebleFreq   ) & 0xFF );
	sta013WriteReg(STA_REG_TREBLE_FREQUENCY_HIGH, (trebleFreq>>8) & 0xFF );
	// respect limits
	trebleEnh = MIN(trebleEnh,MAX_TREBLE_ENHANCE);
	trebleEnh = MAX(trebleEnh,MIN_TREBLE_ENHANCE);
	sta013WriteReg(STA_REG_TREBLE_ENHANCE, trebleEnh);

	// set attentuation to avoid clipping
	sta013WriteReg( STA_REG_TONE_ATTEN, MAX(bassEnh,trebleEnh) );
}

u08 sta013Demand(void)
{
	return bit_is_set(STA013_DEMAND_PORTIN,STA013_DEMAND_PIN);
}

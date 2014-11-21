#ifndef __LCD_H__
#define __LCD_H__

#define  LCD5110_DC_PIN 		0x80000000   					// P0.31 (x000 0000 0000 0000 0000 0000 0000 0000)
#define  LCD5110_RES_PIN		0x00800000   					// P0.23 (0000 0000 x000 0000 0000 0000 0000 0000)
#define  LCD5110_SCE_PIN  		0x00200000   					// P0.21 (0000 0000 00x0 0000 0000 0000 0000 0000)

#define  LCD5110_DC_HIGH()  	IOSET0  |= LCD5110_DC_PIN		// D/C  = '1'(Data) 
#define  LCD5110_DC_LOW()  		IOCLR0  |= LCD5110_DC_PIN		// D/C  = '0'(Command)
#define  LCD5110_RES_HIGH()  	IOSET0  |= LCD5110_RES_PIN		// RES  = '1'(Normal) 
#define  LCD5110_RES_LOW()  	IOCLR0  |= LCD5110_RES_PIN		// RES  = '0'(Reset)
#define  LCD5110_SCE_HIGH() 	IOSET0  |= LCD5110_SCE_PIN		// SCE  = '1'(Disable) 
#define  LCD5110_SCE_LOW() 		IOCLR0  |= LCD5110_SCE_PIN		// SCE  = '0'(Enable)

/* pototype  section */
void lcd_write_data(unsigned char DataByte);					// Write Data to LCD
void lcd_write_command(unsigned char CommandByte);				// Write Command to LCD
void lcd_initial(void);											// Initial LCD Nokia-5110
void lcd_clear_screen(void);									// Clear Screen Display
void lcd_fill_picture(unsigned char picture[]);					// Fill Picture Display
void lcd_gotoxy(unsigned char x,unsigned char y); 				// Set Cursor X(0..83),Y(0..5) 
void lcd_put_char(unsigned char character); 					// Put String(1 Char)
void lcd_print_string(unsigned char *string , 
                      unsigned char CharCount); 				// Print String to LCD
#endif
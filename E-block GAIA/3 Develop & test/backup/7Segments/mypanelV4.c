    
/*
    Title	: Mypanel board firmware V.2 for LED 16 x 48 Dots
	File 	: mypanelV4.c
    Author	: Wichai Tossamartvorakul  (wichai@geartronics.net)
    Date	: 11/2005
    Purpose : Control the LED Message board Mypanel
	Software: AVR-GCC
    Hardware: ATMEGA32

UPDATE HISTORY	
	Algorithm  : SCAN VERTICAL  16 lines
    Modify from  mypanel16.c  (ATMEGA163)
	Modified for TLC
	Modified for 74LS164
	Last update 25/3/2005
	
	Algorithm  : SCAN VERTICAL  16 lines
    Modify from  mypanel32.c  (ATMEGA163)
	Modified for TLC, 74HC595
	Modified for Mypanel Version 3 PCB
	Last update 15/6/2005
	
	Algorithm  : Remove g_buffer and calculate on the fly for character dot to save memory space
	Modify from mypanelV3.c
	For Mypanel Version 4 PCB
	Last udate 10/10/2005
	
	Divided into modular block for E-block project std
	Last update 28/3/2005
	
	
*/

// Note to do Change all i,j,k to global. May save some code in initilaize these variable in every subroutine.


#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "MypanelV4.h"
#include "ir_tv.h"
#include "ps2_kbd.h"
#include "led_matrix.h"
#include "delay.h"
#include "scancodes.h"
#include "text_decode.h"




 

//Predefined  text in EEPROM

int8_t  show_msg[LED_MATRIX_ROW_NUM] EEPROM =  {0,1};

// Becareful This array keep 2 data type 
//  1 Text    = Data (TEXT_SIZE) + Attribute (TEXT_ATTRIBUTE)
//  2 Graphic = DataRed (SLOT_SIZE/2) + DataGreen (SLOT_SIZE/2)
//  Need to make sure Attribute = 0  (RED) to show characters If it is garbage, It will not show 




uint8_t g_msg[SLOT_NUM][SLOT_SIZE] EEPROM = { 
							"Hello ! ",
							"Slot 1 \"x\"",
							"สู่รู้วันนี้  สู่รู้วันนี้  ",
							"zzzzzzzzzzzzzzzzzzzzzzzzzzzzz",
							"30% -70%",
							"BUY 1 GET 1 FREE",
							"Slot 6",
							"Slot 7",
							"Geartronics",
							"Welcome"};
struct control_code  g_control[SLOT_NUM]  EEPROM = { {20,5,0,0},
											{20,5,1,0},
											{20,3,2,0},
											{20,5,3,0},
											{20,5,4,0},
											{20,1,5,0},
											{20,3,6,0},
											{20,4,7,0},
											{20,4,8,0},
											{20,3,9,0}};
											


/* Ram Data structure
   Becareful for data beyound allocate memory 
*/
 
//Global variable which eat space in ram 


// Glolbal Data Structure & Global Variable begin with g_

// Global Data structure use Interupt service routine USE with care
// Use by remote control



// Global Data use by keyboard routine
uint8_t  	g_edge, g_bitcount;          	// 0 = neg.  1 = pos.

// Global data use by  system and Application API

volatile   int8_t 	g_buffcnt;

uint8_t   	g_row = 0;						// Active row
int16_t   	g_current_col;  				// The current_cols of the scan line
int8_t		g_ptr;							// Pointer to character in Buffer
int8_t      g_offset;						// Offset for calculate character to display on mypanel board use with g_ptr
int8_t		g_cursor;						// cursor position on my panel board ( Move by base level characters only)
int8_t      g_display;						// Offset for calculate column to display on mypanel board use with g_cursor
uint8_t   	g_irint =0;						// Ir interrupt
uint8_t   	g_kbdint=0;						// g_kbdint;
int8_t      g_next_slot  = -1;				// Slot number for put in next slot member in control code structure
uint8_t     g_display_row = LED_MATRIX_DOT_PER_BOARD;
int8_t  	g_num[LED_MATRIX_ROW_NUM];      // Number of message in EEPROM for upper row and lower
uint8_t     g_rcv_buff[SLOT_SIZE]= {"slot = 1 text = \"Text from  SERIAL Port\" display_mode = 2 lines = 0  style = 4   "}; 			// Receive buffer for UART
uint8_t     g_rcv_ptr;						// Receive buffer pointer 
uint8_t     g_sync_flag;
/* Keep position of vowels 
    bit 1 - 2     Lower vowel
	bit 3 - 4     Upper vowel
	bit 5 - 6     tone mark

*/

uint8_t 	g_kbd_buffer[TEXT_SIZE];
uint8_t	  	g_display_mode;								// Display mode ( 1 Line /2 Lines mode)
uint8_t	  	g_style[LED_MATRIX_ROW_NUM];
uint8_t		g_cmd_decode;								// 1 = Ready to decode 

 
// External variable 

extern  unsigned char led_matrix_ascii_width[]; 
 

 /*
   This subroutine will copy the data from keyboard buffer into spcific RAM area g_buffer
   and also copy the attribut to attr variable
   
   Input : g_kbd_buff
           
   Output: g_buffer
*/
int16_t kbdbuff2ram (uint8_t row)
{  

   int16_t length;
   
	led_matrix_clear_video_ram (row);
 
// NOTE : May need to add Check EEPROM ready, But the duration for reading each byte should be enough
 
    length =  led_matrix_put_in_video_ram (row,&g_kbd_buffer[g_offset],g_style[row]);

	return length;
  
}



 /*
   This subroutine will copy the data from slot in EEPROM into spcific RAM area
   If the message type is graphic. It will copy one by one to display_buffer (not use Char gen)
   If the message type is text.  It will copy the text g_buffer on RAM 
   and also copy the attribut to attr variable
   
   Input : Global char txt  String to show (This will copy from eePROM at the start up)
		 : g_num[]  		Order of message in EEPROM
 		   row          	row number to copy
		   g_msg in EEPROM
		   g_control
   Output: g_buffer  Global display buffer
           g_style   Style 
 		   number line of text/graphic
   Relate file chargen.h  to get the character generator to put in video ram
*/
int16_t eeprom2ram (uint8_t row, int8_t no)
{  
   int16_t length;   
   
   
// Clear videoram
	led_matrix_clear_video_ram (row);
	length  = 0;
 
 
// NOTE : May need to add Check EEPROM ready, But the duration for reading each byte should be enough
   if ((g_style[row] & LED_MATRIX_GRAPHIC))    //  Graphic Display
   {   
	    led_matrix_put_graphic_video_ram (row,&g_msg[no][0]);
		return (g_display_row-1);    // Start from 0 
   }
   else                 // Text
   {
 		length =  led_matrix_put_in_video_ram(row,&g_msg[no][0],g_style[row]);
// Copy text attributes 
		led_matrix_copy_attribute (row,&g_msg[no][0]);
 		return length;
  
	} 
 
} 
 
	
/* This subroutine will put the text from EEPROM into kbd buffer  before output to message board
   Input : Global char txt  String to show (This will copy from eePROM at the start up)
		 : no = Number of message in EEPROM
		   g_msg in EEPROM
   Output: g_kb_buffer		Keyboard buffer
		   number of characters copied from eeprom

*/
int8_t e2kbdbuff (int8_t no)
{
    uint8_t i = 0;
    uint8_t ch;
 
// NOTE : May need to add Check EEPROM ready, But the duration for reading each byte should be enough
 
	while ((ch = eeprom_read_byte(&g_msg[no][i])) && (i < TEXT_SIZE))   //Get EEPROM BYTE/BYTE
	{
	   g_kbd_buffer[i] = ch;
 		i++;
    }
	
     g_kbd_buffer[i] = 0x0; // Null string
	 // Copy text attributes 
	 
	 led_matrix_copy_attribute (0,&g_msg[no][0]);
     led_matrix_copy_attribute (1,&g_msg[no][0]); 
	 
// Reset to first column
	g_cursor = 0;
	g_display = 0;
	g_offset = 0;
	g_ptr     = 0;
 
	return i;
}
 

/* This subroutine put the character into keyboard buffer.
*/
void put_kbbuff(unsigned char c, int8_t color)
{
uint8_t     *outpt;
uint8_t     tmp,ch,width,n;
int8_t      col;

        if (g_buffcnt == 0)                                  // First character put at the cursor
   		{
            outpt =  &g_kbd_buffer[0];
		  *outpt = c;
		  *outpt++;
		  *outpt = 0x00;
     	  g_ptr = 0;
		  g_cursor = 0;
		  g_display = 0;
		  g_offset = 0;
		  g_buffcnt++;
 		}
		else
		{
  	     	if (g_buffcnt < (SLOT_SIZE-1))                        // If buffer not full -1 because of null string
			{
				outpt =  &g_kbd_buffer[g_ptr];
				outpt++;			// Put the character at the right side
// If next character is vowel move to next char
				do
				{
					ch = *outpt;
 					width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
					if (width == 0)
					{	
						g_ptr++;
						outpt++;
					}	
				} while (width == 0);

				ch = *outpt;
				tmp = c;
				*outpt = tmp;
				outpt++;
				while ((tmp = *outpt))
				{ 
					*outpt = ch;
					outpt++ ;
					ch = tmp; 	
				}
			
// Put Null string at the end
				*outpt = ch;
				outpt++;			
				*outpt = 0x00;
				g_buffcnt++;
				g_ptr++;
								
				if (g_ptr >= SLOT_SIZE)  // For text length < SLOT_SIZE
				{
					g_ptr = SLOT_SIZE-1;
				}
				
			
// Check if vowel cursor is not move
				width =  pgm_read_byte(&led_matrix_ascii_width[c]); 
  			    if (width != 0)      // Consonent not vowels
						g_cursor++;
 
				if (g_cursor >= LED_MATRIX_KBD_SHOW_SIZE)						// Check the range
				{
					g_cursor = LED_MATRIX_KBD_SHOW_SIZE-1;
	
// Move g_offset = number of characters including vowels	
					n = 0;
					do
					{
						g_offset++;
						n++;
						ch = g_kbd_buffer[g_offset];
						if ( ch == 0x00 )  // Null characters at end of text
						{
// Restore pointer to original		
							g_offset -= n;
							n = 0;
							break;
						}
						width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
					} while (width == 0);	// End of text
		 
					if (g_offset > g_buffcnt - LED_MATRIX_KBD_SHOW_SIZE)   // Reach end of buffer
						g_offset =g_buffcnt - LED_MATRIX_KBD_SHOW_SIZE;
					g_display ++;		
					if (g_display > g_buffcnt - LED_MATRIX_KBD_SHOW_SIZE)   // Reach end of buffer
						g_display =g_buffcnt - LED_MATRIX_KBD_SHOW_SIZE;
 				}
// If last character is vowel must move g_ptr back to firsr consonat	
				g_ptr++;
				do
				{
					 g_ptr--;
					 if (g_ptr < 0)
						 g_ptr = 0;
					ch = g_kbd_buffer[g_ptr];
					width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
				} while (width == 0);
			}		
        }	
		
// Put character attribute
				col = (g_display+g_cursor)/2;

				if((g_display+g_cursor)%2)
				{			
					color <<= 4;			   //  1 bytes 2 char
					color  &=  0xF0;
					tmp  = led_matrix_get_attribute(0,col) & 0x1F;
					tmp  |=  color;
					led_matrix_set_attribute (0,col, tmp);
				
					tmp  = led_matrix_get_attribute(1,col) & 0x1F;	
					tmp  |=  color;
					led_matrix_set_attribute (1,col, tmp);
// Restore color back
					color >>= 4;
				
				}else	
				{ 	tmp  = led_matrix_get_attribute(0,col) & 0xF0;
					tmp  |= color;
					led_matrix_set_attribute (0,col, tmp);

					tmp  = led_matrix_get_attribute(1,col) & 0xF0;
 					tmp  |=  color;
					led_matrix_set_attribute (1,col, tmp);
 				}
}

  
void init_kb(void)
{
	g_edge = 0;                               // 0 = falling edge  1 = rising edge
    g_bitcount = 11;
 	g_ptr =  0;
	g_cursor = 0;
    g_buffcnt = 0;
 
}




void init_board (void)
{
// Initialize kbd
   init_kb();
   
// Initialize port	
 	DDRA = 0xFF;   // All port A for output
	DDRB = 0xFE;
	DDRC = 0x00;
// Enable pull up
    PORTD = 0xFF;	
	DDRD = 0x00;	 
 
	g_next_slot = -1;
//  Initialize  value
	g_display_mode =  LED_MATRIX_TWOROWS;		// 2 rows mode
//  Initialize for interupt

//   g_display_row =  led_matrix_get_display_row();
	g_display_row = 48;
	
	g_sync_flag  = 0; 
      	
	
	
}



/*  This routine move cursor left one position if text is Thai. The cursor will move left to the next base
characters (skip vowel)
	Global variable effect
				g_cursor
				g_display
				g_offset
				g_ptr;

*/
void  move_cursor_left (void)
{
	uint8_t ch,width; 
	do
	{
		 g_ptr--;
		 if (g_ptr < 0)
			 g_ptr = 0;
		ch = g_kbd_buffer[g_ptr];
		width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
	
	} while (width == 0);
// Move left to get cursor at the end
// Check g_offset for skip vowel

	g_cursor--;
	if (g_cursor < 0)						// Check the range
	{
 		g_cursor = 0;
		do
		{
			g_offset--;
		    if (g_offset < 0)
				 g_offset = 0;
			ch = g_kbd_buffer[g_offset];
			width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
		} while (width == 0);
		
		g_display--;
		if (g_display < 0 )
				g_display = 0;
 	}


}

/*  This routine move cursor right one position if text is Thai. The cursor will move right to the next base
characters (skip vowel)
	Global variable effect
				g_cursor
				g_display
				g_offset
				g_ptr;

*/
void  move_cursor_right (void)
{
	uint8_t n,ch,width;
 	n = 0;
	do
	{
		g_ptr++;
		n++;
		ch = g_kbd_buffer[g_ptr];
		if ( ch == 0x00 )  // Null characters at end of text
		{
// Restore pointer to original		
            g_ptr -= n;
			n = 0;
			break;
		}
		
		width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
 

    } while (width == 0);	// End of text
	
	if (n)   // N != 0 mean can move cursor
    {	g_cursor++;
		if ((g_buffcnt < LED_MATRIX_KBD_SHOW_SIZE) && (g_cursor >= g_buffcnt))  // For text length < SHOW_SIZE
		{
			g_cursor = g_buffcnt-1;
		}
		if (g_cursor >= LED_MATRIX_KBD_SHOW_SIZE)						// Check the range
		{
			g_cursor = LED_MATRIX_KBD_SHOW_SIZE-1;
// Move g_offset = number of characters including vowels	
            n = 0;
			do
			{
				g_offset++;
				n++;
				ch = g_kbd_buffer[g_offset];
				if ( ch == 0x00 )  // Null characters at end of text
				{
// Restore pointer to original		
					g_offset -= n;
					n = 0;
					break;
				}
				width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
			} while (width == 0);	// End of text
		 
			if (g_offset > g_buffcnt - LED_MATRIX_KBD_SHOW_SIZE)   // Reach end of buffer
				g_offset =g_buffcnt - LED_MATRIX_KBD_SHOW_SIZE;
 			g_display ++;		
			if (g_display > g_buffcnt - LED_MATRIX_KBD_SHOW_SIZE)   // Reach end of buffer
				g_display =g_buffcnt - LED_MATRIX_KBD_SHOW_SIZE;
  
		}
	}	
	
}

 
//API for Hardware interface

/* This routine decode keyboard code and put in keyboard buffer
   INPUT   scan code to decode
*/   
   

void kbd_decode(uint8_t sc)
{
    static unsigned char is_up=0,extend_key=0,cap_lock = 0,shift=0, edit_mode =0;
	static uint8_t    thai_mode = 0;
	static int8_t color =0;
    int8_t  i,col;
	uint8_t ch,style,width,tmp;
 
static uint8_t 	*outpt ;
	
// !!!BECAREFUL THAT ALL KEY IS PROCESS IN g_kdb_buff BUT JUST NOT SHOW BECAUSE g_kbdint had not been set.
 
    if (!is_up)                // Last data received was the up-key identifier
    {
        //outp(sc,PORTB);                   // show me scancode - for debuging
        switch (sc)
        {
          case PS2_KBD_RELEASE: 
				is_up = 1;
				break;      // The up-key identifier
		  case PS2_KBD_EXTEND: 
				extend_key =1; 
				break;  //  Extend key detect
          case PS2_KBD_LEFT_SHIFT: 
          case PS2_KBD_RIGHT_SHIFT: 
				shift = 1; 
				break;      // Right SHIFT
 		  case PS2_KBD_F1:									// F1
// Initialize variable before decode and save text
// PROGRAM STYLE TO TEXT MODE
				 style = g_style[g_row];
				 if (style & LED_MATRIX_GRAPHIC)   // Graphic 
				 { 
					 style   &=  0x0F;    // Clear graphic bit  MO
					 g_style[g_row] = style;   // Mark graphic bit
					 while (!eeprom_is_ready());
						  eeprom_write_byte(&(g_control[g_num[g_row]].show_mode),style);
					 g_buffcnt = 0;
					 g_kbd_buffer[0] = 0x0; // Null string
 				 }
				else
					g_buffcnt =  e2kbdbuff(g_num[g_row]);		//Copy to kbd buffer
					
 				
				g_kbdint = 1;
				g_offset =  0;
				g_display = 0;
				g_ptr  = 0;
				g_cursor = 0;
				edit_mode = 1;
				break;
//			case F2:
//				light_pen();
//				break;
			case PS2_KBD_F3:								// Put character attribute
			    color++;
				if (color > LED_MATRIX_MAX_COLOR-1)
					color = LED_MATRIX_RED;
 
// Put character attribute for vowel if any
  				col = (g_display+g_cursor)/2;
					if((g_display+g_cursor)%2)
					{
						color <<= 4;			   //  1 bytes 2 char
						color  &=  0xF0;
	
					    tmp  = led_matrix_get_attribute(0,col) & 0x0F;
						tmp  |= color;
						led_matrix_set_attribute (0,col, tmp);
						
						tmp  = led_matrix_get_attribute(1,col) & 0x0F;
						tmp  |= color;
						led_matrix_set_attribute (1,col, tmp);			
// Restore color back
						color >>=  4;
					
					}else	
					{
						tmp  = led_matrix_get_attribute(0,col) & 0x0F;
						tmp  |=  color;
						led_matrix_set_attribute (0,col, tmp);
						
						tmp  = led_matrix_get_attribute(1,col) & 0x0F;
 						tmp  |= color;
						led_matrix_set_attribute (1,col, tmp);
					}
	 
 				break;
			case PS2_KBD_BS:						    // Prevent put code in buffer
  				break;
		    case PS2_KBD_LEFT: 						   //  Left arrow  Why go to 2B get this
				if (extend_key)
				{
					extend_key = 0;
					move_cursor_left();
 				}
				break;
		case PS2_KBD_RIGHT: 						   // Right arrow
				if (extend_key)
				{	
					extend_key =0;
					move_cursor_right();
  				}	
				break;
		case PS2_KBD_DEL:
		
     			if (extend_key)
				{
					extend_key = 0;
					if (g_buffcnt == 0 )    // No more character
					      break;
					do
					{		   	
					   outpt = &g_kbd_buffer[g_ptr];
					   if (*outpt != 0x00)
					   {
						   outpt++;	
						   while (( ch = *outpt))
							{
								outpt--;
								*outpt = ch;
								outpt++;
								outpt++;
							}
							outpt--;
							*outpt = 0x00;
							g_buffcnt--;
							if (g_buffcnt < 0)    // Negative
							{
								g_buffcnt == 0;
							}
						}	
						ch = g_kbd_buffer[g_ptr];
						width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
						
					} while (width == 0);	

					if (g_ptr >= g_buffcnt-1)	// If end of line delete need to back cursor
					{
 						do
						{
							 g_ptr--;
							 if (g_ptr < 0)
								 g_ptr = 0;
							 ch = g_kbd_buffer[g_ptr];
							 width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
						} while (width == 0);
 						 g_cursor--;
						 if (g_cursor < 0)						// Check the range
						 {
							g_cursor = 0;
							do
							{
								g_offset--;
								if (g_offset < 0)
									 g_offset = 0;
								ch = g_kbd_buffer[g_offset];
							width =  pgm_read_byte(&led_matrix_ascii_width[ch]); 
							} while (width == 0);
							g_display--;
							if (g_display < 0 )
								g_display = 0;
						 }
					}
				}
				
				break;
		case PS2_KBD_UP: 						   //  Up arrow
		  // Move message to next message
				  if ((extend_key) && edit_mode)
				   {
				   extend_key = 0;
				   g_num[g_row]++;
					if ( g_num[g_row] > SLOT_NUM-1)
					   g_num[g_row] = 0;
					while (!eeprom_is_ready());
						eeprom_write_byte(&show_msg[g_row],g_num[g_row]);
					g_buffcnt =  e2kbdbuff(g_num[g_row]);
 					g_irint =1;							// To break the current showing mode
					}
				break;
		  case PS2_KBD_DOWN: 						   //  Down arrow
				  if ((extend_key) && edit_mode)
				   {
				   extend_key = 0;
					g_num[g_row]--;
					if ( g_num[g_row] < 0)
					   g_num[g_row] = SLOT_NUM-1;   
					while (!eeprom_is_ready());
						eeprom_write_byte(&show_msg[g_row],g_num[g_row]);
					g_buffcnt = e2kbdbuff(g_num[g_row]);
 					g_irint = 1;							// To break the current showing mode
					}  
					break;
			case PS2_KBD_ENTER:									// Protect Enter key put in buffer
					edit_mode  = 0;
					break;
			default:  

// Put character in kbd buffer
            if(!shift && !cap_lock)                    // If shift not pressed,
            {                             // do a table look-up
// Check for Thai characters
				if (thai_mode)
				{
					for(i = 0; pgm_read_byte(&thai_unshifted[i][0]) !=sc && pgm_read_byte(&thai_unshifted[i][0]); i++);
					if (pgm_read_byte(&thai_unshifted[i][0]) == sc) 
					{
						put_kbbuff(pgm_read_byte(&thai_unshifted[i][1]),color);
					}
	
				}
				else
				{
					for(i = 0; pgm_read_byte(&eng_unshifted[i][0]) !=sc && pgm_read_byte(&eng_unshifted[i][0]); i++);
					if (pgm_read_byte(&eng_unshifted[i][0]) == sc) 
					{
						put_kbbuff(pgm_read_byte(&eng_unshifted[i][1]),color);
 					}
				}	
            } 
			else 
			{                    // If shift pressed
				if (thai_mode)
				{
				   for(i = 0; pgm_read_byte(&thai_shifted[i][0])!=sc && pgm_read_byte(&thai_shifted[i][0]); i++);
					if (pgm_read_byte(&thai_shifted[i][0]) == sc) 
					{
						put_kbbuff(pgm_read_byte(&thai_shifted[i][1]),color);
					}

				}
				else
				{
				   for(i = 0; pgm_read_byte(&eng_shifted[i][0])!=sc && pgm_read_byte(&eng_shifted[i][0]); i++);
					if (pgm_read_byte(&eng_shifted[i][0]) == sc) 
					{
						put_kbbuff(pgm_read_byte(&eng_shifted[i][1]),color);
 					}
				}
            } 
			break;
        }
    }
    else {									//Release key
        is_up = 0;                            // Two 0xF0 in a row not allowed
        switch (sc)
        {
          case PS2_KBD_LEFT_SHIFT: 
			  shift = 0; 
			  break;                      // Left SHIFT
          case PS2_KBD_RIGHT_SHIFT: 
			  shift = 0; 
			  break;                      // Right SHIFT
 		  case PS2_KBD_CAPS: 
			  cap_lock ^= 1; 
			  break;  			   			// Cap lock
		  case PS2_KBD_ALT:							// Toggle Thai/Eng
		      thai_mode ^= 1; 
			  break;  		
		  case PS2_KBD_ENTER :  							// Enter key save buffer into RAM
			  		for ( i =0; i < g_buffcnt+1; i++)    // Include NULL character at the end
                    {   
						 while (!eeprom_is_ready());
					     eeprom_write_byte(&g_msg[g_num[g_row]][i],g_kbd_buffer[i]);	 
					}
// Keep text attribute
					for ( i =0; i < TEXT_ATTRIBUTE; i++)    
                    { 
						 while (!eeprom_is_ready());
						 tmp = led_matrix_get_attribute (0,i);
					     eeprom_write_byte(&g_msg[g_num[g_row]][i+TEXT_SIZE],tmp);
					}
 					g_buffcnt = 0;
 					g_kbdint  = 0; 
					g_offset = 0;
 					edit_mode  = 0;
// Enable  interrupt scan timer0
//					TIMSK  = TIMSK | ( 1<< TOIE0);	
					break;
          case 0x76 : 								// ESC Key
					g_kbdint  = 0; 
					g_buffcnt = 0;
					g_offset = 0;
 					edit_mode  = 0;
// Enable  interrupt scan timer0
//					TIMSK  = TIMSK | ( 1<< TOIE0);	
					break;   

		}
    }
 
}

uint8_t light_pen(uint8_t row)
{
//Clear video buffer first 
    static uint8_t color =0;
	uint8_t  i,j,k,col,pos,found,con = 1;
   

// Scan the line to detect X position


     found = led_matrix_light_pen_pos (row, &i, &j);
	 
    if (found)
	{
	 	if (j == 0)   //Command line
		{
			switch (i)
			{
				case 0:
					con = 0;
					break;
				case  2:
				    color = LED_MATRIX_RED;
					break;
				case  3:
				    color = LED_MATRIX_GREEN;
					break;
				case  4:
					color = LED_MATRIX_ORANGE;
					break;
				case 7:
					led_matrix_clear_video_ram (row);
 					break;			
			}
 		}
		else
		{
		// Adjust value for col and j 
		   col  = 1 << i;
		   k    = (g_display_row-1)-j;    // Reverse the position start from 0 - 47
 		   switch (color)
		   {
				case LED_MATRIX_RED:
				   col ^= led_matrix_get_video_ram (row,k);
				   led_matrix_set_video_ram (row, k, col);	  
			 	   break;
				case LED_MATRIX_GREEN:
				    col ^= led_matrix_get_video_ram (row,k+g_display_row);
				   led_matrix_set_video_ram (row, k+g_display_row, col);	  
 			 	   break;
				case LED_MATRIX_ORANGE:
				   col ^= led_matrix_get_video_ram (row,k);
				   led_matrix_set_video_ram (row, k, col);	 
				   col ^= led_matrix_get_video_ram (row,k+g_display_row);
				   led_matrix_set_video_ram (row, k+g_display_row, col);	  
				   break;
				default:
					col ^= led_matrix_get_video_ram (row,k);
				   led_matrix_set_video_ram (row, k, col);	  
				   
			}
 		}
	 }
 
  
 	if (!con)
	{
// Save data in EEPROM and Set the control_code to graphics	
// Turn off LED to longer LED life 
		for (pos = 0 ; pos < SLOT_SIZE ; pos++)  
		{
			
			col = led_matrix_get_video_ram (row,pos);
			while (!eeprom_is_ready());
				eeprom_write_byte(&g_msg[g_num[row]][pos],col);
		}
			while (!eeprom_is_ready());
				eeprom_write_byte(&(g_control[g_num[row]].show_mode),g_style[g_row]);
	}
    return con;	
}
 

/* signal handler for Keyboard external interrupt int0 */

SIGNAL(SIG_INTERRUPT0)
{
    static uint8_t data;                // Holds the received scan code
// Disable  interrupt scan timer0
	if ((data = ps2_kbd_decode(g_edge)))
	{
		kbd_decode(data);
				
	}
 
    if (!g_edge)                               // Routine entered at falling edge
    {
		MCUCR = 0x03;						 	//Rising edge for INT0
     	g_edge = 1;
    }
    else
    {                                       	// Routine entered at rising edge
		MCUCR = 0x02;					    	// Falling edge for INT0
        g_edge = 0;
    }
 
}

//---------------- UART SECTION

uint8_t USART_putc(uint8_t c)
{
    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;
    return 0;
}

uint8_t USART_getc(void)
{
    while( !(UCSRA & (1<<RXC)) );    //wait for char
    
    return  UDR;
}



// UART Receive interrupt
/*
SIGNAL (SIG_UART_RECV)
{

// Disable scanline interrupt ???
   g_rcv_buff[g_rcv_ptr] = UDR; 				// Receive buffer for UART Note UDR use both write /read depend on size on expressions
		
   if ((g_rcv_ptr >= SLOT_SIZE) || ( g_rcv_buff[g_rcv_ptr] == 0x0D)) // CANNOT USE UDR to compare
   {	
//  End with Null 
            g_rcv_ptr++;
            g_rcv_buff[g_rcv_ptr] = 0;   
			g_cmd_decode = 1;
			g_rcv_ptr = 0;
   }
   else
	   g_rcv_ptr++;				
		

}

*/

// Put string to Output
void put_str (uint8_t *str)
{
 uint8_t j;
 j = 0;
      USART_putc (0x0D);
	  USART_putc (0x0A);
      while (str[j] != 0x0)
	  {	
		USART_putc(str[j]);
		j++;
	  }
	

}



/* Command decode from g_rcv_buffer  will decode and action according to command in buffer
 
*/
void cmd_decode (void)
{

 uint8_t  cmd_no;
 uint8_t  cmd_argument[TEXT_SIZE];   // Max argument = text size

 
 
 cmd_no =  text_decode (g_rcv_buff,cmd_argument);
  
	 if ( cmd_no )   // Command > 0 
	 {
	    put_str ("Command = ");
		switch (cmd_no)
		{
			case SLOT_CMD: 
			    USART_putc(0x30+cmd_no);
				put_str(cmd_argument);
				g_num[g_row] = atoi(cmd_argument);
				g_irint = 1;
				break;
			case  TEXT_CMD:
			    USART_putc (0x30+cmd_no);
				put_str (cmd_argument);
				break;
			case  GRAPHICS_CMD:
			    USART_putc (0x30+cmd_no);
				put_str (cmd_argument);
				break;		 
			case  DISPLAY_MODE_CMD:
			    USART_putc (0x30+cmd_no);
				put_str(cmd_argument);
				break;	
			case  LINES_CMD:
			    USART_putc (0x30+cmd_no);
				put_str (cmd_argument);
				break;			
			case  NEXT_SLOT_CMD:
			    USART_putc (0x30+cmd_no);
				put_str (cmd_argument);
				break;		
			case  STYLE_CMD:
			    USART_putc (0x30+cmd_no);
				put_str (cmd_argument);
				break;			
			case  COLOR_CMD:
				USART_putc (0x30+cmd_no);
				put_str (cmd_argument);
				break;		  	
			default:
			    put_str ("Command not found ");
		        break;	  
		}		
	 }


}

/* This subroutine will handle IR remote decode by getting remote code from Interrupt handle for input capture
and will response  to each remote key. For other applications user can modified here for different remote active
However this routine support Sony remote control only
   global effect   g_irint;
*/

void remote_decode ( uint16_t ircode)
{

  uint8_t   style;
  uint8_t   decode,mode;
  
  static int8_t  slot = 0;
   decode = 1;
   switch ( ircode)
	   {
	    case IR_TV_1: 
			g_num[g_row] = 1;
			g_irint = 1;
			slot =1;
			break;
		case IR_TV_2:
		    g_num[g_row] = 2;
			g_irint = 1;
			slot =1;
			break;
	    case IR_TV_3:
			g_num[g_row] = 3;
			g_irint = 1;
			slot =1;
			break;
		case IR_TV_4:
		    g_num[g_row] = 4;
			g_irint = 1;
			slot =1;
			break;
	    case IR_TV_5:
			g_num[g_row] = 5;
			g_irint = 1;
			slot =1;
			break;
		case IR_TV_6:
		    g_num[g_row] = 6;
			g_irint = 1;
			slot =1;
			break;
	    case IR_TV_7:
			g_num[g_row] = 7;
			g_irint = 1;
			slot =1;
			break;
		case IR_TV_8:
		    g_num[g_row] = 8;
			g_irint = 1;
			slot =1;
			break;
	    case IR_TV_9:
			g_num[g_row] = 9;
			g_irint = 1;
			slot =1;
			break;
		case IR_TV_0:
		    g_num[g_row] = 0;
			g_irint = 1;
			slot =1;
			break;
		case IR_TV_CHUP:							
            g_row  = 0;                      // Upper row
			g_irint = 0;
		    IR_DELAY;  						// Delay to avoid repeat command
			break;
		case IR_TV_CHDOWN:							// Ch-
		    g_row   = 1;                        // Lower
			g_irint = 0;
		 	IR_DELAY;	  						// Delay to avoid repeat command
			break;
		case IR_TV_TVMUTE:
			g_next_slot = g_num[g_row];
			g_irint = 1;
 			break;
		case  IR_TV_TVPOWER:
		    g_display_mode = (!g_display_mode) & (0x01);
			 
 			while (!eeprom_is_ready());
			      eeprom_write_byte(&(g_control[g_num[g_row]].display_mode),g_display_mode);
		 	IR_DELAY;	  						// Delay to avoid repeat command 
			g_irint = 1;
  			break;
		case   IR_TV_TVAV:
		     g_style[0]  = LED_MATRIX_SELFTEST;
			 g_style[1]  = LED_MATRIX_SELFTEST;
       		 g_irint = 0;
			 decode = 0;
			 IR_DELAY;
	 		 break;
			
		case  IR_TV_VOLUP:                 			//VOLUMN_UP
			
			 style = eeprom_read_byte(&(g_control[g_num[g_row]].show_mode));
			 mode = style & LED_MATRIX_EEPROM_MASK;
			 style &=  0xF0;    // Clear mode before or again
			 mode++;
			 if (style & LED_MATRIX_GRAPHIC)  // If graphic cannot show vertical
			 {
			    mode &= (~LED_MATRIX_GRAPHIC);   // Mark off Graphic mode 
				if (mode > 0x07)
					style  |= 0;    // Start from Mode 1 
				else
					style  |=  mode; 
			 }
			 else
			 {
				if (mode > 0x0F)
					style  |= 0;    // Start from Mode 1 
				else
					style  |=  mode;
			}	
 			g_style[g_row] = style;
 			 while (!eeprom_is_ready());
			      eeprom_write_byte(&(g_control[g_num[g_row]].show_mode),style);
  			IR_DELAY;  						// Delay to avoid repeat command
			g_irint = 1;
			
			break;
		case  IR_TV_VOLDOWN:                 			//VOLUMN_DOWN
			 style = eeprom_read_byte(&(g_control[g_num[g_row]].show_mode));
 // Extract 4 bit
			 mode = style &  LED_MATRIX_EEPROM_MASK;
 		    style &=  0xF0;    // Clear style mode 
  		     mode--;
			  if (style & LED_MATRIX_GRAPHIC)  // If graphic cannot show vertical
			 {
				if (mode < 1)
					style |= 0x07;
				else 
					style  |= mode;		
			 }
			 else
			 {
				if (mode < 1)
					style |= 0x0F;
				else 
					style  |= mode;		
             } 
	 
            g_style[g_row] = style;
			while (!eeprom_is_ready());
			      eeprom_write_byte(&(g_control[g_num[g_row]].show_mode),style);
 			IR_DELAY; 						// Delay to avoid repeat command
			g_irint = 1;
			break;
		default:
			g_irint = 0;
            decode = 0;
			break;
 	   }
	   if (decode)
	   {
		while (!eeprom_is_ready());
			eeprom_write_byte(&show_msg[g_row],g_num[g_row]);
// If press mute mean wnat to change next slot, next need to press next slot 	
		if ((slot) && (g_next_slot >= 0)) 
		{
			while (!eeprom_is_ready());
			eeprom_write_byte(&(g_control[g_next_slot].next_slot),g_num[g_row]);
			g_next_slot = -1;
			slot = 0;
		}
       } 
 
}


 

/*Interrupt handler for TIMER0 using for scanrate 
   clk = clk/1024 
*/
  
SIGNAL(SIG_OVERFLOW0)
{
   static uint8_t k = 0;
   static uint8_t j = 0;


// Load counter then count = 6 
  TCNT0 = 252;
// Disable Interrupt
  	TIMSK  = TIMSK & 0xFE;
  
  led_matrix_scanline(j,k);
  k++;
  if ( k == LED_MATRIX_ROW_HIGH) 
  {
       k = 0;
	   j++;
	   if ( j == LED_MATRIX_ROW_NUM)
				j = 0;
  }	   
// Enable timer0 Interrupt
	TIMSK  = TIMSK | ( 1<< TOIE0);


}
 
/* Interrupt handle for Input Capture
This routine will detect/ process the Sony IR remote command. Protocol using Sony remote control
INPUT   :  Infrared signal from  ICP pin
		   Command table 
		  

OUTPUT 	: g_num  Number of message to display
		: g_control.display_mode   Display mode of message
		: g_start_bit
		

*/
SIGNAL(SIG_INPUT_CAPTURE1)
{
	uint16_t  ir_code;
    
 
	TCCR1B = 0xC2;   // Change to rising_edge trigger;
     cli();  
	ir_code  = sony_ir_decode(ICR1);
	if (ir_code)
	{       //End code dispatch command    
		remote_decode (ir_code);
//Note if  SELFTEST is selected g_irint = 0;
	}   
	sei();
 
}


// Application API these API called by application and call system API to perform task

//  Application part for the Mypanel Message board

// Remote decode for mypanel message board 



int main( void )
{
int16_t  length[LED_MATRIX_ROW_NUM],i,loop;
uint8_t   start[LED_MATRIX_ROW_NUM] = {LED_MATRIX_STILL,LED_MATRIX_STILL};   // To show start of move algorithm
uint8_t   con = 0,change = 0,style,display_style,j;
int8_t    finish,next_slot[LED_MATRIX_ROW_NUM];

	i = 0;
    finish  = 0;
	g_rcv_ptr   = 0;
	g_cmd_decode = 0;
	
    init_board();
    led_matrix_init_board();

// Get message and style from eeprom 1st time

	for (i = 0; i < LED_MATRIX_ROW_NUM ; i++)
 	{
	    g_num[i] = eeprom_read_byte(&show_msg[i]);
// Mark upper 4 bit off ( Not display mode )
	   g_style[i] = eeprom_read_byte(&(g_control[g_num[i]].show_mode));   
	   g_style[i] &= LED_MATRIX_SHOW_MASK;
	   next_slot[i] = eeprom_read_byte(&(g_control[g_num[i]].next_slot));
       length[i] = eeprom2ram(i,g_num[i]);
 	}

	
// Initialize interrupt 
	MCUCSR 	= 0x02;                      // INT0 interrupt on falling edge
    GICR 	= 0x40;                      	// enable external int0
	TCCR1B 	= TIMER1_CLOCK;				// Time 1 input to clock/8 input capture on falling edge  noise cancel
	TIMSK  	= 1<<TICIE1;   				// Enable input capture enable 

// Enable interrupt for TIMER0 for scanline
    TCCR0  = 0x05;						// Clock/1024 for time0
	TIMSK  = TIMSK | ( 1<< TOIE0);
// Initialize UART  
    
    UCSRC  = 0x86;    // 8 bit non parity  1 stop bit
    UCSRB  = 0x98;    // Enable RX Interrupt and Enable TX, RX pin
	UBRRL  = 0x67;    // Speed 9600 UBRR = 103 at 16 MHZ
	UBRRH  = 0;
 	sei();                 			    // Enable global interrupts 
   
   

while(1) 
{
 
	
// If kbd interupt stop the show message enter input mode
//------------------------------   Keyboard --------------------------------------
 
	if (g_kbdint)
	{
// Disable TImer
       	TIMSK  = TIMSK & 0xFE;   
// Copy from keyboard buffer to video ram
        length[0]   =  kbdbuff2ram (0);
        length[1]   =  kbdbuff2ram (1);

// Set Style to onerow
        g_display_mode = LED_MATRIX_ONEROW;
// Select Algorithm to show	
	    g_style[0] = LED_MATRIX_KBD_EDIT;
		g_style[1] = LED_MATRIX_KBD_EDIT;
		g_next_slot = -1;
 	    change = 1;    //Edit mode
// Enable timer0 Interrupt
		TIMSK  = TIMSK | ( 1<< TOIE0);
   	}
	
	else  // If first exit from EDIT mode by pressing  ESC key g_kbdint had been cleared
	{
	  if (change == 1)
	  {
	  // Disable TImer
       	TIMSK  = TIMSK & 0xFE;  
		for (i = 0; i < LED_MATRIX_ROW_NUM ; i++)  // Recover old state
		{
			g_style[i] = eeprom_read_byte(&(g_control[g_num[i]].show_mode));
// Mark upper 4 bit off ( Not display mode )
			g_style[i] &= LED_MATRIX_STYLE_MODE_MASK;    // MO  No selftest,graphic, vertical
			next_slot[i] = eeprom_read_byte(&(g_control[g_num[i]].next_slot));
 		    length[i] = eeprom2ram(i,g_num[i]);            
 		}
// Restore g_display_mode		
		g_display_mode = eeprom_read_byte(&(g_control[g_num[g_row]].display_mode));
		change = 0;
// Enable timer0 Interrupt
    	TIMSK  = TIMSK | ( 1<< TOIE0);

 	  } 
	}

 
//---------------------------------- Light Pen --------------------------------------
 // Check light pen button
  
 	if ((!(E_IN_PORTD & E_PORT3)) || con)       // PRESS BUTTON
	{
//Clear the dot after Message
// Disable scan rate timer0
	TIMSK  = TIMSK & 0xFE;
            if (!con)    // Enter first time
			{ 
// Show the signal line    MO
// Need to clear interrupt before reading from eeprom
// Check whether it is graphic or text 
	             i = g_num[g_row];
                if ( g_style[g_row] & LED_MATRIX_GRAPHIC)
				{   // GRAPHIC  COPY DIRECTLY
					led_matrix_put_graphic_video_ram (g_row,&g_msg[i][0]);
 		     	}
				else
				{
// Translate text to graphic and put in g_buffer  
				led_matrix_text2graphic (g_row,&g_msg[i][0]);
 				}
				length[g_row] = g_display_row-1;
				g_style[g_row] = LED_MATRIX_STILL | LED_MATRIX_GRAPHIC;

 
			}
			con = light_pen(g_row);
// Enable scan rate timer0
	TIMSK  = TIMSK | ( 1<< TOIE0);
 			
 	}	
 

//------------------------------ Remote control ---------------------------
// If just pass infrared interrupt or keyboard UP/Down change the message and style again
// Should change only when new data is selected  ??
 
	if (g_irint)
	{  
// Exit mode from light pen
			con = 0;
	        cli();
			g_display_mode = eeprom_read_byte(&(g_control[g_num[g_row]].display_mode));
			if (g_display_mode != LED_MATRIX_ONEROW)    // Normal Tworows 
			{
//	CHK			for (i = 0; i < LED_MATRIX_ROW_NUM ; i++)
//				{
	       		    g_style[g_row] = eeprom_read_byte(&(g_control[g_num[g_row]].show_mode));
// Mark upper 4 bit off ( Not display mode)
					g_style[g_row] &= LED_MATRIX_SHOW_MASK;   // Include self test
 					next_slot[g_row] = eeprom_read_byte(&(g_control[g_num[g_row]].next_slot));
					length[g_row] = eeprom2ram(g_row,g_num[g_row]);
 //				}
 			}
			else  //ONE_ROW
			{
// Select Algorithm to show
			   g_style[0] = eeprom_read_byte(&(g_control[g_num[g_row]].show_mode));
// Mark upper 4 bit off ( Not display mode)
			   g_style[0]  &= 0x1F;    // MO
			   next_slot[0] = eeprom_read_byte(&(g_control[g_num[0]].next_slot));
               next_slot[1] = next_slot[0];
			   g_style[1]   = g_style[0];
			   start[0]  = 	LED_MATRIX_STILL;
			   start[1]  = 	LED_MATRIX_STILL;
			   length[0] =	eeprom2ram(0,g_num[g_row]);
			   length[1] =	eeprom2ram(1,g_num[g_row]);
			   length[1] = 	length[0];
 // Shift data to buffer 
 			}
			g_irint = 0;  			// Clear irint flag
			finish  = 0;
            sei();    
	}
 

// Scan column for 3 color		
// For each color for one loop
// Delay for running text
   for ( loop = 0 ; loop < LED_MATRIX_MOVE_DELAY ; loop++)
	{
	   for (j = 0 ; j < LED_MATRIX_ROW_NUM; j++) //  Because scanline start from 0 then calculate backward for time
                                                //   to calculate
		{
// Put data into buffer	
            style =  g_style[j] & LED_MATRIX_STYLE_MASK;   // Mark vertical / Graphic off vertical & Graphic handle in display style sub
			if ( g_display_mode == LED_MATRIX_ONEROW)
				display_style = g_style[g_row];
			else
				display_style = g_style[j];
			 
  			switch (style)
			{ 
 				case LED_MATRIX_KBD_EDIT:
 				     start[j] = LED_MATRIX_KBD_EDIT;
// keyboard edit mode length always = g_buffcnt*CHAR_SIZE (variable)
 				     led_matrix_kbd_still(j,g_buffcnt*LED_MATRIX_DEFAULT_CHAR_SIZE,g_display,g_cursor,display_style,g_display_mode);  
 					 break;
				case LED_MATRIX_MOVEUP:
					 finish =  led_matrix_moveD2U(j,length[j],start[j],loop,display_style,g_display_mode);
					 start[j] = LED_MATRIX_MOVEUP;
  					 break;
 				case LED_MATRIX_MOVEDOWN:
					 finish = led_matrix_moveU2D(j,length[j],start[j],loop,display_style,g_display_mode);
					 start[j] = LED_MATRIX_MOVEDOWN;
  					 break;
			 	case LED_MATRIX_MOVELEFT:
  					 finish = led_matrix_moveR2L(j,length[j],start[j],loop,display_style,g_display_mode);
					 start[j] = LED_MATRIX_MOVELEFT;
  					 break;
				case LED_MATRIX_MOVERIGHT:
  					finish = led_matrix_moveL2R(j,length[j],start[j],loop,display_style,g_display_mode);
  					start[j] = LED_MATRIX_MOVERIGHT;
 					break;
				case LED_MATRIX_STILL:
					if ( length[j] < g_display_row)
					{
						finish = led_matrix_still(j,length[j],loop,display_style,g_display_mode);
						start[j] = LED_MATRIX_STILL;
					}
					else
					{
						finish = led_matrix_move_still(j,length[j],start[j],loop,display_style,g_display_mode);
						start[j] = LED_MATRIX_STILL;
					}
   					break;
				case LED_MATRIX_CENTER:
				    led_matrix_center(j,length[j],display_style,g_display_mode);
					start[j] = LED_MATRIX_CENTER;
					break;
   				case LED_MATRIX_SELFTEST:
				    led_matrix_selftest(j);
					start[j] = LED_MATRIX_SELFTEST;
					finish = 0;
					break;
 
 				default:
					start[j] = LED_MATRIX_STILL;
					finish =  led_matrix_still(j,length[j],loop,display_style,g_display_mode); 
				 	break;
			} 
  			 
			g_sync_flag = j;
			if (finish )  // Show algorithm Finished 			
			{
// Change to next slot message
			  if (next_slot[j] != g_num[j])
			  {
				  g_num[j] = next_slot[j];
				  g_irint = 1;  
			  }
			}
	
		}

	}
// If command decode status is set   decode the command
/*
   if (g_cmd_decode)
   {
      put_str(g_rcv_buff);
	  cmd_decode();
      g_cmd_decode = 0;

   }
 */  

  }
}

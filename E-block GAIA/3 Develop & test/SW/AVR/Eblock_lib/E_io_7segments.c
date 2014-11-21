/*!  
	\file   	E_io_7segments.c
	\brief  	Library module for 7 Segments 
	\ingroup	E_io_7segments  ( Name refer in *.h  group)
	\par  Module description
    \n	Driver for  two of 7 Segments on E_IO Module. 
	\n	Show output to 7 Segments both for characters and text mode.
	\n	Including scroll text for long message. 	
	\n Original written for 	:   \b AVR
 	\n CPU port/tested		:   ATMEGA32
 	\n Hardware use			:   E_IO
 	\n Port use				:   E_PA0 - E_PA7, E_PB1, E_PB2
 	\n Memory	FLASH		:   XXX
 	\n		RAM				:   XXX
 	\n		EEPROM			:   XXX
 	\n Reference Document	:   Document describe algorithm

	\version 0.1
 	\author  wichai@geartronics.net
	\date 	 12/08/08 

	\par Update History
 	\nDate			By 					Comments
 	\n----			--					--------
	\n 
 
*/






//Include file


#include <avr/pgmspace.h>
#include "E_io_7segments.h"
#include "avr_timer.h"


//! String buffer to display
uint8_t g_display_str[MAX_STR] = {"0"};  // Need to initialize
//!  Length of string in buffer 
uint8_t g_display_length;

//! Pointer to start of display buffer
uint8_t g_start_ptr;   
//! Flag there is display buffer change
uint8_t g_display_change;    



//! Fonts save to show on 7 Segments 

const char font[FONT_NUM]  PROGMEM =  {
//  From SPACE 0x32  to ` 0x60
 0x00,0x82,0x22,0x36,0x65,0x24,0x52,0x02, //32 - 39
 0x39,0x0F,0x63,0x46,0x02,0x40,0x80,0x52, //40 - 47
 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07, //48 - 55
 0x7F,0x6F,0x48,0x18,0x63,0x48,0x43,0x53, //56 - 63
 0x5F,0x77,0x7C,0x39,0x5e,0x79,0x71,0x3d, //64 - 71
 0x76,0x06,0x1e,0xf2,0x38,0x55,0x54,0x3f, //72 - 79
 0x73,0x67,0x31,0x6d,0x78,0x3e,0x1c,0x6a, //80 - 87
 0x49,0x6e,0x52,0x39,0x24,0x0f,0x23,0x08, //88 - 95
 0x20};

// Start routine here

 



/*!	\brief  Init  7 segments module  for output port and initial timer for refresh rate 
	\n      ( Use the same timer for io_sw module)
	\param  None  
	\return None
	\par Port/Periheral Usage 
	\n   E_PA0 - E_PA7, E_PB1, E_PB2
	\par Theory of Operation   
	\n   Each I/O pins need to set direction for use as input or output
	\par Pseudo code
	\n   Initial all usage GPIO to output direction
	\n 	 Initial display length to show on 7 Segments = 1 
	\note    
	\n  
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
void io_7segments_init ( void) 
{

// Set output for E_PA0 - E_PA7, E_PB0, E_PB2
    E_DIR_PA0(OUTPUT);
	E_DIR_PA1(OUTPUT);
	E_DIR_PA2(OUTPUT);
	E_DIR_PA3(OUTPUT);
	E_DIR_PA4(OUTPUT);
	E_DIR_PA5(OUTPUT);
	E_DIR_PA6(OUTPUT);
	E_DIR_PA7(OUTPUT);
	E_DIR_PB1(OUTPUT);
	E_DIR_PB2(OUTPUT);
 	
// Timer initialize
   timer_tick (100);    // Set timer tick for display update
	
	g_display_length = 1 ;
  	g_start_ptr  = 0;
	g_display_change = 0;
	
		 
}


/*!	\brief  Init  7 segments module  for output port and initial timer for refresh rate 
	\n      ( Use the same timer for io_sw module)
	\param  None  
	\return None
	\par Port/Periheral Usage 
	\n   E_PB0,E_PB3
	\par Theory of Operation   
	\n
	\par Psudo code
	\n
	\note   (Pseudo code)
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     

void io_7Segments_num (float  number, int decimal_point) 
{
 
// Convert float to string
	
}


/*!	\brief  Copy display string to display buffer 
	\param  str 	Input Pointer to display string  
	\n		 
	\return None
	\par Port/Periheral Usage 
	\n    
	\par Theory of Operation   
	\n   Copy display string from external  to internal display buffer for 7 Segments 
	\n   This routine will convert all lower case to uppercase and set characters which is not 
	\n   in range 32 - END_SUPPORT_ASCII (96) to  SPACE characters   
	\par Pseudo code
	\n 		Copy from external string to internal display buffer until NULL Character found
	\n         If lower case convert to uppercase
	\n		   If out of support range map to BLANK
	\n		   save the length of string to display_length
	\n
	\note   Display buffer set at constant  MAX_STR = 25  
	\bug    If characters more than 128 will get font index overflow.

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
void io_7segments_write (uint8_t *str)  	
{
   uint8_t i = 0;
   int8_t ch;
   g_display_length = 0;
   while ( *(str+i))
   {  
        ch =  *(str+i);
		if  (ch > END_SUPPORT_ASCII)    // Over support fonts limit
			ch -= FONT_OFFSET;			// Convert to small cap
		ch -= FONT_OFFSET;				// Convert to font index
		if (ch < 0)					    //  < 0 ASCII map to NULL (Show blank)
		    ch = 0;
		g_display_str[i] = ch;
		i++;
   };
   g_display_length = i;                  // Length of char
   g_display_str[i] = 0; 				  // NULL Characters end of string
   g_start_ptr = 0;
   g_display_change = 1;				 // Display buffer change refresh display
}


/*!	\brief  Update 7 Segments modules display 
	\param  
	\n	Global  display_str 		- String to display
	\n								  (String of characters which already map to range 32 - 65)
	\n			display_length		- String length
	\return None
	\par Port/Periheral Usage 
	\n   E_PA0 - E_PA7, E_PB1, E_PB2
	\par Theory of Operation   
	\n   Because 2 digits share the same Digit segments. Need to multiplex between each characters. 
	\n   Update display on 7 Segments with this algorithm
	\n		   1 Char show leftmost   (Easy to implement)
	\n		   2 Chars show 2 digits
	\n		  > 2 Chars scroll left and re-start when finished
	\n   To make the refresh process dependent from other tasks. User timer tick to update the display.
	\par Pseudo code
	\n		Check scrolling time  if reach  move to next characters  start from  begin of buffer 
	\n		 until end
	\n      If DOT found  Turn on the dot in previous characters and skip to show next characters
	\n		Toggle to show each digit using the timer_tick 
	\note    
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
void io_7segments_update (void)  	
{
 
  static uint8_t fontmap[MAX_CHAR];
  static uint16_t time_shift =  0;   	// Scrolling time interval
  static uint8_t toggle;      			// Toggle display

		  uint8_t index = 0;
 		time_shift++;
		if ((time_shift > INTERVAL) || (g_display_change)) // Interval to shift digits or display buffer change
		{
			time_shift = 0;
			g_display_change = 0;      // Disable flag until new buffer fetch
		 	index 		= g_display_str[g_start_ptr];
			fontmap[0]  = pgm_read_byte(&font[index]); 	
			index = g_display_str[g_start_ptr+1]; 
 			if (index == DOT)   // Found dot 
			{
 				fontmap[0]  |= _7SEGMENT_DOT;
 				g_start_ptr++;    // Move to next char 
  				index = g_display_str[g_start_ptr+1]; 
 			} 
			fontmap[1]  = pgm_read_byte(&font[index]);	
			g_start_ptr++;	
		 	if (g_start_ptr > g_display_length - 2) 	// Index start from 0 and show 2characers each time 
				g_start_ptr = 0;					// Reset to start of text again
        }
	
		if (toggle) 			//  toggle display 2 digits
		{
			_7SEGMENT_CHAR   =   fontmap[1];
 			_7SEGMENT_DIGIT1_OFF;
 			_7SEGMENT_DIGIT2_ON;
			toggle = 0;
		}	
		else // Digit 1 rightmost
		{
		    _7SEGMENT_CHAR   =  fontmap[0];
			_7SEGMENT_DIGIT1_ON;
 			_7SEGMENT_DIGIT2_OFF;
			toggle = 1;
		}
 
			
} 
  

 

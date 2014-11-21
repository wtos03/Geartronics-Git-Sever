/*!  
	\file   	E_io_sw.c
	\brief  	Library module for toggle Switch on E_IO Module
	\ingroup	E_io_sw
	\par Module description
 	\n Original written for 	:   \b AVR
 	\n CPU port/tested			:   ATMEGA32
 	\n Hardware use				:   E_IO
 	\n Port use					:   E_PB0, E_PB3
 	\n Memory	FLASH			:   XXX
 	\n			RAM				:   XXX
 	\n		EEPROM				:   -
 	\n Reference Document		:   Document describe algorithm

	\version 0.1
 	\author  wichai tossamartvorakul /Wichai@geartronics.net
	\date 	 13/08/08

	\par Update History
 	\n	Date			By 					Comments
 	\n	----			--					--------
	\n 
 
*/






// Include file
#include "cpu.h"
#include "E_delay.h"
#include "E_io_sw.h"






// Start Subroutine here


 



/*!	\brief  Initial toggle SW on E_IO block 
	\param  None  
	\return None
 	\par Port/Periheral Usage 
	\n   E_PB0,E_PB3
	\par Theory of Operation   
	\n	 Initial Port for Input and enable pull up resistor by setting
	\n	 Direction register and Output 1 to port to enable pull up
	\par Pseudo code
	\n	 Inital port to use for output
	\n   Enable pull up resistor 
	\note  
	\n	 Use Macro   E_IN_XXX to get input
	\n   Use Macro   E_OUT_XXX (y) to get output
	\n   Use Macro   E_DIR_XXX  to set direction
	\n    XXX is  E_BLOCK PORT NAME  Y  = ON, OFF 
	       
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/           
void  io_sw_init (void) 
{
// Set direction of two switch to Input	
	E_DIR_PB0 (INPUT);
	E_DIR_PB3(INPUT);
// Enable pull up resistor 
	E_OUT_PB0(ON);
	E_OUT_PB3(ON);
}




/*!	\brief  Get status of  toggle SW on E_IO block 
	\param  Toggle Switch to check 1 or 2  
	\return   = 0 No press
	\n		  > 1 Time to  press &hold the switch max 65530
	\par Port/Periheral Usage 
	\n   E_PB0,E_PB3
	\par Theory of Operation   
	\n   Get the SW status by reading PIN register. If hold the key the press value  will be
	\n   cumulative until MAX VALUE of int16 is reach then stop increment
	\n   To check sw hold use number of loop to dertermine whether the switch 
	\n	is pressed or hold
	\par Pseudo Code
	\note   To correct debounce by allowing return value more than debouce interval ex 10 - 100
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/       
uint16_t  io_sw_read (uint8_t sw) 
{
   static  uint16_t  press[MAX_KEY] = {0,0};  // Keep status for each key
   uint8_t state = 1;
  
   
   sw--; 					// Start from 0  
   delay(100);			   // Debounce key
   
   if (sw == 0 )
   {
		state = E_IN_PB0;
   }
   if (sw == 1 )
   {
		state = E_IN_PB3;
   }
   if (!state) 			// If Press = 0  (Because enable pull up resister)
	{
		press[sw]++;
		if (press[sw] > 65530)
			press[sw] = 65530;   // MAX VALUE
	}
	else   //  Release or never press 
	{
		press[sw] = 0;
	}
 	return press[sw];
}




/*!  
	\file   	E_delay.c
	\brief  	Delay loop 
	\ingroup	E_delay 
	\par  Delay loop for use in E-block lib
        \n Start with tab +\n	
 	\n Original written for 	:   \b for bold CPU/Microcontroller
 	\n CPU port/tested		:   CPU/Microcontroller
 	\n Hardware use			:   IC Chip
 	\n Port use			:   Port name / Function
 	\n Memory	FLASH		:   XXX
 	\n		RAM		:   XXX
 	\n		EEPROM		:   XXX
 	\n Reference Document		:   Document describe algorithm

	\version xxx.yyy
 	\author  Name / Email
	\date 	 DD/MM/20XX  

	\par Update History
 	\nDate			By 					Comments
 	\n----			--					--------
	\n 
 
*/






//! Include file
#include <avr/io.h>






// Start routine here

 


/*!	\brief  Initial toggle SW on E_IO block 
	\param  None  
	\return None
	\par Port/Periheral Usage 
	\n   E_PB0,E_PB3
	\par Theory of Operation   
	\n
	\par Psedud code
	\n
	\note   (Pseudo code)
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
void delay ( uint16_t j )
{
     uint16_t i;
	 for ( i = 0; i < j ; i++);

}
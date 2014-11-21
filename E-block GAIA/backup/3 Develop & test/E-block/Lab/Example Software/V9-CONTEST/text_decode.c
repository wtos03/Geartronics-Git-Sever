/* Text command decode 
    

Bug Found :  need to have one space at the end of line, If not it will not proceed the last command
	
*/
#include <stdlib.h>
#include <string.h>
#include "text_decode.h"



/* Command decode from g_rcv_buffer will decode and action according to command in buffer
Delimeter  {=, SPACE,  ", Comma, Carriage return, Tab, Line Feed}

FORMAT  =  {Cmd =  Argument} Seperate command with Delimiter
		   End line with CR, LF
		   Text " " will be ignores for white space etc. until pair of " found 
		   


*/


// ENd with NULL to end of checking
const uint8_t text_cmd[CMD_NUM][CMD_LENGTH] PROGMEM  = {"motor","speed","angle","program","save","run", ""}; 

const uint8_t delimiter[] PROGMEM = {0x3D,0x22,0x20,0x09,0x2C,0x0D,0x0A,0x00};  
//const uint8_t white_space[] PROGMEM = {0x20,0x09,0x00}; // TAke this out got problems ??????

/*  
Decode text to command number and command arguments. Can called repeatly until end of buffer found 
(Return 0xFF)
  INPUT : Pointer start buffer to decode
        : Pointer of command argument
  OUTPUT: cmd number  0 = not found 
	    : Pointer to command argument in buffer
		: Pointer for next char to decode in buffer
Will read data in buffer and put the argumnet in start of buffer



*/ 

uint8_t text_decode (uint8_t *buffer, uint8_t *cmd_argument)
{

 uint8_t  found,ch,ch2,cmd_no,j,k,index,cmd_complete;
 uint8_t  state = WAIT_CMD_STATE, quote  = 0;
 static uint16_t i=0; // Keep pointer for next command decode
 cmd_complete = 0;
 cmd_no 	= 0;
 index 		= 0;
 
 cmd_argument  = buffer;
 
 while (*(buffer+i))	  		//End with NULL
 {
// Check with delimiter

 	 j = 0;
	 found = 0;
	
	switch (state)
	{
	  case WAIT_CMD_STATE:   // If found first character in command table  then go to get_cmd state
 		j = 0;
		while ((ch = pgm_read_byte(&text_cmd[j][0])))     //Check first charaters in cmd table
		{ 
		  if (*(buffer+i) == ch)
		  {
			state = GET_CMD_STATE;   // Found delimiter
			*(cmd_argument) = ch;
			index = 1;
			break; 
		  }
		 j++;
		}  
	    break;
	  case GET_CMD_STATE:
	    ch = *(buffer+i);
        j = 0; 
		found = 0;
	  	while ((ch2 = pgm_read_byte(&delimiter[j])))   //Check delimiter End with NULL
		{  
		   if ( ch == ch2)
		   {
              found = 1;
              break;
		   } 	 
           j++;	
		}
		if (found)
		{
        	*(cmd_argument+index) = 0x00;
		     if (ch == '=')
			 {
// Check command number 
  				for (k = 0; k < CMD_NUM ; k++)
				{
					if (!(strcasecmp_P(cmd_argument,&text_cmd[k][0])))  // Found then keep command
					{
						cmd_no  = k+1;   // Start from 0 
						break;
					}
				}
// clear index
  				index = 0;
			    state = WAIT_ARG_STATE;	
			 }	
		}
		else
		{
			*(cmd_argument+index) = ch;
			index++;		
		}
		break;
		
	  case WAIT_ARG_STATE:
		ch = *(buffer+i);
        j = 0; 
		found = 0;
	  	while ((ch2 = pgm_read_byte(&delimiter[j])))   //Check delimiter End with NULL
		{  
		   if ( ch == ch2)
		   {
              found = 1;
              break;
		   } 	 
           j++;	
		}
				
		if ( ch == '"')   // Start quote
		{
			quote ^= 1;   // Toggle quote	
			state = GET_ARG_STATE;   
 			index = 0;
		}
		
		if (!found)    // Not delimiter start get Argument
		{
			state = GET_ARG_STATE;   
			*(cmd_argument) = ch;
			index = 1;
		}
		break;	
	  case	GET_ARG_STATE:
	  	ch = *(buffer+i);
        j = 0; 
		found = 0;
	  	while ((ch2 = pgm_read_byte(&delimiter[j])))   //Check delimiter End with NULL
		{  
		   if ( ch == ch2)
		   {
              found = 1;
              break;
		   } 	 
           j++;	
		}
 		if ( ch == '"')     // End quote
		{
			quote ^= 1;   // Toggle quote	
   		}
		if ( quote)
		{
         	found = 0; //  ignore all characters after the quote until found quote again
// Cancel quote
            
		}	
		if (found)
		{	
            cmd_complete = 1;
			*(cmd_argument+index) = 0;
		    state = WAIT_CMD_STATE;
		}
		else
		{
			*(cmd_argument+index) = ch;
			index++;		
		}  
		break;
	}
	
	if(cmd_complete)   // COmmand both cmd number and argument
	{
		return cmd_no;	 	 
	}
	i++;
 }
  i = 0;
  return 0xFF;  // End of Buffer

}

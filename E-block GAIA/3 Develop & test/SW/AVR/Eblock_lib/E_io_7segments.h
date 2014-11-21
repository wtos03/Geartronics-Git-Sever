/*!   \file  	E_io_7segments	 
      \brief    Library for 7 Segments on E_IO block
      \defgroup E_io_7segments
      \ingroup  Eblock_lib
	\n Summary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable				X   
	\n Function prototype				-
	\n Variable (Keep in Flash Program memory)	-
	\n Memory usage					?? bytes
	\n Update History
	\n Used by module                         	xxxxx
     \code #include <E_io_7segments.h> \endcode
 
      \version	0.1
      \author 	wichai tossamartvorakul wichai@geartronics.net
      \date	   13/08/08

      \n \b Update \b History
 
	\n Date                 By                     Comments
	\n ----                 --                     ---------
        \n
 
 
	\note   To use this library need to add one interrupt service routint in main program module
   
			SIGNAL(SIG_OUTPUT_COMPARE0)
			{
				io_7segments_update ();    //Update display <-- Put this routine to refresh display
			}


*/
 
 

/* External include file

*/
#include "cpu.h"


//! Constant variable, For comment in doxygen before the variable declaration ???? 

//! Variable in Flash
//!  For comment in doxygen before the variable declaration ???? 


//! Function prototypes

void io_7segments_init (void);
void io_7segments_write (uint8_t * );
void io_7segments_update (void);
 
#define  FONT_NUM    	65 
#define  MAX_STR 		25 	 // MAX CHARACTERS DISPLAY ON 7 SEGMENTS
#define  MAX_CHAR		2	 // MAX CHARACTER TO DISPLAY
#define	 FONT_OFFSET 	32   // Start from 32 SPACE 
#define  END_SUPPORT_ASCII  96  // Support font until 96
#define  INTERVAL		100  // Shift digit interval
#define  DOT			14

#define  _7SEGMENT_DIGIT1_ON 	E_OUT_PB1(0)
#define  _7SEGMENT_DIGIT2_ON 	E_OUT_PB2(0)
#define  _7SEGMENT_DIGIT1_OFF 	E_OUT_PB1(1)
#define  _7SEGMENT_DIGIT2_OFF 	E_OUT_PB2(1)
#define  _7SEGMENT_DOT			0x80

#define  _7SEGMENT_CHAR      	E_OUT_PORTA

 
 


/*!   \file  	E_io_sw.h			 
      \brief    Include file  E_io_sw module
	\n			< module description>
	\defgroup  E_io_sw  Toggle SW on E_IO Module
	\ingroup   Eblock_lib 	// If belong to other group
	\n Summary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable				X   
	\n Function prototype				-
	\n Variable (Keep in Flash Program memory)	-
	\n Memory usage					?? bytes
	\n Update History
	\n Used by module                   xxxxx
	\n  
	\code #include <E_io_sw.h> \endcode
	\version	0.1
	\author 	wichai@geartronics.net
	\date	<latest update date>
	\n \b Update \b History
 
	\n Date                 By                     Comments
	\n ----                 --                     ---------
	\n
 
 
	\note   
 
*/

 



//! Constant variable
#define MAX_KEY 	2   // MAX  SW on E_IO board

// Port use for Press SW


//! Variable in Flash



//! Function prototypes
void  	 io_sw_init   (void);
uint16_t  io_sw_read ( uint8_t );




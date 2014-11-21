/*!   \file  	E_io_piezo 
      \brief    Library for Piezo driver on E_IO block
      \defgroup E_io_piezo
      \ingroup  Eblock_lib
	\n Summary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable				X   
 	\n Variable (Keep in Flash Program memory)	-
	\n Memory usage					?? bytes
	\n Update History
	\n Used by module                         	xxxxx
     \code #include <E_io_piezo.h> \endcode
 
      \version	0.1
      \author 	wichai tossamartvorakul wichai@geartronics.net
      \date	   13/08/08

      \n \b Update \b History
 
	\n Date                 By                     Comments
	\n ----                 --                     ---------
        \n
 
 
	\note    

*/
 



/* External include file

*/
#include "cpu.h"

//! Constant variable Calculat from  Freq = Fclk/ (2*N*(1+(Value)   N =256 Prescale
#define C  118   	// 261.63 hz
#define D  105   	// 293.67 hz
#define E  94    	// 329.63 hz
#define F  88    	// 349.23 hz
#define G  79    	// 392.00 hz
#define A  70    	// 440.00 hz  
#define B  62    	// 493.88 hz
#define C5  59    	// 523.25 hz
#define BUZZER  5  	//  4 Khz 
#define STOP 0


//! Variable in Flash



//! Function prototypes
void io_piezo_init (void);
void io_piezo_write ( uint8_t);



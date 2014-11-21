/*!   \file  	avr_timer 
      \brief    Timer for avr on E_MEGA32
      \defgroup avr_timer
      \ingroup  Eblock_lib
	\n Summary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable				X   
	\n Function prototype				-
	\n Variable (Keep in Flash Program memory)	-
	\n Memory usage					?? bytes
	\n Update History
	\n Used by module                         	xxxxx
     \code #include <avr_timer.h> \endcode
 
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

//!TIMER NUMBER
#define TIMER0  0
#define TIMER1A 1
#define TIMER1B 2
#define TIMER2  3



//! Constant variable, For comment in doxygen before the variable declaration ???? 
// !   TIMER MODE0 and TIMER MODE2 Can use the same constant bit. Because it's the same.
#define  NORMAL_MODE    0
#define  PWM_MODE     	_BV(WGM00)
#define  CTC_MODE	  	_BV(WGM01)
#define  FAST_PWM_MODE	_BV(WGM00) | _BV(WGM01)
#define  ICP_RISING	    _BV(ICNC1) | _BV(ICES1)
#define  ICP_FALLING	_BV(ICNC1) 



//! OUTPUT MODE  This cannot use with TIMER1  
#define OC_DISCONNECT  	0
#define TOGGLE_OC		_BV(COM00)    // Reserved for PWM and FAST_PWM mode
									  //because PWM don't need toggle it's toggle using PWM algorithm
#define CLEAR_OC		_BV(COM01)
#define SET_OC			_BV(COM00) | _BV(COM01)

//!Clock source
#define  STOPPED_CLOCK  	0
#define  CLK_BY_1			_BV(CS00)
#define  CLK_BY_8			_BV(CS01)
#define  CLK_BY_64			_BV(CS00) | _BV(CS01)
#define  CLK_BY_256			_BV(CS02)  
#define  CLK_BY_1024		_BV(CS00) | _BV(CS02)
#define  EXT_CLK_FALLING	_BV(CS01) | _BV(CS02)
#define  EXT_CLK_RISING		_BV(CS00) | _BV(CS01) | _BV(CS02) 





//! Variable in Flash
//!  For comment in doxygen before the variable declaration ???? 


/*! Function prototypes for CPU routine we will not add CPU name in front of routine name. To make it
easy to portable to other CPU in the future. But the io hardware need to put name in front to avoid
Hardware conflict. 
*/
//! Set timer mode (TIMER 0, 2 support)
void set_timer_mode (uint8_t,uint8_t ,uint8_t , uint8_t);
//! Set timer tick for 7 segments display
void timer_tick (uint8_t);



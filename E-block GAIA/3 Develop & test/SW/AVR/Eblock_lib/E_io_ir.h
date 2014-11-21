/*!   \file  	E_io_ir
      \brief    Library for IR on E_IO block
      \defgroup E_io_ir
      \ingroup  Eblock_lib
	\n Summary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable				X   
	\n Function prototype				-
	\n Variable (Keep in Flash Program memory)	-
	\n Memory usage					?? bytes
    \code #include <E_io_7segments.h> \endcode
 
      \version	0.1
      \author 	wichai tossamartvorakul wichai@geartronics.net
      \date	   13/08/08

      \n \b Update \b History
 
	\n Date                 By                     Comments
	\n ----                 --                     ---------
    \n
 
 
	\note   To use this library   interrupt service routine SIG_INPUT_CAPTURE1 had been define in E_io_ir.c module
	\n	SIGNAL(SIG_INPUT_CAPTURE1)
	\n	{	
	\n		uint16_t  ir_code;
    \n		set_timer_mode (TIMER1B,ICP_RISING, OC_DISCONNECT, CLK_BY_8);
    \n		cli();  
	\n		ir_code  = io_ir_decode(ICR1);
	\n		if (ir_code)
	\n		{       
    \n	    io_ir_update( ir_code);    //End code dispatch command 
	\n	// Change to falling edge to wait for start bit 
	\n		set_timer_mode (TIMER1B,ICP_FALLING, OC_DISCONNECT, CLK_BY_8);	
	\n		}   
	\n		sei();				
	\n	}

*/
  

//! External include file
#include <avr/io.h>
#include "cpu.h"
 


//!Constant

#define  MAX_INT16   65536
#define  READ_IR	 0
#define  WRITE_IR    1 
#define  IR_TV_DELAY   CPU_CLOCK_SPEED*7   // Delay for IR Remote



//! Function prototypes
void 		io_ir_init(void);
uint16_t  	io_ir_read(uint16_t);
//uint16_t 	io_ir_decode(uint16_t);
 


// PULSE LENGTH  Calculate from Clock speed and prescale 
// at Clock 1 MHz and prescale 8
// START_BIT 	300   4T  
// BIT1 	 	200   3T  
// PULSE WIDTH 	 75    T  600 uS

  
#define  IR_TV_STOP_BIT		12


// Define setting up parameters
// Remote control code  Sony at this moment measure at prescale = clk/8 and clk = 16 Mhz

 
#define  SONY			// SOny Remote control


// Define setting up parameters
// Remote control code  Sony at this moment
#ifdef SONY
//Sony Channel
#define  IR_TV_1  			0x0010
#define  IR_TV_2  			0x0810
#define  IR_TV_3  			0x0410
#define  IR_TV_4  			0x0c10
#define  IR_TV_5  			0x0210
#define  IR_TV_6  			0x0a10 
#define  IR_TV_7  			0x0610
#define  IR_TV_8  			0x0E10 
#define  IR_TV_9  			0x0110
#define  IR_TV_0  			0x0910 
#define  IR_TV_CHUP 		0x0090
#define  IR_TV_CHDOWN 		0x0890
#define  IR_TV_VOLUP  		0x0490
#define  IR_TV_VOLDOWN 		0x0C90
#define  IR_TV_POWER   		0x0A90
#define  IR_TV_TVAV      	0x0A50
#define  IR_TV_MUTE    		0x0290
#define  IR_TV_MINUS		0xAF0
#define  IR_TV_PLUS         0x2F0
#define  IR_TV_UP           0x950
#define  IR_TV_DOWN         0x3F0
#define  IR_TV_START_BIT  	CPU_CLOCK_SPEED*300  
#define  IR_TV_BIT1	   		CPU_CLOCK_SPEED*200
#define  IR_TV_PULSE_WIDTH  CPU_CLOCK_SPEED*75 
#define  IR_TV_STOP_BIT		12
//! For VCD they will precede by 0a00 0409
#define  IR_VCD_POWER		0x0A8C
#define  IR_VCD_PLAY		0x058C
#define  IR_VCD_PLUS        0x2F0
#define  IR_VCD_MINUS		0xAF0
#define  IR_VCD_UP          0x990
#define  IR_VCD_DOWN        0x3F0
#define  IR_VCD_NEXT		0xF8C
#define  IR_VCD_PREV		0x78C
#define  IR_VCD_STOP		0x18C
#define  IR_VCD_FF			0x2CC
#define  IR_VCD_REV			0xACC
#define  IR_VCD_PAUSE       0x98C
#define  IR_VCD_ANGLE       0x02C
#define  IR_VCD_RL			0xE8C

#endif

#ifdef PANASONIC
 
#define  IR_TV_START_BIT  	CPU_CLOCK_SPEED*300  
#define  IR_TV_BIT1	   		CPU_CLOCK_SPEED*200
#define  IR_TV_PULSE_WIDTH  CPU_CLOCK_SPEED*75 
#define  IR_TV_STOP_BIT		12

//! For  PAnasonic VCD they will precede by 0a00 0409
#define  IR_VCD_POWER		0x9057
#define  IR_VCD_PLAY		0x904a
#define  IR_VCD_NEXT		0x9054     //!< Show  >>|
#define  IR_VCD_PREV		0x9044      //!< Show  |<<
#define  IR_VCD_STOP		0x9040
#define  IR_VCD_FF			0x904a		//!< Show  >>
#define  IR_VCD_REV			0x9052		//!< Show  <<
#define  IR_VCD_PAUSE       0x904c
#define  IR_VCD_ANGLE       0x9041
#define  IR_VCD_RL			0x9059

#endif
 


 
 




/* 
 Include file for ir_tv.c
 
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   21/3/2006
 
 
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
*/

/* External Dependents    Need outside parameters 

CPU_CLOCKSPEED   in 		cpu_xxxx.h


*/


#include <avr/io.h>
#include "cpu.h"


// Function prototypes
uint16_t sony_ir_decode(uint16_t);
void init_ir(void);


// Port use 

#define IR_DDR_PORT		 E_DIR_PORTD
#define IR_ICP			 E_PORT6

// Delay for IR Remote
#define IR_TV_DELAY   CPU_CLOCK_SPEED*375



// Define setting up parameters
// Remote control code  Sony at this moment measure at prescale = clk/8 and clk = 16 Mhz


//Sony Channel IR CODE
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
#define  IR_TV_TVPOWER   	0x0A90
#define  IR_TV_TVAV      	0x0A50
#define  IR_TV_TVMUTE    	0x0290

// PULSE LENGTH  Calculate from Clock speed and prescale 
// at Clock 1 MHz and prescale 8
// START_BIT 	300   4T  
// BIT1 	 	200   3T  
// PULSE WIDTH 	 75    T  600 uS
#define  PRESCALE   128     // 1024/8
#define  IR_TV_START_BIT  	CPU_CLOCK_SPEED*300/PRESCALE  
#define  IR_TV_BIT1	   		CPU_CLOCK_SPEED*200/PRESCALE 
#define  IR_TV_PULSE_WIDTH  CPU_CLOCK_SPEED*75/PRESCALE  
#define  IR_TV_STOP_BIT		12

// ETC

#define  MAX_INT16   65536

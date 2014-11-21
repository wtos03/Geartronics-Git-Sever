/*!  
	\file   	E_io_ir.c
	\brief  	IR rx module for E_IO block
	\ingroup	E_io_ir
	\par  		Support only Sony or remote which use  Pulse width for 1,0  ex PANASONIC
        \n  	
 	\n Original written for 	:   \b for bold ATMEGA32
 	\n CPU port/tested		:   ATMEGA32
 	\n Hardware use			:   E_IO with RX  TSOP4838 ??
 	\n Port use				:   E_PB7 (Need to set Jumper J1 to connect to E_PB7
	\n							TIMER1, ICP mode  ICP pin ( map to E_PB7 with J1)
 	\n Memory	FLASH		:   XXX
 	\n			RAM			:   XXX
 	\n			EEPROM		:   XXX
 	\n Reference Document	:   Document describe algorithm

	\version 0.1
 	\author  Wichai Tossamartvorakul / wichai@geartronics.net
	\date 	 22/8/2008

	\par Update History
 	\nDate			By 					Comments
 	\n----			--					--------
	\n 
	\note   IR Module may report many repeat IR code if you hold. Need to add algorithm to check release key
	
 
*/






// Include file
 
#include <avr/interrupt.h>
#include "E_io_ir.h"
#include "avr_timer.h"



// Start routine here


 

/*!	\brief  Initial IR Module by initial TIMER1 to ICP mode on  
	\param  none	  
	\return none
	\n		   
	\par Port/Periheral Usage 
	\n   E_TIMER0   ( OC2 for timer2 on ATMEGA32)
	\par Theory of Operation   
	\n   Initial timer0 to measure the lenght of pulse on TIMER0 pin by counting the time between
	\n	 falling -> Rising for Start bit. And Rising ->Another Rising for data. The logic 1 and 0 will 
	\n	 have different pulse width 
	\n    
	\par Pseudo Code
	\n	 	Set ICP pin on E_PB7 to input
	\n   	Initial timer1B for ICP triggle on FALLING EDGE
	\n	    Enable Interrupt  for ICP_CAPTURE
	\note	
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/       

void io_ir_init(void)
{
 //Set Direction for input 
 	E_DIR_PB7(INPUT);
	set_timer_mode (TIMER1B,ICP_FALLING, OC_DISCONNECT, CLK_BY_8);  // It use as input not output
	TIMSK   |= _BV(TICIE1);   				// Enable Interrupt for INPUT CAPTURE, Detect for IR signal
	sei();
}





/*!	\brief  Decode IR signal with help of interrupt INPUT CAPTURE
	\n       Support SONY at other with  Pulse width Modulation
	\param  Input  Input capture counter 16 bit register
	\n	    	  
	\return   	0  			= no code detect
	\n			ir_decode   = detect IR code 
	\n		   
	\par Port/Periheral Usage 
	\n   E_TIMER0   ( OC2 for timer2 on ATMEGA32)
	\par Theory of Operation   
	\n   Measure the pulse width between falling -> Rising for startbit.  Start bit = 4T Bit 1 = 3T others is 0
	\n   T= Normal pulse width depend on remote type. Sony = XX  If detect start bit, then detect for
	\n  data 1 or 0 until reach 12 bits. Then return the IR code. 
	\n	 
	\par Pseudo Code
	\n	 	Calculate pulse width by substract ICP counter. from pulse width
	\n		can determine whether it is  START BIT 		 =  4T
	\n						  			 BIT1 			 =  3T
	\n									 BIT0            =  2T
	\n									 T				 =  550 us	(1/2 of Wave's Period)
	\n		until 12 bits then return ir code					
	\note	
	\n		Only support Sony and Panasonic or other which use Pulse width Modulation and have same
	\n		T as Sony and Panasonic
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/       

uint16_t io_ir_decode ( uint16_t capture_counter)
{
  static 	uint16_t  	prev_capture_counter = 0;
  static 	uint8_t   	stop_bit = 0;
  static	uint16_t	ir_code = 0;
 			uint16_t	width  = 0;	  
			
// Calculate width of pulse by compare with the prev counter
      if ( capture_counter > prev_capture_counter)
			width = capture_counter - prev_capture_counter;
	  else  // Counter had pass counter
			width = capture_counter+(MAX_INT16 - prev_capture_counter);
			
//Check for start bit
     if ( width > IR_TV_PULSE_WIDTH)
	 {
		  if (width > IR_TV_START_BIT)  			//  Found Start bit (4T)
			    {
				  stop_bit = 0;
				  prev_capture_counter = 0;
				  ir_code = 0;     			// New code
				}
		  else if (width > IR_TV_BIT1)	  		// bit 1 (3T)
				{
				   ir_code <<= 1;    			//Shift right
				   ir_code  |= 1;    			// Or 1
				    stop_bit++;
				}
		        else 	  // bit 0
				{
					ir_code  <<=  1;  //Shift right
					stop_bit++;
				}
	}
	prev_capture_counter = capture_counter;
	if (stop_bit == IR_TV_STOP_BIT)
	{       //End code dispatch command
  	    stop_bit = 0;
		return ir_code;
			
//Note if  SELFTEST is selected g_irint = 0;
	}   
	else
		return 0;		// No code detect yet
}


/*!	\brief  Read or update the IR Signal  
	\param  code       = 0 (READ_IR)
	\n				   > 0  IR_CODE TO WRITE
	\return IR code 0  mean No code detect
	\n		   
	\par Port/Periheral Usage 
	\n   E_TIMER0   ( OC2 for timer2 on ATMEGA32)
	\par Theory of Operation   
	\n   Read of Write the IR code into  static variable. Avoid to use global variable
	\n	 
	\par Pseudo Code
	\n	 	 If code detect return code  and clear the code. This mode code will be read once only
	\n       If it does not detect anything return 0 
	\n       Update the internal code when IR code detected 
	\note	
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/       

uint16_t  io_ir_read( uint16_t code)
{
	static uint16_t ir_code = 0 ;   // IR code 
    if (code)  //  Write IR 
	{
        ir_code = code;
	    return 0;
	}
	else
	{
	    code = ir_code;
		ir_code = 0;     // Clear value after read
	    return code;
	}
}



/*!	\brief  This routine will detect/ process the Sony IR remote command. Protocol using Sony remote control
	\param  Infrared signal from  ICP pin	  
	\return none
	\n		   
	\par Port/Periheral Usage 
	\n   E_TIMER0   ( OC2 for timer2 on ATMEGA32)
	\par Theory of Operation   
	\n   Measure the pulse length by get the value of ICR1 (16 bits) Input capture register. ICR1 
	\n   will continuously count. When the pulse go to falling edge we keep the value of ICR1 and when 
	\n   the pulse go to rising edge. We capture the value of ICR1 again and substrate 2 values, then 
	\n	 we get the pulse width  for start bit.  But for check on bit 1 or 0 we measure the entire
	\n   wave form  Pulse on-off period. This is a lot of easier than measure only pulse on period.
	\n   Because we can set interrupt at rising edge only.
	\n	 
	\par Pseudo Code
	\n   - Set to rising edge to calculage for start bit
	\n	 - Sent the ICR1 value to decode the remote signal
	\n   - If IR complete code detect save the ir_code and set trigger to falling edge again
	\n		(Wait for start bit)
	\note   
	\bug    <bug to fix>

	\author wichai tossamartvorakul  wichi@geartronics.net
	\sa	 <see also two name joined by ::>
*/     
 
 SIGNAL(SIG_INPUT_CAPTURE1)
{
	static uint16_t  ir_code;   // Save time to allocate memory
	ir_code = ICR1;  // Need to get ICR1 first before any operation
    set_timer_mode (TIMER1B,ICP_RISING, OC_DISCONNECT, CLK_BY_8);
    cli();  
	ir_code  = io_ir_decode(ir_code);
	if (ir_code)
	{       
        io_ir_read( ir_code);    //End code dispatch command send ir_code to keep in buffer
// Change to falling edge to wait for start bit 
		set_timer_mode (TIMER1B,ICP_FALLING, OC_DISCONNECT, CLK_BY_8);	
	}   
	sei();
 
}

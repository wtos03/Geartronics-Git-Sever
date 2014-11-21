/*

 Module 				:   IO Test Module
 Description 			:   IO  Test module for CHapter xxx
 Original written for 	:   ATMEGA32
 CPU port/tested		:   ATMEGA32
 Hardware use			:   Dual 7 Segments common cathode module
 Port use				:   E_PORTA (0-7), E_PORTB (0-1)
 Memory	FLASH			:   XXX
		RAM				:   XXX
		EEPROM			:   XXX
 Document				:   Document describe algorithm
 Written by 			:   Wichai  wichai@geartronics.net
 Date					:  	23/04/2006
  
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
  */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <stdlib.h>
#include "Dual_7Segment.h"
#include "DC_motor.h"
#include "ir_tv.h"
#include "E_uart.h"
#include "text_decode.h"


// Global variable

uint8_t  g_mode1  = MOTOR_STOP;
uint8_t  g_mode2  = MOTOR_STOP;
uint8_t  g_motor;             	// Motor number to process
uint8_t  g_speed;				// Speed Motor
int16_t  g_angle;	            // Motor Angle	


// ------ UART
uint8_t     g_rcv_buff[MAX_BUFF]; 
uint8_t		g_cmd_decode =0;				// 1 = Ready to decode 



void main(void);
void init_board(void);



void delay ( uint16_t  time)
{
   uint16_t i;
   for (i = 0 ; i < time; i++);

}


/* This subroutine will handle IR remote decode by getting remote code from Interrupt handle for input capture
and will response  to each remote key. For other applications user can modified here for different remote active
However this routine support Sony remote control only
   global effect   g_irint;
*/

uint8_t remote_decode ( uint16_t ircode)
{ 
  uint16_t pwm;
  uint8_t speed, found = 1;
  speed = 10;
  switch ( ircode)
	   {
	    case IR_TV_1:	
			speed = 2;
			 break;
		case IR_TV_2:
			speed = 3;
			 break;
	    case IR_TV_3:
			speed = 4;
			 break;
		case IR_TV_4:
			speed = 5;
			 break;
	    case IR_TV_5:
			speed = 6;
			 break;
		case IR_TV_6:
			speed = 7;
			 break;
	    case IR_TV_7:
			speed = 8;
			break;
		case IR_TV_8:
			speed = 9;
			break;
	    case IR_TV_9:
			speed = 10;
			break;
		case IR_TV_0:
			g_mode1  = MOTOR_STOP;
			g_mode2  = MOTOR_STOP;
			break;
		case IR_TV_CHUP:							
        	g_mode1 = MOTOR_FORWARD;		
			g_mode2 = MOTOR_FORWARD;
			break;
		case IR_TV_CHDOWN:							
			g_mode1 = MOTOR_BACKWARD;
			g_mode2 = MOTOR_BACKWARD;
			break;
		case IR_TV_TVMUTE:
			break;
		case  IR_TV_TVPOWER:
		  	break;
		case   IR_TV_TVAV:
		    break;
		case  IR_TV_VOLUP:
            g_mode1 = MOTOR_FORWARD;		
			g_mode2 = MOTOR_BACKWARD;
			break;
		case  IR_TV_VOLDOWN:                 			
			g_mode1 = MOTOR_BACKWARD;
			g_mode2 = MOTOR_FORWARD;
			break;
		default:
			found = 0;
			g_mode1  = MOTOR_STOP;
			g_mode2  = MOTOR_STOP;
			break;
 	   }
       if (found)
	   {	pwm = (speed * 100);             // Start from +  speed*2^4
			OCR1A = pwm;
			OCR1B = pwm;
		}	
	   return found;

}





/* Command decode from g_rcv_buffer  will decode and action according to command in buffer
 
*/

int16_t cmd_decode (void)
{

 uint8_t  cmd_no =0;
 uint8_t  *cmd_argument;
 uint8_t  *buffer_ptr;
 
 
// Should repeat until end of buffer
 
// Loop until end of text

   buffer_ptr   = g_rcv_buff;
   cmd_argument = g_rcv_buff;
     
   while ( cmd_no !=  CMD_LINE_END)
   {
 		cmd_no =  text_decode (buffer_ptr,cmd_argument);
//		debug_value (cmd_no,10);
		uart_puts("Command :");
		uart_putc(0x30+cmd_no);
		uart_putc('*');
		uart_puts(cmd_argument);
  
 		switch (cmd_no)
		{
			case MOTOR_CMD: 
				g_motor = (uint8_t)atoi(cmd_argument);
 				break;
			case  SPEED_CMD:	
				g_speed = (uint8_t)atoi(cmd_argument);
  				break;
			case  ANGLE_CMD:   // Can be - 
				g_angle = atoi(cmd_argument);
				if (g_angle > 0)
				{
					if (g_motor == 1)
						g_mode1 = g_angle;
					else
						g_mode2 = g_angle;
				}
				else
					uart_puts("Negative value");
				break;	
//		    case  CMD_LINE_END:
//				break;
			default:
			    uart_puts("Command not found ");
// 				uart_putc('-');
//				uart_puts(cmd_argument);
		        break;	  
		}		
    }
 return 0; 
}


 
 
/* UART receiver ready
*/

SIGNAL(SIG_UART_RECV)
{
  static uint16_t i= 0;
  uint8_t ch;
  ch = UDR;

	g_rcv_buff[i] = ch;    
	i++;
 
// Found 0xOD or 0x0A end with null
   if ((ch == 0x0D) || (ch == 0x0A)) // CANNOT USE UDR to compare
   {	
    	g_rcv_buff[i]  = 0;   // End with null
		i = 0;
  		g_cmd_decode = 1;
   }
}

  

/*************************************************************************
Function: UART Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/

SIGNAL(SIG_UART_DATA)
{ 
    uint8_t  ch;
	if ((ch = uart_get_TX()))
	{
 		UDR = ch;
	}	
	else 
// No character disable  interrupt to avoid repetable call Enable again when
//	send character in putc procedure
	{
	    UCSRB &=  ~(1<<UDRIE);
	}
	
}

 
 
/* Interrupt handle for Input Capture
This routine will detect/ process the Sony IR remote command. Protocol using Sony remote control
INPUT   :  Infrared signal from  ICP pin
		   Command table 
		  

OUTPUT 	: g_num  Number of message to display
		: g_control.display_mode   Display mode of message
		: g_start_bit
		

*/
SIGNAL(SIG_INPUT_CAPTURE1)
{
	uint16_t  ir_code;
    ir_code = ICR1;
	cli(); 

    TCCR1B  |= _BV(ICES1);	     // Rising edge
 	ir_code  = sony_ir_decode(ir_code);  

	if (ir_code)
	{       //End code dispatch command    
 
		remote_decode (ir_code);
// Return to falling edge for Start		
		TCCR1B  &= ~(_BV(ICES1));

//Note if  SELFTEST is selected g_irint = 0;
	} 
    sei();
 
}


void init_board()
{
	d7segment_init();
	init_ir();
	init_motor();
	uart_init(); 
  
	g_cmd_decode = 0; 
	sei();
}

void main()
{
  uint8_t sw1,sw2;
  uint16_t k;
  int8_t d1,d2;
  uint8_t show[3] = {0,'f','b'};
  
  
//  enum {MOTOR_STOP,MOTOR_FORWARD,MOTOR_BACKWARD,MOTOR_BRAKE};
  d7segment_init();   // Init both SW and 7 Segments
  init_ir();
  init_motor();
  uart_init(); 
   
  sei();
  d1 = 0;
  d2 = 0;

  
  while (1)
  {
   sw1 = _7SEGMENT_SW1_IN_PORT & _7SEGMENT_SW1; 
   
   if (!sw1) // SW1  Press
    {   	
		d1++;
	     if (d1 > MOTOR_BACKWARD)
			d1 = MOTOR_STOP;
		for (k = 0; k < 500 ;k++)                         // Delay for key bounce
				delay(6000);						
		g_mode1 = d1;

	}
   
   sw2 = _7SEGMENT_SW2_IN_PORT & _7SEGMENT_SW2; 
   if (!sw2) // SW2 Press
   {
        d2++;
		if (d2 > MOTOR_BACKWARD)
			d2 = MOTOR_STOP;
 		for (k = 0; k < 500 ;k++)                         // Delay for key bounce		
 				delay(6000);
		g_mode2 = d2;
	
   }

 
 
// Command decode
    if ( g_cmd_decode)
	{
// Decode command	
   	     cmd_decode ();
		 g_cmd_decode = 0;
	
	}


// Motor contol section

	motor_control(1, g_mode1);
	motor_control(2, g_mode2);
	
//  		for (k = 0; k < 10 ;k++)                         // Delay for key bounce
//			delay (30000);
//		motor_control (1, MOTOR_STOP);

   	d7segment_display(show[g_mode1],1);
 			delay(12000);
	d7segment_display(show[g_mode2],2);
 			delay(12000);
		
    
  }
     
}



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
#include  "adc.h"


// Operation Mode
#define DEBUG




// Global variable

// Motor control

uint8_t  g_mode[MOTOR_MAX]  = {MOTOR_STOP,MOTOR_STOP};
uint8_t  g_motor;             	// Motor number to process
uint16_t  g_speed[MOTOR_MAX];				// Speed Motor
int16_t  g_angle[MOTOR_MAX];	            // Motor Angle	
uint8_t  g_adc_channel[MOTOR_MAX] = {0x0,0x02};  // ADC0-1 x1 Diff and ADC2-3 x1 Diff
uint16_t g_pos[MOTOR_MAX] = {0,0};
 

// ------ UART
uint8_t  g_rcv_buff[MAX_BUFF]; 
uint8_t	 g_cmd_decode =0;				// 1 = Ready to decode 



void main(void);
void init_board(void);



void delay ( uint16_t  time)
{
   uint16_t i;
   for (i = 0 ; i < time; i++);

}

void  set_motor_parameter ( uint8_t mode)
{
   switch (mode)
   {
    case  MOTOR_FORWARD:
        g_mode[0] = MOTOR_FORWARD;		
		g_mode[1] = MOTOR_FORWARD;
		g_angle[0] = 1;
		g_angle[1] = 1;
		break;
	case  MOTOR_BACKWARD:
		g_mode[0] = MOTOR_BACKWARD;
		g_mode[1] = MOTOR_BACKWARD;
		g_angle[0] = 1;
		g_angle[1] = 1;
		break;
    case MOTOR_STOP:
		g_mode[0]  = MOTOR_STOP;
		g_mode[1]  = MOTOR_STOP;
		g_angle[0] = 0;
		g_angle[1] = 0;
		break;
	case MOTOR_TURN_LEFT:
		g_mode[0] = MOTOR_FORWARD;		
		g_mode[1] = MOTOR_BACKWARD;
		g_angle[0] = 1;
		g_angle[1] = 1;
		break;
	case MOTOR_TURN_RIGHT:
		g_mode[0] = MOTOR_BACKWARD;
		g_mode[1] = MOTOR_FORWARD;
		g_angle[0] = 1;
		g_angle[1] = 1;
		break;
	 default:
	 	g_mode[0]  = MOTOR_STOP;
		g_mode[1]  = MOTOR_STOP;
		g_angle[0] = 0;
		g_angle[1] = 0;
		break;
   }
  
}

/* This subroutine will handle IR remote decode by getting remote code from Interrupt handle for input capture
and will response  to each remote key. For other applications user can modified here for different remote active
However this routine support Sony remote control only
   global effect   g_irint;
*/

void remote_decode ( uint16_t ircode)
{ 

  uint8_t set_speed = 0;
  switch ( ircode)
	   {
	    case IR_TV_1:	
			g_speed[g_motor] = 800;
			set_speed =1;
			 break;
		case IR_TV_2:
			g_speed[g_motor] = 700;
			set_speed =1;
			break;
	    case IR_TV_3:
			g_speed[g_motor] = 600;
			set_speed =1;			
			break;
		case IR_TV_4:
			g_speed[g_motor] = 500;
			set_speed =1;			
			 break;
	    case IR_TV_5:
			g_speed[g_motor] = 400;
			set_speed =1;			
			 break;
		case IR_TV_6:
			g_speed[g_motor] = 300;
			set_speed =1;			
			 break;
	    case IR_TV_7:
			g_speed[g_motor] = 200;
			set_speed =1;			
			break;
		case IR_TV_8:
			g_speed[g_motor] = 100;
			set_speed =1;			
			break;
	    case IR_TV_9:
			g_speed[g_motor] = 10;
			set_speed =1;			
			break;
		case IR_TV_0:
			set_motor_parameter (MOTOR_STOP);
			break;
		case IR_TV_CHUP:
			set_motor_parameter(MOTOR_FORWARD);
			break;
		case IR_TV_CHDOWN:							
			set_motor_parameter(MOTOR_BACKWARD);
			break;
		case IR_TV_TVMUTE:
			break;
		case  IR_TV_TVPOWER:
		  	break;
		case   IR_TV_TVAV:
		    break;
		case  IR_TV_VOLUP:
            set_motor_parameter(MOTOR_TURN_LEFT);
			break;
		case  IR_TV_VOLDOWN:                 			
		    set_motor_parameter(MOTOR_TURN_RIGHT);
			break;
		default:
 //			set_motor_parameter (MOTOR_STOP);
			break;
 	   }
       if (set_speed)
	   {	         // Start from +  speed*2^4
            g_speed[1] = g_speed[g_motor]*MOTOR_SPEED_MULTIPLY; 
			g_speed[0] = g_speed[1];
 	   }
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
     
   while ( cmd_no !=  CMD_LINE_END)     // Until end of line which mean g_motor will change
   {
 		cmd_no =  text_decode (buffer_ptr,cmd_argument);
#ifdef DEBUG		
		uart_puts("Command :");
		uart_putc(0x30+cmd_no);
		uart_putc('*');
		uart_puts(cmd_argument);
#endif        
 		switch (cmd_no)
		{
			case MOTOR_CMD: 
				g_motor = (uint8_t)atoi(cmd_argument);
				if (g_motor > 0)
				   g_motor--;                //  > 0 Start from 0 need -1
				else
				   g_motor = 0;
 
 				break;
			case  SPEED_CMD:	
				g_speed[g_motor] = (uint8_t)atoi(cmd_argument);
				g_speed[g_motor] *=  MOTOR_SPEED_MULTIPLY;
  				break;
			case  ANGLE_CMD:   // Can be - 
				g_angle[g_motor] = atoi(cmd_argument);
				if (g_angle[g_motor] < 0)
				{   
						g_mode[g_motor] = MOTOR_BACKWARD;
// Set g_angle to positive value by invert all bit and +1                
                        g_angle[g_motor] = (~(g_angle[g_motor])+1);
                        if  (g_angle[g_motor] > 1)
						{
 						   g_angle[g_motor] *= MOTOR_ANGLE_COEF;
						   g_speed[g_motor] =  1000*MOTOR_SPEED_MULTIPLY;
						}		
				}	
				else  if (g_angle[g_motor] == 0)
				        g_mode[g_motor] = MOTOR_STOP;
				else	  
				{
					if ((g_angle[g_motor] > 0))   //  +
						g_mode[g_motor] = MOTOR_FORWARD;
					 if  (g_angle[g_motor] > 1)
					 {
 						g_angle[g_motor] *=  MOTOR_ANGLE_COEF;	
						g_speed[g_motor] =  1000*MOTOR_SPEED_MULTIPLY;
					 }
 				}
				break;	
 			default:
			    uart_puts("Command not found ");
 		        break;	  
		}		
    }
 return 0; 
}

/* Calculate backemf and save in g_pos

*/
void calculate_backemf (uint8_t motor)
{
     uint16_t backemf=0;
  // Read backemf
	
	if (g_mode[motor] == MOTOR_FORWARD)
	{     	
		adc_init(3);
		backemf = read_adc(g_adc_channel[motor]);
		backemf = read_adc(g_adc_channel[motor]);
 	}
	if (g_mode[0] == MOTOR_BACKWARD)	
	{   
		adc_init(3);
		backemf = read_adc((g_adc_channel[motor]+1));		
		backemf = read_adc(g_adc_channel[motor]);
 	}
	if (backemf > 211)
    {
		backemf  -= 211;
		debug_value(backemf,10);
		g_pos[motor] += backemf;
    }
}



// Interrupt Vector table

 
/*
 OC1A Compare match  Toggle between on and off
*/ 
SIGNAL (SIG_OUTPUT_COMPARE1A)
{
  static uint8_t toggle=0;
 
  if (toggle)   // ON period
  {  
    OCR1A = 0x0FFF - g_speed[0];
    motor_control(1, g_mode[0]);
    if ( g_mode[0] != MOTOR_STOP)
			uart_putc ('x');	
 
  }
  else //  OFF period
  {
    OCR1A =  g_speed[0];
	motor_control(1, MOTOR_STOP);
	calculate_backemf(0);
  }
  
  toggle ^= 1;

}


/*
 OC1B Compare match 
*/ 
SIGNAL (SIG_OUTPUT_COMPARE1B)
{
 static uint8_t toggle=1;     // Opposite with OCR1A for optimize
 
  if (toggle)   // ON period
  {  
    OCR1B =  g_speed[1];
	motor_control(2, g_mode[1]);
 	
  }
  else //  OFF period
  {
    OCR1B = 0x00FF - g_speed[1];
	motor_control(2, MOTOR_STOP);	 
//	calculate_backemf(1);
  }
  
  toggle ^= 1;
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
    uint8_t i;
	d7segment_init();
	init_ir();
	init_motor();
	uart_init(); 
    adc_init(1);   // AVCC as reference 
	
	g_cmd_decode = 0; 
	g_motor= 0;
	
	for ( i = 0 ; i< MOTOR_MAX ;i++)
	{
		g_speed[i] = 10;
		g_angle[i] = 1;
	} 
	sei();
 
}

void main()
{
  uint8_t sw1,sw2,i;
   uint16_t k,backemf;
  int8_t d1,d2;
  uint8_t show[3] = {0,'f','b'};
   init_board();
  d1 = 0;
  d2 = 0;
  backemf =0;
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
		g_mode[0] = d1;
		g_angle[0] = 1;    // Continuous move

	}
   
   sw2 = _7SEGMENT_SW2_IN_PORT & _7SEGMENT_SW2; 
   if (!sw2) // SW2 Press
   {
        d2++;
		if (d2 > MOTOR_BACKWARD)
			d2 = MOTOR_STOP;
 		for (k = 0; k < 500 ;k++)                         // Delay for key bounce		
 				delay(6000);
		g_mode[1] = d2;
		g_angle[1] = 1;    // Continuous move	
   }

// Command decode
    if ( g_cmd_decode)
	{
// Decode command	
   	    cmd_decode ();
		g_cmd_decode = 0; 
		g_pos[g_motor] = 0;
	}
	
 	
    for (i = 0 ; i < MOTOR_MAX ; i++)
	{
// If angle > 1 Backemf calculate
		if (((g_angle[i] > 1)) && ( g_mode[i] != MOTOR_STOP))
		{   
  
			if ( g_pos[i]  >= (g_angle[i]))  //  Check for position
			{
 				g_mode[i] = MOTOR_STOP;
				uart_puts ("Voltage SUM");
				debug_value(g_pos[i],10);
				g_pos[i] =0;
				
			}
		}	
		
// 7 Segments display	
		d7segment_display(show[g_mode[i]],i+1);
 			delay(10000);	
 
    }
	
//	adc_init(3);
//	backemf = read_adc((g_adc_channel[0]));   
// 	debug_value(backemf,10);
  }
  
}



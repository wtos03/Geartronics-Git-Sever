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
#include <avr/signal.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include "Dual_7Segment.h"
#include "DC_motor.h"
#include "ir_tv.h"
#include "E_uart.h"
#include "text_decode.h"
#include "adc.h"
#include "backemf.h"


// Operation Mode
#define DEBUG


#define EEPROM __attribute__((section(".eeprom")))


// Global variable

// Motor control

uint8_t  g_mode[MOTOR_MAX]  = {MOTOR_STOP,MOTOR_STOP};
uint8_t  g_motor;             	// Motor number to process

uint8_t  g_speed[MOTOR_MAX];				// Speed Motor
uint8_t  g_speed_table[MOTOR_MAX_SPEED] 	= {120,160,190,200,210,220,230,240,250}; 
uint8_t  g_adc_channel[MOTOR_MAX] = {0x10,0x1b};  // ADC0-1 x1 Diff and ADC2-3 x1 Diff
int32_t  g_pos[MOTOR_MAX] = {0,0};
int32_t  g_angle[MOTOR_MAX];	            // Motor Angle	
uint8_t  g_status[MOTOR_MAX] = {0,0}; 
// ------ UART
uint8_t  g_rcv_buff[MAX_BUFF]; 
uint8_t	 g_cmd_decode =0;				// 1 = Ready to decode 
// ------ IR
uint16_t  g_ircode;

// ------ Store program
uint8_t  g_program_eeprom[MAX_PROGRAM] EEPROM = {"motor = 1 angle = 10  motor =2 angle= -10 \n motor=1 angle=-100 \n"};
uint8_t  g_program_buff[MAX_PROGRAM] =  {"angle = 22"};
uint8_t  g_program_flag =0;
uint16_t g_program_index =0;


void main(void);
void init_board(void);
int16_t cmd_decode (uint8_t *);


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

 
// Run program which save in eeprom or g_program_buff
//  Mode =1 run from eeprom
//  Mode =2 run from buffer
void program_run (void)
{
   uint16_t i;
   uint8_t ch,j;
   uint8_t  buff[MAX_BUFF];
   
      i = 0;
 	  while ((ch = eeprom_read_byte(&g_program_eeprom[i])))   //Get EEPROM BYTE/BYTE
	  {
			g_program_buff[i] = ch;
			i++;
	  }
	  g_program_index = i;
   
 
   i = 0;
   j = 0;
   
   while ((ch = g_program_buff[i]))
   {
 		buff[j] = ch;
		j++;
		if ((ch == 0x0D) || (ch == 0x0A)) // CANNOT USE UDR to compare
		{	
// Copy to buffer 
			buff[j]  = 0;   // End with null for decode
			uart_puts(buff);
		    cmd_decode(buff);
 
// Check both finish before proceed next command		
 			while ((g_status[0] | g_status[1]))    
			{
 				delay (15000);                    // Need to put delay  to make it work. DOn't have time to figure out why ???
			}
			j = 0;
 		}
		
		i++;	 
   }
}



/* Command decode from g_rcv_buffer  will decode and action according to command in buffer
 
*/

int16_t cmd_decode (uint8_t *buffer_ptr)
{

 uint8_t  cmd_no =0;
 uint8_t  *cmd_argument;
 uint8_t   i,j ;
 
// Should repeat until end of buffer
 
// Loop until end of text

   cmd_argument = buffer_ptr;
     
   while ( cmd_no !=  CMD_LINE_END)     // Until end of line which mean g_motor will change
   {
 		cmd_no =  text_decode (buffer_ptr,cmd_argument);
#ifdef DEBUG		
//		uart_puts("Command :");
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
				i = (uint8_t)atoi(cmd_argument);
				if ( i >=  MOTOR_MAX_SPEED)
						i = MOTOR_MAX_SPEED;    // Start from 0
				if (i)   // i > 0; 		
					i--;   //Start from 0;		
				g_speed[g_motor] =  g_speed_table[i];
				if (g_speed[g_motor] == 0)
				        g_mode[g_motor] = MOTOR_STOP;
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
						   g_status[g_motor] = 1;
 						   g_speed[g_motor] = g_speed_table[MOTOR_ANGLE_SPEED] ;
						}		
				}	
				else  if (g_angle[g_motor] == 0)
				       {  	
							g_mode[g_motor] = MOTOR_STOP;
 					   }
				else	  
				{
					if ((g_angle[g_motor] > 0))   //  +
					{
						g_mode[g_motor] = MOTOR_FORWARD;
 					}
					if  (g_angle[g_motor] > 1)
					{
 						g_angle[g_motor] *=  MOTOR_ANGLE_COEF;	
						g_status[g_motor] = 1;
  						g_speed[g_motor]  = g_speed_table[MOTOR_ANGLE_SPEED] ;
					}
 				}
                g_pos[g_motor] = 0;
				break;	
			case PROGRAM_CMD:
				g_program_flag = (uint8_t)atoi(cmd_argument);
				if (g_program_flag)   // Start  program save in buffer
				{
					uart_puts ("Program Start to save : ");
				     g_program_index = 0;
				}
				else
				{  
					uart_puts ("Program End : ");
				}
				break;
 			case SAVE_CMD:
				j = (uint8_t)atoi(cmd_argument);
				if ( j > 0)    // Save commmand to eeprom
                {
					g_program_buff[g_program_index] = 0x00 ;  // Put null at the end
                    g_program_index++;   
                    for ( i =0; i < g_program_index; i++)
                    {
						while (!eeprom_is_ready());
							eeprom_write_byte(&g_program_eeprom[i],g_program_buff[i]);
					}
					debug_value( g_program_index,10);
					uart_puts ("Save complete ");
					g_program_index = 0;	
				}
				break;
		    case RUN_CMD:
			    program_run();
			    break;
 			default:
			    uart_putc(0x0D);
				uart_putc(0x0A);
//			    uart_puts("Command not found ");
 		        break;	  
		}		
    }
 
 return 0; 
}





/* Calculate backemf and save in g_pos

*/
void  calculate_backemf (uint8_t motor)
{
     int16_t backemf=0;
  // Read backemf
     static uint8_t  i = 0;

	adc_init(1);
	backemf = read_adc(g_adc_channel[motor]);	
// Cut offset	
	if ((backemf > 0) && (backemf < MOTOR_OFFSET))
			backemf =0;
			
// Negative convert to positive with 2 complement	
	if (g_mode[motor] == MOTOR_STOP)   
    {	
		if (backemf & 0x200)
		{
		    backemf = (~(backemf) & (0x03FF))+1;
 		}		
	}		
			
	if (g_mode[motor] == MOTOR_FORWARD)   // Motor stop backemf -  Forward backemf +
    {	
		if (backemf & 0x200)
		{
		    backemf = (~(backemf) & (0x03FF))+1;
 		}
		else
			backemf = 0;
    }
	if (g_mode[motor] == MOTOR_BACKWARD)   // Motor stop backemf +  backward backemf -
    {	
		if (backemf & 0x200)
		{
			backemf =0;
		}
    }
	
	g_pos[motor] += backemf;
    if (g_mode[motor] != MOTOR_STOP)	
 	{   
//	    i++;
//		if (i == 100 )
		{
//		    uart_putc(motor+0x30);
//			uart_putc('-');
//			debug_value(backemf,10);
			i = 0;
		}
	}	
//Compare voltage	
	if (((g_angle[motor] > 1)) && ( g_mode[motor] != MOTOR_STOP))
	{   
//Compare voltage sum	
		if ( g_pos[motor]  >= (g_angle[motor]))  //  Check for position
		{
			g_mode[motor] = MOTOR_STOP;			
 			uart_puts ("Voltage SUM");
			debug_lvalue(g_pos[motor],10);
			g_pos[motor] =0;	
            g_status[motor] = 0;    // Clear status proceed with next command			
		}
	}
 		
}



/* This subroutine will handle IR remote decode by getting remote code from Interrupt handle for input capture
and will response  to each remote key. For other applications user can modified here for different remote active
However this routine support Sony remote control only
   global effect   g_irint;
*/

void remote_decode ( uint16_t ircode)
{ 

  uint8_t set_speed = 8 ;
  switch ( ircode)
	   {
	    case IR_TV_1:	
 			set_speed = 0;
			 break;
		case IR_TV_2:
			set_speed =1;
			break;
	    case IR_TV_3:
 			set_speed =2;			
			break;
		case IR_TV_4:
 			set_speed =3;			
			 break;
	    case IR_TV_5:
 			set_speed =4;			
			 break;
		case IR_TV_6:
 			set_speed = 5;			
			 break;
	    case IR_TV_7:
 			set_speed = 6;			
			break;
		case IR_TV_8:
 			set_speed = 7;			
			break;
	    case IR_TV_9:
 			set_speed =8;			
			break;
		case IR_TV_0:
			set_motor_parameter (MOTOR_STOP);
			uart_puts ("Voltage SUM 1");
			debug_lvalue(g_pos[0],10);
			g_pos[0] =0;
			uart_puts ("Voltage SUM 2");
			debug_lvalue(g_pos[1],10);
			g_pos[1] =0;
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
		    program_run();
		    break;
		case  IR_TV_VOLUP:
            set_motor_parameter(MOTOR_TURN_LEFT);
			break;
		case  IR_TV_VOLDOWN:                 			
		    set_motor_parameter(MOTOR_TURN_RIGHT);
			break;
 		default:
			break;
 	   }
	   g_speed[1] = g_speed_table[set_speed]; 
	   g_speed[0] = g_speed[1];

}


// Interrupt Vector table
// Couter timer 0 reach top  
//    ON Motor   0 - OCR0  ON   OCR0 - 0xFF OFF
 SIGNAL (SIG_OVERFLOW0)   
{ 
    calculate_backemf(0); 
	OCR0 =  g_speed[0];
    motor_control(1, g_mode[0]);
	MOTOR_ENABLE12_CONTROL  |= MOTOR_ENABLE12;  // Enable Motor control
		
}

// Counter timer 0 reach OCR0  OFF MOTOR
 SIGNAL (SIG_OUTPUT_COMPARE0)
{
		MOTOR_ENABLE12_CONTROL   &= ~(MOTOR_ENABLE12);   // Disable Motor control Float motor 
//		motor_control(1, MOTOR_STOP);
}
 
// TIMER2 will opposite with timer 0 to balance calculate backemf between timer0 and timer2
// Couter timer 2 reach top  
//    ON Motor   0 - OCR2  ON   OCR2 - 0xFF FF
 SIGNAL (SIG_OVERFLOW2)   
{
 	MOTOR_ENABLE34_CONTROL  &= ~(MOTOR_ENABLE34);   // Disable Motor control Float motor 
//	motor_control(2, MOTOR_STOP);
   
}

// Counter timer 2 reach OCR2  ON MOTOR
 SIGNAL (SIG_OUTPUT_COMPARE2)
{
	calculate_backemf(1);
  	OCR2 =  0xFF - g_speed[1];   // Because opposite with timer0
    motor_control(2, g_mode[1]);
 	MOTOR_ENABLE34_CONTROL  |= MOTOR_ENABLE34;
 
}
 
   
/* UART receiver ready
*/

SIGNAL(SIG_UART_RECV)
{
  static uint16_t i= 0;
  uint8_t ch;
  ch = UDR;

  if (g_program_flag)   // Save into buffer
  {
    g_program_buff[g_program_index] = ch;
	if (g_program_index < MAX_PROGRAM)
	{
		g_program_index++; 
	}	
  }
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
// 	cli(); 

    TCCR1B  |= _BV(ICES1);	     // Rising edge
 	ir_code  = sony_ir_decode(ir_code);  
 	if (ir_code)
	{       //End code dispatch command    
 
        g_ircode = ir_code;
// Return to falling edge for Start		
		TCCR1B  &= ~(_BV(ICES1));
 
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
    adc_init(3);   // 2.56 BG as reference 
	
	g_cmd_decode = 0; 
	g_motor= 0;
	g_program_index =0;
	g_ircode =0;
	
	for ( i = 0 ; i< MOTOR_MAX ;i++)
	{
		g_speed[i] = 1;
		g_angle[i] = 1;
 	} 
	sei();
 
}

     





void main()
{
  uint8_t sw1,sw2,i;
   uint16_t k;
   int16_t backemf;
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
   	    cmd_decode (g_rcv_buff);
		g_cmd_decode = 0; 
//  		g_pos[g_motor] = 0;
  	}
	if(g_ircode)
	{
	   remote_decode (g_ircode);
	   g_ircode =0;
	}
 
    for (i = 0 ; i < MOTOR_MAX ; i++)
	{
// 7 Segments display	
		d7segment_display(show[g_mode[i]],i+1);
        delay(500);
 
    }
	
//	adc_init(1);
// 	backemf = read_adc((g_adc_channel[0]));  
//	backemf = MOTOR_OFFSET- backemf;
// 	debug_value(backemf,10);
  }
  
}



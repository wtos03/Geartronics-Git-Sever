//AVR_IO
//Control the AVR from a remote host.  One of its main uses is to add analog and digital I/O
//to a host that has none (such as a PC) by reading and writing the port register.
// Command packets are sent by the host with the following formats:
//
// Commands
// 1. STX 0x57 register data  ETX         Write data to register
// 2. STX 0x52 register x     ETX         Read byte from register, byte is returned by UART
// 3. STX 0x45 x        x     ETX         Send the current error, byte is returned by UART
// x = don't care, but there must be a byte in that location.
//
// An error number is saved when an error occurs:
// Errors
// 0 no error
// 1 expected STX
// 2 expected ETX
// 3 invalid/unknown command
// 4 receive timeout
//
// Upon receiving a byte, a 5msec timer is active.  The next byte must arrive before the timer
// expires, else error 4 is set.  This is a recovery mechanism in case the host does not
// finish sending the command packet.
//
// Upon detection of any error, the command packet is ignored and the parse engine starts
// looking for the STX character.
//
// Development: Originally written for AVRGCC to run on the STK200
// Steven R. Nickels  2002/10/04


#include <io.h>
#include <interrupt.h>
#include <sig-avr.h>

#include "misc.h"
#include "avr_io.h"


//Globals
BYTE rx_timeout;
BYTE rx_buffer[16];
BYTE rx_in;
BYTE rx_out;
BYTE tx_buffer[10];
BYTE tx_numbytes;
BYTE tx_empty;

Commands command;
Errors error;
BYTE reg;
BYTE data;
ParseStates parse_state;


//Prototypes
void DoCommand( void );
void Error( Errors error_num );
void UartSend( void );


int main( void )
{
  //Initialization - set globals
	rx_timeout = FALSE;
  rx_in = 0;
  rx_out = 0;
  tx_empty = TRUE;
	tx_numbytes = 1;
	parse_state = PARSE_STATE_STX;
	error = ERROR_NONE;

  //Init the uart
  outp((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN),UCR);        																		//Enable RxD/TxD and ints
  outp((BYTE)UART_BAUD_SELECT, UBRR);                          																		//Set baud

	//Init timer 5msec (used for byte receive timeout)
  outp( (1<<TOIE0), TIMSK );
  outp( 4, TCCR0 );																																								//4MHz/256 = 64us, 64us * x = 5ms, x = 78, since it's an upcounter, TCNT0 must be set to 256-78=178

  //Enable Interrupts
  sei();                                    																											//Enable interrupts

	//Send ':' - this informs the host that we're up and run'n
  outp( 0x3A, UDR );																																							//Let the host know we're up and running

	//while forever
  for( ;; )
  {
		//Check receive timeout
		if( rx_timeout == TRUE )
		{
			if( parse_state != PARSE_STATE_STX )									//We timeout after ETX, but we should not generate an error
			{
				Error( ERROR_RX_TIMEOUT );		
				parse_state = PARSE_STATE_STX;
			}
			rx_timeout = FALSE;
		}

		//If data is in the receive buffer
    if( rx_in != rx_out )
    {
      switch( parse_state )
      {
        case PARSE_STATE_STX:
          if( rx_buffer[rx_out] == STX )
          {
            ++parse_state;
          }
          else
          {
          	Error( ERROR_START_FRAME );											//Stay in the PARSE_STATE_STX state
          }
          break;

        case PARSE_STATE_COMMAND:
          command = rx_buffer[rx_out];
          ++parse_state;
          break;
          
        case PARSE_STATE_REGISTER:
        	reg = rx_buffer[rx_out];
          ++parse_state;
          break;
          
        case PARSE_STATE_DATA:
          data = rx_buffer[rx_out];
          ++parse_state;
          break;
          
        case PARSE_STATE_ETX:
          if( rx_buffer[rx_out] == ETX )
          {
          	DoCommand();
          }
          else
          {
          	Error( ERROR_END_FRAME );
          }
					//fall into default

        default:
          parse_state = PARSE_STATE_STX;
          break;
      }
      ++rx_out;
      rx_out &= 0x0F;																				//WARNINIG, this only works if the size of rx_out is a factor of 2
    }
  }            
}


/*******************************************************
 DoCommand
*******************************************************/
void DoCommand( void )
{
	//Write register
	if( command == CMD_WRITE_REG )
  {
 		outp( data, reg );
 		
 		Error( ERROR_NONE );
    return;
  }
	  
	//Read register
	if( command == CMD_READ_REG )
  {
  	tx_buffer[0] = inp( reg );
    UartSend();
    
    Error( ERROR_NONE );
    return;
  }
  
	//Return the error
  if( command == CMD_GET_ERROR )
  {
		tx_buffer[0] = error;
		UartSend();
    return;
	}

	//If we got here, then the command is not valid
	Error( ERROR_INVALID_CMD );
}


/*******************************************************
 Error
 Records the current error, resets the parse state.
*******************************************************/
void Error( Errors error_num )
{
	error = error_num;
	
	parse_state = PARSE_STATE_STX;
}


/*******************************************************
 UartSend
 Sends a stream of bytes out the serial port
 The bytes must be in the tx_buffer array and the
 number of bytes must be set in tx_numbytes.
 The variable tx_empty must be initialized to 1 at
 startup.
*******************************************************/
void UartSend( void )
{
  BYTE i;

  for( i = 0; i < tx_numbytes; i++ )
  {
    while( tx_empty == 0 )
    {
      ;
    }
    tx_empty = 0;
    outp( tx_buffer[i], UDR );
  }
}


/*******************************************************
 ISR - UART has sent a byte and is ready for another.
*******************************************************/
SIGNAL(SIG_UART_TRANS)      
{
  tx_empty = 1;
}


/*******************************************************
 ISR - byte received in UART
 Received bytes are placed into rx_buffer
*******************************************************/
SIGNAL(SIG_UART_RECV)      
{
  rx_buffer[rx_in] = inp( UDR );
  rx_in++;
  rx_in &= 0x0F;																						//This only works if the size of rx_buffer is a factor of 2

  outp( RX_TIMEOUT_CNT, TCNT0 );														//Reset receive timeout
}


/*******************************************************
 ISR - receive timeout
*******************************************************/
SIGNAL(SIG_OVERFLOW0)
{
	rx_timeout = TRUE;
}






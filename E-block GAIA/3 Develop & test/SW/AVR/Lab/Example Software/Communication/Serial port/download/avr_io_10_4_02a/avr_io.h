//avr_io.h

#ifndef avr_io_h
#define avr_io_h


#define F_CPU             4000000       //4Mhz
//#define UART_BAUD_RATE    9600          //9600 baud
#define UART_BAUD_RATE    19200         //19200 baud
//#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16)-1)
#define UART_BAUD_SELECT  12            //19200 @ 4MHz

#define RX_TIMEOUT_CNT    0xB2

#define STX   0x02
#define ETX   0x03

typedef enum
{
  CMD_WRITE_REG   = 0x57, //W
  CMD_READ_REG    = 0x52, //R
  CMD_GET_ERROR   = 0x45  //E
}Commands;

typedef enum
{
  ERROR_NONE                = 0,
  ERROR_START_FRAME         = 1,
  ERROR_END_FRAME           = 2,
  ERROR_INVALID_CMD         = 3,
  ERROR_RX_TIMEOUT          = 4
}Errors;

typedef enum
{
	PARSE_STATE_STX,
	PARSE_STATE_COMMAND,
	PARSE_STATE_REGISTER,
	PARSE_STATE_DATA,
	PARSE_STATE_ETX
}ParseStates;

#endif //avr_io_h

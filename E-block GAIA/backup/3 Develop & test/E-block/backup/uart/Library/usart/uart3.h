#ifndef __UART_H__
#define __UART_H__

/* Global definitions */
typedef unsigned char  u08;
typedef          char  s08;
typedef unsigned short u16;
typedef          short s16;

/* UART Baud rate calculation */
#define UART_CPU               4000000      /* 4Mhz */
#define UART_BAUD_RATE         9600         /* baud rate*/
#define UART_BAUD_SELECT       (UART_CPU/(UART_BAUD_RATE*16l)-1)

/* Global functions */
extern void UART_SendByte       (u08 Data);
extern u08  UART_ReceiveByte    (void);
extern void UART_PrintfProgStr  (u08* pBuf);
extern void UART_PrintfEndOfLine(void);
extern void UART_Printfu08      (u08 Data);
extern void UART_Printfu16      (u16 Data);
extern void UART_Init           (void);

/* Macros */
#define PRINT(string) (UART_PrintfProgStr(PSTR(string)))
#define EOL           UART_PrintfEndOfLine
#endif


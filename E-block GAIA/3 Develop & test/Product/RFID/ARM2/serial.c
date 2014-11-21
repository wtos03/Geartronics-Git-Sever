/****************************************************/
/* Examples Program For "CP-JR ARM7 LPC2138" Board  */
/* Target MCU  : Philips ARM7-LPC2138               */
/*       	   : X-TAL : 19.6608 MHz                */
/*             : Run Speed 58.9824 MHz (With PLL)   */
/*             : PLL Setup = M(3),P(2)				*/
/*             : VPB Clock = CPU Clock = 58.9824MHz	*/
/* Keil Editor : uVision3 V3.03a                    */
/* Compiler    : Keil CARM V2.50a		            */
/* Create By   : Eakachai Makarn (WWW.ETT.CO.TH)    */
/* Last Update : 17/May/2006                        */
/* Function    : Example Use UART0	                */
/****************************************************/
// Print Message to UART0 (9600,N,8,1)

#include "LPC213x.H" 	                                	// LPC2138 MPU Register
#include <stdio.h>											// For Used Function printf 
#include "serial.h"


/******************************/
/* Initial UART0 = 9600,N,8,1 */
/* VPB(pclk) = 58.9824 MHz/2  */
/******************************/
void init_serial0 (void)  
{
  PINSEL0 &= 0xFFFFFFF0;									// Reset P0.0,P0.1 Pin Config
  PINSEL0 |= 0x00000001;									// Select P0.0 = TxD(UART0)
  PINSEL0 |= 0x00000004;									// Select P0.1 = RxD(UART0)

  U0LCR &= 0xFC;											// Reset Word Select(1:0)
  U0LCR |= 0x03;											// Data Bit = 8 Bit
  U0LCR &= 0xFB;											// Stop Bit = 1 Bit
  U0LCR &= 0xF7;											// Parity = Disable
  U0LCR &= 0xBF;											// Disable Break Control
  U0LCR |= 0x80;											// Enable Programming of Divisor Latches

  // U0DLM:U0DLL = 29.4912MHz / [16 x Baud]
  //             = 29.4912MHz / [16 x 9600]
  //             = 192 = 0x00C0
  U0DLM = 0x00;												// Program Divisor Latch(192) for 9600 Baud
  U0DLL = 0xC0;

  U0LCR &= 0x7F;											// Disable Programming of Divisor Latches

  U0FCR |= 0x01;											// FIF0 Enable
  U0FCR |= 0x02;											// RX FIFO Reset
  U0FCR |= 0x04;											// TX FIFO Reset
  U0FCR &= 0x3F;                      
}

/****************************/
/* Write Character To UART0 */
/****************************/
int putchar (int ch)  
{                  
  if (ch == '\n')  
  {
    while (!(U0LSR & 0x20));  								// Wait TXD Buffer Empty
    U0THR = 0x0D;                          					// Write CR
  }
  while (!(U0LSR & 0x20));									// Wait TXD Buffer Empty
  return (U0THR = ch);										// Write Character
}

/*****************************/
/* Read Character From UART0 */
/*****************************/
int getchar (void)  
{                    
  while (!(U0LSR & 0x01));									// Wait RXD Receive Data Ready
  return (U0RBR);											// Get Receice Data & Return
}


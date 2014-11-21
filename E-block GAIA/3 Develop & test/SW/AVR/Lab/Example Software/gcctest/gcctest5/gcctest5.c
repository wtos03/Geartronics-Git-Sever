/*
    Title:    AVR-GCC test program #5 for the STK200 eva board
    Author:   Volker Oth
    Date:     4/1999
    Purpose:  read and writes the built in EEprom via the interrupt driven UART.
              The "tranceive complete" interrupt is used to send the contents of
              the EEprom continuously to the PC.
              When a byte was received from the pc, the "receive complete"
              interrupt is called, which outputs the byte to PortB and stores
              it into the EEprom.              
              UART format: 9600 baud, 8bit, 1 stopbit, no parity
    needed
    Software: AVR-GCC to compile
    needed
    Hardware: ATS90S8515/8535/2313/mega(?) on STK200/STK300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#include <io.h>
#include <interrupt.h>
#include <signal.h>
#include <eeprom.h>


#define F_CPU            4000000      /* 4Mhz */
#define UART_BAUD_RATE      9600      /* 9600 baud */


#define EEPROM_SIZE (E2END+1)
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)


typedef unsigned char  u08;
typedef          char  s08;
typedef unsigned short u16;
typedef          short s16;


u16 read_counter;
u16 write_counter;


SIGNAL(SIG_UART_RECV)      /* signal handler for receive complete interrupt */
{
    register u08 ee_write;

    ee_write = inp(UDR);                       /* read byte for UART data buffer */
    outp(~ee_write, PORTB);                    /* output byte to PortB (LEDs) */
    eeprom_wb(write_counter, ee_write);        /* write byte to eeprom */
    if (++write_counter >= EEPROM_SIZE)        /* overflow: restart at offset 0 */
        write_counter = 0;                    
}


SIGNAL(SIG_UART_TRANS)     /* signal handler for tranceive complete interrupt */
{
    register u08 ee_read;

    ee_read = eeprom_rb(read_counter);           /* read next byte from EEprom */
    outp(ee_read, UDR);                          /* write byte to UART data buffer */
    if (++read_counter >= write_counter)         /* overflow: restart with 1st char */
	read_counter = 0;
}


int main(void)
{
    outp(0xff ,DDRB);      /* PortB output */
    outp(0x00, PORTB);     /* switch LEDs on */

    /* enable RxD/TxD and ints */
    outp((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN),UCR);       
    /* set baud rate to 9600 */
    outp(UART_BAUD_SELECT, UBRR);  

    sei();                 /* enable interrupts */

    read_counter  = 0;     /* start reading with 1st byte in EEprom */
    write_counter = 0;     /* start writing with 1st byte in EEprom */
    /* write 1st byte to UART data buffer */
    outp('#', UDR);       

    for (;;) {}            /* loop forever */
}
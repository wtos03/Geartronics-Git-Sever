//
// mcudefs.h
//
// This file contains register, bit and interrupt vector definitions for
// different processor types. chip45boot uses the myXXXX names for USART
// access and interrupt settings.
// Feel free to add more mcu types to the file!
// 
// (C) E.Lins, www.chip45.com
// 18-Sep-2006
//
// Change History
// --------------
// 17-Feb-2007
// - added section for ATmega32 (thanks to Luigi)
//


#ifndef MCUDEFS
#define MCUDEFS


#ifdef __AVR_ATmega8__
#define myIVSELREG GICR
#define myUSART_ReceiveCompleteVect USART_RXC_vect
#define myRXCIE RXCIE
#define myUDR   UDR
#define myUBRRH UBRRH
#define myUBRRL UBRRL
#define myUCSRA UCSRA
#define myUCSRB UCSRB
#define myUCSRC UCSRC
#define myUDRE  UDRE
#define myRXC   RXC
#define myRXEN  RXEN
#define myTXEN  TXEN
#define myUCSZ0 UCSZ0
#define myUCSZ1 UCSZ1
#define myURSEL _BV(URSEL) // bit must be set to one on certain MCU types
#define myCOUNTERTYPE uint8_t
#endif

#ifdef __AVR_ATmega32__
#define myIVSELREG GICR
#define myUSART_ReceiveCompleteVect USART_RXC_vect
#define myRXCIE RXCIE
#define myUDR   UDR
#define myUBRRH UBRRH
#define myUBRRL UBRRL
#define myUCSRA UCSRA
#define myUCSRB UCSRB
#define myUCSRC UCSRC
#define myUDRE  UDRE
#define myRXC   RXC
#define myRXEN  RXEN
#define myTXEN  TXEN
#define myUCSZ0 UCSZ0
#define myUCSZ1 UCSZ1
#define myURSEL _BV(URSEL) // bit must be set to one on certain MCU types
#define myCOUNTERTYPE uint8_t
#endif

#ifdef __AVR_ATmega128__
#define myIVSELREG MCUCR
#define myUSART_ReceiveCompleteVect USART0_RX_vect
#define myRXCIE RXCIE0
#define myUDR   UDR0
#define myUBRRH UBRR0H
#define myUBRRL UBRR0L
#define myUCSRA UCSR0A
#define myUCSRB UCSR0B
#define myUCSRC UCSR0C
#define myUDRE  UDRE0
#define myRXC   RXC0
#define myRXEN  RXEN0
#define myTXEN  TXEN0
#define myUCSZ0 UCSZ00
#define myUCSZ1 UCSZ01
#define myURSEL 0          // MCU type does not require URSEL bit to be set
#define myCOUNTERTYPE uint16_t
#endif

#ifdef __AVR_AT90CAN128__
#define myIVSELREG MCUCR
#define myUSART_ReceiveCompleteVect USART0_RX_vect
#define myRXCIE RXCIE0
#define myUDR   UDR0
#define myUBRRH UBRR0H
#define myUBRRL UBRR0L
#define myUCSRA UCSR0A
#define myUCSRB UCSR0B
#define myUCSRC UCSR0C
#define myUDRE  UDRE0
#define myRXC   RXC0
#define myRXEN  RXEN0
#define myTXEN  TXEN0
#define myUCSZ0 UCSZ00
#define myUCSZ1 UCSZ01
#define myURSEL 0          // MCU type does not require URSEL bit to be set
#define myCOUNTERTYPE uint16_t
#endif

#ifdef __AVR_ATmega168__
#define myIVSELREG MCUCR
#define myUSART_ReceiveCompleteVect USART_RX_vect
#define myRXCIE RXCIE0
#define myUDR   UDR0
#define myUBRRH UBRR0H
#define myUBRRL UBRR0L
#define myUCSRA UCSR0A
#define myUCSRB UCSR0B
#define myUCSRC UCSR0C
#define myUDRE  UDRE0
#define myRXC   RXC0
#define myRXEN  RXEN0
#define myTXEN  TXEN0
#define myUCSZ0 UCSZ00
#define myUCSZ1 UCSZ01
#define myURSEL 0          // MCU type does not require URSEL bit to be set
#define myCOUNTERTYPE uint8_t
#endif

#ifdef __AVR_ATmega644__
#define myUSART_ReceiveCompleteVect USART0_RX_vect
#define myRXCIE RXCIE0
#define myUDR   UDR0
#define myUBRRH UBRR0H
#define myUBRRL UBRR0L
#define myUCSRA UCSR0A
#define myUCSRB UCSR0B
#define myUCSRC UCSR0C
#define myUDRE  UDRE0
#define myRXC   RXC0
#define myRXEN  RXEN0
#define myTXEN  TXEN0
#define myUCSZ0 UCSZ00
#define myUCSZ1 UCSZ01
#define myURSEL 0          // MCU type does not require URSEL bit to be set
#define myCOUNTERTYPE uint8_t
#endif

#endif  // #ifndef MCUDEFS

// end of file mcudefs.h

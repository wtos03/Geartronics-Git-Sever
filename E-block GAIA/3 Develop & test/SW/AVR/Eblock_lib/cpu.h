/* 
 Include file			: cpu_atmega32.h
 Description			: To setup parameters for ATMEGA32. 
						  Every microcontroller will need to include this file for port mapping
						  and other parameters ex. Clock speed. This will be the first files to include
						  
 CPU					:  ATMEGA32						  
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   21/3/2006
 \NOTE 	 Change 0x01, 0x02 to E_PORT0 -- E_PORT7
 
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
*/

#include <avr/io.h>


#define 	CPU_CLOCK_SPEED		16   // 16 Mhz



#define INPUT  	0
#define OUTPUT 	1
#define ON    	1
#define OFF		0

 
// General Port mapping between  E-block port to AVR ATMEGA32 port

//-------  Standard  E block port
#define P0			0x01
#define P1			0x02
#define P2			0x04
#define P3			0x08
#define P4			0x10
#define P5			0x20
#define P6			0x40
#define P7			0x80



#define E_OUT_PORTA			PORTB
#define E_OUT_PORTB			PORTC
#define E_OUT_PORTC			PORTA
#define E_OUT_PORTD			PORTD

#define E_IN_PORTA			PINB
#define E_IN_PORTB			PINC
#define E_IN_PORTC			PINA
#define E_IN_PORTD			PIND

#define E_DIR_PORTA			DDRB
#define E_DIR_PORTB			DDRC
#define E_DIR_PORTC			DDRA
#define E_DIR_PORTD			DDRD

// Need to define each pin  Because each CPU may not map E-block pin directly

//------------------- Top left connector

#define  E_OUT_PA0(x)	  (x > 0) ? (PORTB |=  P0) :  (PORTB  &= ~(P0))
#define  E_OUT_PA1(x)	  (x > 0) ? (PORTB |=  P1) :  (PORTB  &= ~(P1))
#define  E_OUT_PA2(x)	  (x > 0) ? (PORTB |=  P2) :  (PORTB  &= ~(P2))
#define  E_OUT_PA3(x)	  (x > 0) ? (PORTB |=  P3) :  (PORTB  &= ~(P3))
#define  E_OUT_PA4(x)	  (x > 0) ? (PORTB |=  P4) :  (PORTB  &= ~(P4))
#define  E_OUT_PA5(x)	  (x > 0) ? (PORTB |=  P5) :  (PORTB  &= ~(P5))
#define  E_OUT_PA6(x)	  (x > 0) ? (PORTB |=  P6) :  (PORTB  &= ~(P6))
#define  E_OUT_PA7(x)	  (x > 0) ? (PORTB |=  P7) :  (PORTB  &= ~(P7))

// INPUT PORT   > 0 ON   0 = OFF
#define  E_IN_PA0 		  PINB & P0		
#define  E_IN_PA1 		  PINB & P1    	 
#define  E_IN_PA2 		  PINB & P2    	 
#define  E_IN_PA3 		  PINB & P3    	 
#define  E_IN_PA4 		  PINB & P4    	 
#define  E_IN_PA5 		  PINB & P5    	 
#define  E_IN_PA6 		  PINB & P6    	 
#define  E_IN_PA7 		  PINB & P7     

// DIRECTION PORT
#define  E_DIR_PA0(x)	  (x > 0) ? (DDRB |=  P0) :  (DDRB  &= ~(P0))
#define  E_DIR_PA1(x)	  (x > 0) ? (DDRB |=  P1) :  (DDRB  &= ~(P1))
#define  E_DIR_PA2(x)	  (x > 0) ? (DDRB |=  P2) :  (DDRB  &= ~(P2))
#define  E_DIR_PA3(x)	  (x > 0) ? (DDRB |=  P3) :  (DDRB  &= ~(P3))
#define  E_DIR_PA4(x)	  (x > 0) ? (DDRB |=  P4) :  (DDRB  &= ~(P4))
#define  E_DIR_PA5(x)	  (x > 0) ? (DDRB |=  P5) :  (DDRB  &= ~(P5))
#define  E_DIR_PA6(x)	  (x > 0) ? (DDRB |=  P6) :  (DDRB  &= ~(P6)) 
#define  E_DIR_PA7(x)	  (x > 0) ? (DDRB |=  P7) :  (DDRB  &= ~(P7))


//------------------  Bottom left connector
#define E_OUT_TIMER0(x)		(x > 0) ? (PORTD |=  P7) :  (PORTD  &= ~(P7))
#define E_OUT_INT1(x)		(x > 0) ? (PORTD |=  P3) :  (PORTD  &= ~(P3))
#define E_OUT_PB0(x)		(x > 0) ? (PORTC |=  P2) :  (PORTC  &= ~(P2))
#define E_OUT_PB1(x)		(x > 0) ? (PORTC |=  P3) :  (PORTC  &= ~(P3))
#define E_OUT_PB2(x)		(x > 0) ? (PORTC |=  P4) :  (PORTC  &= ~(P4))
#define E_OUT_PB3(x)		(x > 0) ? (PORTC |=  P5) :  (PORTC  &= ~(P5))
#define E_OUT_PB4(x)		(x > 0) ? (PORTC |=  P6) :  (PORTC  &= ~(P6))
#define E_OUT_PB5(x)		(x > 0) ? (PORTC |=  P7) :  (PORTC  &= ~(P7))
#define E_OUT_PB6(x)		(x > 0) ? (PORTD |=  P7) :  (PORTD  &= ~(P7))
#define E_OUT_PB7(x)		(x > 0) ? (PORTD |=  P6) :  (PORTD  &= ~(P6))

#define E_DIR_TIMER0(x)		(x > 0) ? (DDRD |=  P7) :  (DDRD  &= ~(P7))
#define E_DIR_INT1(x)		(x > 0) ? (DDRD |=  P3) :  (DDRD  &= ~(P3))
#define E_DIR_PB0(x)		(x > 0) ? (DDRC |=  P2) :  (DDRC  &= ~(P2))
#define E_DIR_PB1(x)		(x > 0) ? (DDRC |=  P3) :  (DDRC  &= ~(P3))
#define E_DIR_PB2(x)		(x > 0) ? (DDRC |=  P4) :  (DDRC  &= ~(P4))
#define E_DIR_PB3(x)		(x > 0) ? (DDRC |=  P5) :  (DDRC  &= ~(P5))
#define E_DIR_PB4(x)		(x > 0) ? (DDRC |=  P6) :  (DDRC  &= ~(P6))
#define E_DIR_PB5(x)		(x > 0) ? (DDRC |=  P7) :  (DDRC  &= ~(P7))
#define E_DIR_PB6(x)		(x > 0) ? (DDRD |=  P7) :  (DDRD  &= ~(P7))
#define E_DIR_PB7(x)		(x > 0) ? (DDRD |=  P6) :  (DDRD  &= ~(P6))

#define E_IN_TIMER0     	(PIND & P7)
#define E_IN_INT1			(PIND & P3)
#define E_IN_PB0			(PINC & P2)
#define E_IN_PB1			(PINC & P3)
#define E_IN_PB2			(PINC & P4)
#define E_IN_PB3			(PINC & P5)
#define E_IN_PB4			(PINC & P6)
#define E_IN_PB5			(PINC & P7)
#define E_IN_PB6			(PIND & P7)
#define E_IN_PB7			(PIND & P6)



//!------------------- Top right connector
#define E_OUT_SCL(x)		(x > 0) ? (PORTC |=  P0) :  (PORTC  &= ~(P0))
#define E_OUT_SDA(x)		(x > 0) ? (PORTC |=  P1) :  (PORTC  &= ~(P1))
#define E_OUT_RXD(x)		(x > 0) ? (PORTD |=  P0) :  (PORTD  &= ~(P0))
#define E_OUT_TXD(x)		(x > 0) ? (PORTD |=  P1) :  (PORTD  &= ~(P1))
#define E_OUT_INT0(x)		(x > 0) ? (PORTD |=  P2) :  (PORTD  &= ~(P2))
#define E_OUT_ICP(x)		(x > 0) ? (PORTD |=  P6) :  (PORTD  &= ~(P6))
#define E_OUT_TIMER1(x)		(x > 0) ? (PORTD |=  P5) :  (PORTD  &= ~(P5))
#define E_OUT_TIMER2(x)		(x > 0) ? (PORTD |=  P4) :  (PORTD  &= ~(P4))
// iNPUT
#define E_IN_SCL			(PINc & P0)
#define E_IN_SDA			(PINc & P1)
#define E_IN_RXD			(PIND & P0)
#define E_IN_TXD			(PIND & P1)
#define E_IN_INT0			(PIND & P2)
#define E_IN_ICP			(PIND & P6)
#define E_IN_TIMER1			(PIND & P5)
#define E_IN_TIMER2			(PIND & P4)
// Direction
#define E_DIR_SCL(x)		(x > 0) ? (DDRC |=  P0) :  (DDRC  &= ~(P0))
#define E_DIR_SDA(x)		(x > 0) ? (DDRC |=  P1) :  (DDRC  &= ~(P1))
#define E_DIR_RXD(x)		(x > 0) ? (DDRD |=  P0) :  (DDRD  &= ~(P0))
#define E_DIR_TXD(x)		(x > 0) ? (DDRD |=  P1) :  (DDRD  &= ~(P1))
#define E_DIR_INT0(x)		(x > 0) ? (DDRD |=  P2) :  (DDRD  &= ~(P2))
#define E_DIR_ICP(x)		(x > 0) ? (DDRD |=  P6) :  (DDRD  &= ~(P6))
#define E_DIR_TIMER1(x)		(x > 0) ? (DDRD |=  P5) :  (DDRD  &= ~(P5))
#define E_DIR_TIMER2(x)		(x > 0) ? (DDRD |=  P4) :  (DDRD  &= ~(P4))

//-------------------- Bottom right connector
#define E_IN_ADC0			(PINA & P0)
#define E_IN_ADC1			(PINA & P1)
#define E_IN_ADC2			(PINA & P2)
#define E_IN_ADC3			(PINA & P3)
#define E_IN_ADC4			(PINA & P4)
#define E_IN_ADC5			(PINA & P5)
#define E_IN_ADC6			(PINA & P6)
#define E_IN_ADC7			(PINA & P7)
// OUTPUT
#define E_OUT_ADC0(x)		(x > 0) ? (PORTA |=  P0) :  (PORTA  &= ~(P0))
#define E_OUT_ADC1(x)		(x > 0) ? (PORTA |=  P1) :  (PORTA  &= ~(P1))
#define E_OUT_ADC2(x)		(x > 0) ? (PORTA |=  P2) :  (PORTA  &= ~(P2))
#define E_OUT_ADC3(x)		(x > 0) ? (PORTA |=  P3) :  (PORTA  &= ~(P3))
#define E_OUT_ADC4(x)		(x > 0) ? (PORTA |=  P4) :  (PORTA  &= ~(P4))
#define E_OUT_ADC5(x)		(x > 0) ? (PORTA |=  P5) :  (PORTA  &= ~(P5))
#define E_OUT_ADC6(x)		(x > 0) ? (PORTA |=  P6) :  (PORTA  &= ~(P6))
#define E_OUT_ADC7(x)		(x > 0) ? (PORTA |=  P7) :  (PORTA  &= ~(P7))
// DIRECTION
#define E_DIR_ADC0(x)		(x > 0) ? (DDRA |=  P0)  :  (DDRA  &= ~(P0))
#define E_DIR_ADC1(x)		(x > 0) ? (DDRA |=  P1)  :  (DDRA  &= ~(P1))
#define E_DIR_ADC2(x)		(x > 0) ? (DDRA |=  P2)  :  (DDRA  &= ~(P2))
#define E_DIR_ADC3(x)		(x > 0) ? (DDRA |=  P3)  :  (DDRA  &= ~(P3))
#define E_DIR_ADC4(x)		(x > 0) ? (DDRA |=  P4)  :  (DDRA  &= ~(P4))
#define E_DIR_ADC5(x)		(x > 0) ? (DDRA |=  P5)  :  (DDRA  &= ~(P5))
#define E_DIR_ADC6(x)		(x > 0) ? (DDRA |=  P6)  :  (DDRA  &= ~(P6))
#define E_DIR_ADC7(x)		(x > 0) ? (DDRA |=  P7)  :  (DDRA  &= ~(P7))

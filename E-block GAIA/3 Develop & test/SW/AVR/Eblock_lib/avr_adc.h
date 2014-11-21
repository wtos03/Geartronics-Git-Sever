/*!   \file  	avr_adc		 
      \brief    Analog to Digital Convertor module
	  \n	     Convert analog to digital 
	  \n         There are two mode Polling mode and interrupt mode
      \defgroup  avr_adc
      \ingroup   Eblock_lib
	\nSummary for include files (X = have  - = NO)
	\n --------------------------------------------
	\n Constant variable						X   
	\n Function prototype						-
	\n Variable (Keep in Flash Program memory)	-
	\n Memory usage					?? bytes
       \code #include <avr_adc.h> \endcode
 
      \version	0.1
      \author 	wichai@geartronics.net	
      \date	    16/09/08
      \n \b Update \b History
 
	\n Date                 By                     Comments
	\n ----                 --                     ---------
    \n 16/09/08				Wichai				Initial version
 
 
	\note   
 
*/
 
 

/* External include file

*/
#include "cpu.h"


 
//! Constant variable, For comment in doxygen before the variable declaration ???? 

#define AREF_REF  		0x00     // See ADMUX register for detail
#define AVCC_REF  		0x40
#define INTERNAL_REF 	0xC0
#define ENABLE          1
#define DISABLE         0

//! ADC Channel
#define ADC_0          0
#define ADC_1          1
#define ADC_2          2
#define ADC_3          3
#define ADC_4          4
#define ADC_5          5
#define ADC_6          6
#define ADC_7          7

//! Different with gain
#define  DIFF_0_0_10X		0x08				//  Diff gain 10x between ADC0 and ADC0
#define  DIFF_1_0_10X		0x09				//  Diff gain 10x between ADC1 and ADC0
#define  DIFF_0_0_200X      0x0A
#define  DIFF_1_0_200X      0x0B
#define  DIFF_2_2_10X       0x0C
#define  DIFF_3_2_10X       0x0D
#define  DIFF_2_2_200X      0x0E
#define  DIFF_3_2_200X      0x0F

#define  DIFF_0_1_1X        0x10
#define  DIFF_1_1_1X        0x11
#define  DIFF_2_1_1X        0x12
#define  DIFF_3_1_1X        0x13
#define  DIFF_4_1_1X        0x14
#define  DIFF_5_1_1X        0x15
#define  DIFF_6_1_1X        0x16
#define  DIFF_7_1_1X        0x17
#define  DIFF_0_2_1X        0x18
#define  DIFF_1_2_1X        0x19
#define  DIFF_2_2_1X        0x1A
#define  DIFF_3_2_1X        0x1B
#define  DIFF_4_2_1X        0x1C
#define  DIFF_5_2_1X        0x1D









/*!
ADCSRA 
	   Bit			Name			Purpose
		7			ADEN			ADC Enable bit
		6			ADSC			ADC Start conversion bit
		5			ADATE			ADC Auto trigger Enable (Advance need to set ADTS in SFIOR
		4			ADIF			ADC Interrupt Flag
		
		3			ADIE			ADC Interrupt Mask bit
		2- 0       ADPS0-2			ADC Prescaler division factor select bits
                    Use 111 for 128 for 16 Mhz   Freq should be 50 KHz - 200 Khz
					More division convert slower
					Less division convert faster
ADMUX 
		Bit 5   left or right adjust bit in ADLAR

*/



#define   SINGLE_CONVERSION_DIV_16      0x83     // Single conversion and divide clock by 16
#define   AUTO_TRIGGER_DIV_16        	0xA3     // Auto triggle non interrupt divide by 16


 
 


//! Function prototypes
//! Initialize adc mode 
void adc_init(uint8_t, uint8_t, uint8_t);   
// Read adc value 
uint16_t adc_read(uint8_t);
// !Read value from interrupt
uint16_t adc_int_read(uint8_t);				 



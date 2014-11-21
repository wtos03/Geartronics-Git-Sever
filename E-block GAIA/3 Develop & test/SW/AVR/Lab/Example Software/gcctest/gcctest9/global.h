#ifndef GLOBAL_H
#define GLOBAL_H

#define MAX_U16  65535
#define MAX_S16  32767

#define IN       0
#define OUT      1

#define LOW      0
#define HIGH     1

#define FALLING  0
#define RISING   1

typedef unsigned char  u08;
typedef          char  s08;
typedef unsigned short u16;
typedef          short s16;
typedef u08            bool;

#define DDR(x) ((x)-1)    /* address of data direction register of port x */
#define PIN(x) ((x)-2)    /* address of input register of port x */


#ifdef AVR_ATmega103
  #define AVR_MEGA 1
#else
  #ifdef AVR_ATmega603
    #define AVR_MEGA 2
  #else
    #ifdef AVR_ATmega161
      #define AVR_MEGA 3
    #else
      #define AVR_MEGA 0
    #endif
  #endif
#endif

#endif
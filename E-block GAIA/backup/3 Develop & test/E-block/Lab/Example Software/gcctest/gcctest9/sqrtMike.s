;****************************************************************************
;* 
;* File name: sqrtMike.s
;* Author: Mike Dvorsky
;* Date: 29 April 2000
;*
;* 16-bit integer square root for AVR.  Uses a shift-and-subtract
;* binary search algorithm to compute square root.  Equivalent C code is:
;*
;* unsigned char sqrt16(unsigned int n)
;* {
;*    unsigned int result,      /* Partial solution shifted */
;*                 pwr2,        /* Power of 2 */
;*                 err,         /* Error in partial solution = n - result^2 */
;*                 tmp;         /* Intermediate value */
;*
;*    result = 0;
;*    err = n;
;*    for (pwr2 = 0x4000; pwr2 != 0; pwr2 >>= 2)
;*    {
;*       /* Loops 8 times, once for each result bit starting with msb */
;*       tmp = result + pwr2;
;*       if (tmp <= err)        /* Should this bit be 1? */
;*       {
;*          err -= tmp;         /* Yes it should, make it so */
;*          result = tmp + pwr2;
;*       }
;*       result >>= 1;          /* Shift result into position and move to next bit */
;*    }
;*
;*    return result;
;* }
;****************************************************************************

#include <macros.inc>
#include <ctoasm.inc>

/* Error, initially equal to argument */
#define err_low  rP1
#define err_high rP0

/* Partial result */
#define res_low  R26
#define res_high R27

/* Temporary intermediate value */
#define tmp_low  R30
#define tmp_high R31

/* Power of 2 */
#define p2_low   R22
#define p2_high  R23


        .global sqrtMike
        .func sqrtMike

sqrtMike:
        ldi     res_low,  0x00          ;result = 0
        ldi     res_high, 0x00
        ldi     p2_low,   0x00          ;pwr2 = 0x4000
        ldi     p2_high,  0x40

next_bit:
        mov     tmp_low, res_low        ;tmp = result + pwr2
        mov     tmp_high, res_high
        add     tmp_low, p2_low
        adc     tmp_high, p2_high

        cp      err_low, tmp_low        ;tmp <= err?
        cpc     err_high, tmp_high
        brlo    prep_for_next_bit

        ;This bit should be 1, store in result and adjust err
        sub     err_low, tmp_low        ;err -= tmp
        sbc     err_high, tmp_high

        mov     res_low, tmp_low        ;res = tmp + pwr2
        mov     res_high, tmp_high
        add     res_low, p2_low
        adc     res_high, p2_high

prep_for_next_bit:
        lsr     res_high                ;res >>= 1
        ror     res_low

        lsr     p2_high                 ;pwr2 >>= 2
        ror     p2_low
        lsr     p2_high
        ror     p2_low

        mov     tmp_low, p2_low         ;loop if pwr2 != 0
        or      tmp_low, p2_high
        brne    next_bit

        mov     rByte, res_low          ;Move return value to proper registers
        ;mov     rP0, res_high          ;not needed for sqrt of 16bit
        
        ret

        .endfunc

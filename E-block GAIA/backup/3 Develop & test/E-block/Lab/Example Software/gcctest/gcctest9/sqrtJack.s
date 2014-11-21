/*
Smallest code AVR Sqrt
June 1999, by Jack Tidwell
Calculates the sqrt by subtracting an ODD number that self increments (by 2)
for each iteration. If the new ODD 'subber' is LESS than the previous
results, increment our root by 1, and loop again.
Worst case is about 200us at 8mhz and less than 15us for 8 bit.
Example sqrt(100)

num     'odd_suber'     sqrt
100     1               1
99      3               2
96      5               3
91      7               4
84      9               5
75      11              6
64      13              7
51      15              8
36      17              9
19      19              10

so the sqrt(100) = 10

enter with the 16 bit Number in rP0, rP1

unsigned char isqrt( unsigned short num);
*/

#include <macros.inc>
#include <ctoasm.inc>

#define num_lo   rP1
#define num_hi   rP0   /* should be the other way round ? */
#define suber_lo r28
#define suber_hi r29
#define sqrt     r16

	.global sqrtJack
	.func sqrtJack

sqrtJack:
	push	r16
	push	r28
	push	r29

    clr     sqrt
    ldi     suber_lo,1	; initialize the seed to be subtracted
    clr     suber_hi	;	for each iteration
loop:   
    sub     num_lo,suber_lo
    sbc     num_hi,suber_hi
	brlo	exit
    inc     sqrt
    adiw    suber_lo,2	; keep the number to subtract ODD.
    rjmp    loop
exit:   
    mov rByte, sqrt

	pop	r29
	pop	r28
	pop	r16
	ret

	.endfunc

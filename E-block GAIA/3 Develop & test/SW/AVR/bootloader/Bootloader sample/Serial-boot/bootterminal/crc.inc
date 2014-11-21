;***************************************************************************
;* INCLUDE
;*	CRC
;* DESCRIPTION
;*	Routines for calculating/verifying Cyclic Redundancy Checksums
;*************************************************************************

.equ	crc_polynomial = 0x1021
.equ	crc_init_value = 0x0000

;************************************************************************* 
;* FUNCTION
;*	CRC16
;* DESCRIPTION 
;*	Calculate 16 bit CCITT CRC
;* PARAMETERS
;*	w1  = Number of bytes to include in calculation
;*	Z   = Pointer to buffer with data
;* RETURNS
;*	X   = Calculated 16 bit CRC
;*	Z   = Pointer to first byte after buffer
;************************************************************************* 
CRC16_XZ:
	push	w1
	push	w2
	push	YL
	push	YH
	ldi	XL,low(crc_init_value)
	ldi	XH,high(crc_init_value)
	ldi	YL,low(crc_polynomial)
	ldi	YH,high(crc_polynomial)
crc16_byteloop:
	ld	w2,Z+
	eor	XH,w2
	ldi	w2,8
crc16_bitloop:
	lsl	XL
	rol	XH
	brcc	crc16_nextbit
	eor	XL,YL
	eor	XH,YH
crc16_nextbit:
	dec	w2
	brne	crc16_bitloop
	dec	w1
	brne	crc16_byteloop
	pop	YH
	pop	YL
	pop	w2
	pop	w1
	ret

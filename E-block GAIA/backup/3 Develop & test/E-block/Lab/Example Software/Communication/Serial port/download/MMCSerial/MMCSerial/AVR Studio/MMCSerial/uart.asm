;-----------------------------------------------------------------
;
; Name:		uart.asm
; Title:	AVR Uart services
; Version:	1.0
; Last updated:	2001.02.03
; Target:	AT90Sxxxx (All devices)
;
;-----------------------------------------------------------------
;
; DESCRIPTION
;
; UART service which uses interrupts to read characters
; from the serial port and buffer them, but which handles trans-
; mission synchronously, without interrupts.
; 
;-----------------------------------------------------------------

.dseg

uart_rhead:	.byte	1
uart_rtail:	.byte	1
uart_rsize:	.byte	1
uart_rerror:	.byte	1

.equ	UART_BUFSIZE	=$40	; must be power of 2
.equ	UART_BUFFER	=$80	; must be on a UART_BUFSIZE boundary

.equ	UART_BAUDRATE	=115200
.equ	UART_BAUDK	=(CLOCK/(16*UART_BAUDRATE))-1

;-----------------------------------------------------------------

.cseg

;-----------------------------------------------------------------
;
; Reset vector - setup

uart_reset:
	ldi	rTEMP,UART_BUFFER
	sts	uart_rhead,rTEMP
	sts	uart_rtail,rTEMP
	
	clr	rTEMP
	sts	uart_rsize,rTEMP
	sts	uart_rerror,rTEMP

	ldi	rTEMP,UART_BAUDK
	out	UBRR,rTEMP	; load baudrate
	
	ldi	rTEMP,(1<<TXEN)|(1<<RXEN)|(1<<RXCIE)	; enable transmit/receive, enable rx int.
	out	UCR,rTEMP

	ret

;-----------------------------------------------------------------
;
; Receive vector - buffer input

uart_receive:
	in	rISREGSAVE,SREG
	mov	rISCRATCH4,XL

	sbic	USR,FE		; check for
	rjmp	uart_rxerr_fe	; framing error

	lds	rITEMP,uart_rsize
	cpi	rITEMP,UART_BUFSIZE
	breq	uart_rxerr_bf	; buffer full

	inc	rITEMP
	sts	uart_rsize,rITEMP

	lds	XL,uart_rhead
	in	rITEMP,UDR
	st	X+,rITEMP

	andi	XL,(UART_BUFSIZE-1)
	ori	XL,UART_BUFFER
	sts	uart_rhead,XL

	sbic	USR,OR		; check for
	rjmp	uart_rxerr_or	; overrun

uart_rxend:
	mov	XL,rISCRATCH4
	out	SREG,rISREGSAVE
	reti

uart_rxerr_fe:
uart_rxerr_bf:
	in	rITEMP,UDR		; dummy read to clear RxC
uart_rxerr_or:
	ldi	rITEMP,1		; overflow, framing error
	sts	uart_rerror,rITEMP

	rjmp	uart_rxend

;-----------------------------------------------------------------
;
; Read from receiver buffer char=rRETURN, wait until character ready

uart_readcharwait:
	rcall	uart_charready
	breq	uart_readcharwait

uart_readchar:
	push	XL
	
	lds	XL,uart_rtail
	ld	rRETURN,x+
	andi	XL,(UART_BUFSIZE-1)
	ori	XL,UART_BUFFER
	sts	uart_rtail,XL

	cli
	lds	rSCRATCH,uart_rsize
	dec	rSCRATCH
	sts	uart_rsize,rSCRATCH
	sei

	pop	XL

	ret
	
;-----------------------------------------------------------------
;
; Ask if a character is ready from the UART (eq=no, ne=yes)

uart_charready:
	lds	rSCRATCH,uart_rsize
	and	rSCRATCH,rSCRATCH
	ret	

;-----------------------------------------------------------------
;
; Write to transmitter a string followed by a cr/lf
;
; - - - UNUSED - - - - - - - - - - - - - - - - - - - - - - - - - -
;
;uart_writestringlf:
;	rcall	uart_writestring
;
;	; falls through to uart_writecrlf...
;
; - - - UNUSED - - - - - - - - - - - - - - - - - - - - - - - - - -

;-----------------------------------------------------------------
;
; Write to transmitter a cr/lf pair
;
; - - - UNUSED - - - - - - - - - - - - - - - - - - - - - - - - - -
;
;uart_writecrlf:
;	ldi	rPARAM1,$0D
;	rcall	uart_writechar
;	ldi	rPARAM1,$0A
;
;	; falls through to uart_writechar again...
;
;
; - - - UNUSED - - - - - - - - - - - - - - - - - - - - - - - - - -

;-----------------------------------------------------------------
;
; Write to transmitter char=rPARAM1

uart_writechar:
	sbis	USR,UDRE
	rjmp	uart_writechar
	out	UDR,rPARAM1
	ret

;-----------------------------------------------------------------
;
; Write to transmitter hex value=rPARAM1

uart_writehex:
	push	rPARAM1
	swap	rPARAM1
	rcall	uart_writehexit
	pop	rPARAM1
	
uart_writehexit:
	andi	rPARAM1,$0F
	subi	rPARAM1,-48
	cpi	rPARAM1,$3A
	brcs	uart_writechar
	subi	rPARAM1,-7
	rjmp	uart_writechar

;-----------------------------------------------------------------
;
; Write 0-terminated string in Flash at Z to transmitter
;
; - - - UNUSED - - - - - - - - - - - - - - - - - - - - - - - - - -
;
;uart_writestring:
;	lpm				; r0,Z
;	adiw	ZL,1
;	mov	rPARAM1,r0
;	and	rPARAM1,rPARAM1
;	breq	uart_writestringend
;
;	rcall	uart_writechar
;
;	rjmp	uart_writestring
;
;uart_writestringend:
;	ret				; NB: Shared with uart_writestring
;
; - - - UNUSED - - - - - - - - - - - - - - - - - - - - - - - - - -
	
;-----------------------------------------------------------------
;
; Write 0-terminated string in RAM at Z to transmitter, with crlf
; if reqd.
;
; - - - UNUSED - - - - - - - - - - - - - - - - - - - - - - - - - -
;
;uart_writestringram:
;	clr	zh
;uart_writestringramloop:
;	ld	rPARAM1,z+
;	and	rPARAM1,rPARAM1
;	breq	uart_writestringend
;
;	rcall	uart_writechar
;
;	rjmp	uart_writestringramloop
;
;uart_writestringramlf:
;	rcall	uart_writestringram
;	rjmp	uart_writecrlf
;
; - - - UNUSED - - - - - - - - - - - - - - - - - - - - - - - - - -

;-----------------------------------------------------------------
;
; Constant Data

;-----------------------------------------------------------------

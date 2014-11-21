;***** Bootloader using Xmodem. For Mega16 running at 16 MHz

.include "m16def.inc"

.equ	UARTBaudRateReg	=103	; = 9600 baud at 16 MHz (51=19200, 25=38400)
.equ	TC1Reload	=65536-15625	; = 1 second at Ck/1024 prescaler

.equ	Xmodem_SOH	=0x01
.equ	Xmodem_EOT	=0x04
.equ	Xmodem_ACK	=0x06
.equ	Xmodem_NAK	=0x15
.equ	Xmodem_CAN	=0x18
.equ	Xmodem_C	=0x43


; r0-r1 reserved for mul results
.def	w1	=r16	; General purpose working register
.def	w2	=r17	; General purpose working register
.def	w3	=r18	; General purpose working register
.def	w4	=r19	; General purpose working register
.def	Retries	=r20	; Protocol retry counter
.def	ExpSeq	=r21	; Expected next sequence number
.def	RetryChar=r22
; r26-31 reserved to X, Y and Z registers
; X  Receivebufferpointer


.dseg
XmodemBuffer:	;Equals next byte
XmodemBlk:	.byte 1
XmodemBlkCom:	.byte 1
XmodemData:	.byte 128
XmodemCRC:	.byte 2


.cseg

.org	0
	jmp	RESET
	
.org	0x1F00
	rjmp	RESET     ; Reset Handler


.include "crc.inc"


;**** Bootloader initialization
RESET:
	ldi	w1,low(RAMEND)
	out	SPL,w1			;Init stackpointer
	ldi	w1,high(RAMEND)
	out	SPH,w1
	
	clr	w1			;Init Timer/Counter 1
	out	TCCR1A,w1
	ldi	w1,(5<<CS10)
	out	TCCR1B,w1
	
	ldi	w1,high(UARTBaudRateReg);Init USART
	out	UBRRH,w1
	ldi	w1,low(UARTBaudRateReg)
	out	UBRRL,w1
	clr	w1
	out	UCSRA,w1
	ldi	w1,(1<<RXEN)|(1<<TXEN)
	out	UCSRB,w1
	ldi	w1,(1<<URSEL)|(3<<UCSZ0)
	out	UCSRC,w1
	
	ldi	w1,Xmodem_C
	out	UDR,w1		;TX Xmodem ready to receive (CRC)
	rcall	StartTimer
	ldi	Retries,2
	ldi	RetryChar,Xmodem_C
	ldi	ExpSeq,1
	rjmp	StateIdle
	
	

StateIdle:
	sbic	UCSRA,RXC
	rjmp	SI_Receive
	rcall	Timeout
	brne	StateIdle
	dec	Retries
	brne	SI_MoreRetries
	rjmp	Boot
SI_MoreRetries:
	out	UDR,RetryChar	;TX Xmodem NAK
	rcall	StartTimer
	rjmp	StateIdle
SI_Receive:
	in	w1,UDR
	cpi	w1,Xmodem_SOH
	breq	SI_SOH
	cpi	w1,Xmodem_EOT
	brne	StateIdle
	ldi	w1,Xmodem_ACK
	out	UDR,w1		;TX Xmodem ACK
	rcall	StartTimer
BootDelayLoop:
	rcall	Timeout
	brne	BootDelayLoop
	rjmp	Boot
SI_SOH:
	cpi	RetryChar,Xmodem_C
	brne	SI_NotStart
	ldi	RetryChar,Xmodem_NAK
	ldi	Retries,10
SI_NotStart:
	ldi	XL,low(XmodemBuffer)
	ldi	XH,high(XmodemBuffer)
	rcall	StartTimer
	rjmp	StateRX
	


StateRX:
	sbic	UCSRA,RXC
	rjmp	SRX_Receive
	rcall	Timeout
	brne	StateRX
SRX_NakIdle:
	ldi	w1,Xmodem_NAK
	out	UDR,w1		;TX Xmodem NAK
	rcall	StartTimer
	rjmp	StateIdle
SRX_Receive:
	rcall	StartTimer
	in	w1,UDR
	st	X+,w1
	cpi	XL,low(XmodemCRC+2)
	brne	StateRX
	ldi	w1,130
	ldi	ZL,low(XmodemData)
	ldi	ZH,high(XmodemData)
	rcall	CRC16_XZ
	or	XL,XH
	brne	SRX_NakIdle	; CRC error
	lds	w1,XmodemBlk
	cp	w1,ExpSeq
	breq	FlashPage
	inc	w1
	cpse	w1,ExpSeq
	rjmp	SRX_Abort
	ldi	w1,Xmodem_ACK	; Received previous frame
	out	UDR,w1		; TX Xmodem ACK
	rcall	StartTimer
	rjmp	StateIdle
SRX_Abort:
	ldi	w1,Xmodem_CAN
	out	UDR,w1		; TX Xmodem CAN
	rjmp	Boot


FlashPage:
	clr	ZL
	lds	ZH,XmodemBlk
	dec	ZH
	lsr	ZH
	ror	ZL
	movw	YH:YL,ZH:ZL
	ldi	w1,(1<<PGERS)|(1<<SPMEN)
	out	SPMCR,w1
	spm			; Flash page erase
FP_WaitForErase:
	in	w1,SPMCR
	sbrc	w1,SPMEN
	rjmp	FP_WaitForErase

	ldi	w1,(1<<SPMEN)
	ldi	w2,64
	ldi	XL,low(XmodemData)
	ldi	XH,high(XmodemData)
FP_WriteLoop:
	ld	r0,X+
	ld	r1,X+
	out	SPMCR,w1
	spm			; Temp storage write
	adiw	ZH:ZL,2
	dec	w2
	brne	FP_WriteLoop

	movw	ZH:ZL,YH:YL
	ldi	w1,(1<<PGWRT)|(1<<SPMEN)
	out	SPMCR,w1
	spm			; Flash page write
FP_WaitForWrite:
	in	w1,SPMCR
	sbrc	w1,SPMEN
	rjmp	FP_WaitForWrite

	;Finished
	inc	ExpSeq
	ldi	w1,Xmodem_ACK	; Received previous frame
	out	UDR,w1		; TX Xmodem ACK
	rcall	StartTimer
	rjmp	StateIdle



StartTimer:
	push	w1
	ldi	w1,high(TC1Reload)
	out	TCNT1H,w1
	ldi	w1,low(TC1Reload)
	out	TCNT1L,w1
	ldi	w1,(1<<TOV1)
	out	TIFR,w1		; Clear overflow flag
	pop	w1
	ret


Timeout:
	push	w1
	push	w2
	in	w1,TIFR
	ldi	w2,(1<<TOV1)
	and	w1,w2
	eor	w1,w2
	pop	w2
	pop	w1
	ret


Boot:
	ldi	w1,(1<<RWWSRE)|(1<<SPMEN)
	out	SPMCR,w1
	spm			; Flash read enable
	clr	ZL
	clr	ZH
	ijmp


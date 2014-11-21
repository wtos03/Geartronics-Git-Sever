;-----------------------------------------------------------------
;
; Name:		MMCSerial.asm
; Title:	Adapter to communicate via RS232 with Multimedia Card.
; Version:	1.0
; Last updated:	2001.08.26
; Target:	AT90S2313
;
;-----------------------------------------------------------------
;
; DESCRIPTION
;
; Application to interface a Multimedia Card with RS232.
;
; The card is operated in SPI mode, with the signals being attached
; to pins on Port B as follows:
;
; Pin B0 is used as the chip select for the card.
; Pin B1 is the data output to the card (=MOSI)
; Pin B2 is the data input from the card (=MISO)
; Pin B3 is the clock to the card (=SCK)
;
; Three status LEDs are wired to pins PB4, PB5, PB6.
; 
; Communication with the host is via RS232 at 115,200 baud (8N1).
; The RTS and CTS lines are attached to pins D2 and D3 respectively,
; and the transmitter enable pin to pin D4.
;
; Clock speed is 3.6864 MHz, as required to get an accurate baud
; rate of 115200 baud.
;
;-----------------------------------------------------------------

.nolist
.include "2313def.inc"
.list

;-----------------------------------------------------------------
;
; General purpose registers

.def	rRETURN		=r0	; return value from functions

.def	rZERO		=r1

.def	rLENGTH0	=r2	; 32 bit counter
.def	rLENGTH1	=r3
.def	rLENGTH2	=r4
.def	rLENGTH3	=r5

.def	rSCRATCH	=r7
.def	rSCRATCH2	=r8
.def	rSCRATCH3	=r9
.def	rSCRATCH4	=r10

.def	rISCRATCH	=r11	; for use in interrupt routines
.def	rISCRATCH2	=r12	; ditto
.def	rISCRATCH3	=r13	; ditto
.def	rISCRATCH4	=r14	; ditto

.def	rISREGSAVE	=r15	; sreg saved here in interrupts

.def	rTEMP		=r16
.def	rTEMP2		=r17

.def	rITEMP		=r18	; for use in interrupt routines
.def	rITEMP2		=r19	; ditto

.def	rPARAM1		=r20
.def	rPARAM2		=r21

.def	rLOOPI		=r22	; loop counters
.def	rLOOPJ		=r23

;-----------------------------------------------------------------
;
; Equates

;-----------------------------------------------------------------
;
; IO Port Bits
;
; Port B

.equ	bCS		=0
.equ	bMOSI		=1
.equ	bMISO		=2
.equ	bSCK		=3
.equ	bLEDWRITE	=4		; Writing/erasing
.equ	bLEDREAD	=5		; Reading
.equ	bLEDACTIVE	=6		; Active and awake
.equ	bLEDAUX		=7		; Aux (debug)

.equ	mCS		=(1<<bCS)
.equ	mMOSI		=(1<<bMOSI)
.equ	mMISO		=(1<<bMISO)
.equ	mSCK		=(1<<bSCK)
.equ	mLEDWRITE	=(1<<bLEDWRITE)
.equ	mLEDREAD	=(1<<bLEDREAD)
.equ	mLEDACTIVE	=(1<<bLEDACTIVE)
.equ	mLEDAUX		=(1<<bLEDAUX)

;
; Port D
;

.equ	bRTS		=2
.equ	bCTS		=3
.equ	bSERIALENABLE	=4
.equ	mRTS		=(1<<bRTS)
.equ	mCTS		=(1<<bCTS)
.equ	mSERIALENABLE	=(1<<bSERIALENABLE)

;
; Common response values
;

.equ	Response_OK	=0x10
.equ	Response_Fail	=0x11
.equ	Response_Unk	=0x12
.equ	Response_Awake	=0x13
.equ	Response_Wait	=0x14
.equ	Response_Data	=0x15

;-----------------------------------------------------------------
;
; Timings

.equ	CLOCK		=3686400	;3.6864 MHz

;-----------------------------------------------------------------
;
; Macros

; This is a slow version of SCKDELAY for debugging purposes
; Each clock cycle lasts around 130ms
;
;.macro	SCKDELAY
;	in	rITEMP2,PORTB
;	andi	rITEMP2,0xF0
;
;	clr	XL
;	clr	XH
;
;	in	rITEMP,PINB
;	com	rITEMP
;	andi	rITEMP,0xF0
;	swap	rITEMP
;	or	rITEMP,rITEMP2
;
;	out	PORTB,rITEMP
;
;	rcall	WaitMicro
;.endmacro

.macro	SCKDELAY
	; No delay required - the card can run faster than us!
.endmacro

;-----------------------------------------------------------------
;
; Variables

.dseg

Debug:	.byte	1		; Bit 0:0 = raw, 1 = human-readable debug mode (hex)

;-----------------------------------------------------------------
;
; Interrupt service vectors

.cseg
.org 0
	rjmp 	Reset		; Reset vector
	
.org INT0addr
	reti			; Used to wake up the MCU

.org INT1addr
	reti			; Not used, but better to be safe than sorry!

.org URXCaddr
	rjmp	uart_receive
	

;-----------------------------------------------------------------
;
; Start of actual code.
;

.org STDORG

;-----------------------------------------------------------------
;
; Include modules

.include "uart.asm"

;-----------------------------------------------------------------
;
; Reset vector - generic system.

Reset:
	ldi	rTEMP,RAMEND-1		; Stack setup
	out	SPL,rTEMP

	ldi	rTEMP,(1<<SE)		; Sleep enable, idle mode on sleep, low levels to wake
	out	MCUCR,rTEMP

	clr	rZERO			; A handy 0
	sts	Debug,rZERO		; Set debug mode to 0

	;
	; Set up data directions etc. on ports. Unused pins are
	; set to input with pullup resistors enabled to prevent
	; floating.
	;
	
	ldi	rTEMP,0xFF
	out	PORTD,rTEMP

	ldi	rTEMP,0xFF-(mSCK)	; SCK low to start
	out	PORTB,rTEMP
	
	ldi	rTEMP,0xFF-(mMISO)	; MISO is an input
	out	DDRB,rTEMP
	
	ldi	rTEMP,(mCTS|mSERIALENABLE)	; Serial enabled to start with
	out	DDRD,rTEMP
	
	;
	; Reset uart
	;
	
	rcall	uart_reset

	;
	; Enable interrupts and off we go...
	;
	
	sei
		
	;
	; Initialize the card
	;
	
	rcall	SPIInitialize

	tst	rRETURN
	breq	Main_Success	; OK!

Main_FatalError:
	cbi	PORTB,bLEDACTIVE
	rcall	WaitMicro
	sbi	PORTB,bLEDACTIVE
	rcall	WaitMicro
	rjmp	Main_FatalError
	
	;
	; OK - Jump into main loop via power off mode, unless
	; RTS is asserted already in which case, off we go.
	;

Main_Success:
	sbic	PIND,bRTS		; RTS asserted?
	rjmp	Main_PowerOff		; No... go straight to power off
	
	cbi	PORTD,bCTS		; OK to send commands
	cbi	PORTB,bLEDACTIVE	; Active LED on

;-----------------------------------------------------------------
;
; Main loop.

Main_Loop:
	lds	rRETURN,Debug
	sbrs	rRETURN,0
	rjmp	Main_WaitCommand

	ldi	rPARAM1,0x0D		; In debug mode, output a command prompt
	rcall	uart_writechar
	ldi	rPARAM1,0x0A
	rcall	uart_writechar
	ldi	rPARAM1,'-'
	rcall	uart_writechar

Main_WaitCommand:
	sleep				; wait for interrupt (from UART?)
	
	rcall	uart_charready		; a character?
	breq	Main_WaitCommand 	; no... go back to sleep
	
	rcall	ReceiveData		; read it and identify command
	
	mov	rPARAM1,rRETURN

	;
	; Switch on different valid values of command byte
	;
	
	cpi	rPARAM1,'!'
	brne	PC+2
	rjmp	CommandDebug
	
	cpi	rPARAM1,'?'
	brne	PC+2
	rjmp	CommandStatus
	
	cpi	rPARAM1,0
	brne	PC+2
	rjmp	CommandNop
	
	cpi	rPARAM1,'S'
	brne	PC+2
	rjmp	CommandSleep

	cpi	rPARAM1,'I'
	brne	PC+2
	rjmp	CommandIdentify
	
	cpi	rPARAM1,'C'
	brne	PC+2
	rjmp	CommandIdentifyCard

	cpi	rPARAM1,'R'
	brne	PC+2
	rjmp	CommandRead
	
	cpi	rPARAM1,'W'
	brne	PC+2
	rjmp	CommandWrite
	
	cpi	rPARAM1,'E'
	brne	PC+2
	rjmp	CommandErase
	
	;
	; Drop through to...
	;

Main_ErrorUnk:
	ldi	rPARAM1,Response_Unk
	rjmp	Main_Response

Main_ErrorFail:
	ldi	rPARAM1,Response_Fail
	rjmp	Main_Response

Main_ResponseOK:
	ldi	rPARAM1,Response_OK

Main_Response:
	rcall	SendData
	
	; Turn off any transientLEDs
	
	sbi	PORTB,bLEDREAD
	sbi	PORTB,bLEDWRITE
	
	rjmp	Main_Loop

;-----------------------------------------------------------------
;
; CommandDebug
; 21 mm 20
; Sets debug mode mm. Debug mode 01 = hex, debug mode 00 = raw.

CommandDebug:
	rcall	ReceiveData
	rcall	ReceiveSpace
	brne	Main_ErrorUnk

	sts	Debug,rRETURN
	
	rjmp	Main_ResponseOK

;-----------------------------------------------------------------
;
; CommandNop
; 00
; Does nothing, returns OK

CommandNop:
	rjmp	Main_ResponseOK
	
;-----------------------------------------------------------------
;
; CommandSleep
; 53 20
; Sends card to sleep, awaiting a new command.
; Returns [Wait], waits until RTS is de-asserted, sends [OK], waits
; a further 65ms and then powers down. 
; If RTS is not deasserted within 65ms or so, powerdown is cancelled
; and it sends [Fail].

CommandSleep:
	rcall	ReceiveSpace
	brne	Main_ErrorUnk

	ldi	rPARAM1,Response_Wait
	rcall	SendData

	clr	XL
	clr	XH
	
CommandSleep_Wait:
	sbic	PIND,bRTS
	rjmp	CommandSleep_PowerOff

	sbiw	XL,1
	brne	CommandSleep_Wait
	
	rjmp	Main_ErrorFail	

CommandSleep_PowerOff:
	ldi	rPARAM1,Response_OK
	rcall	SendData		; Send OK when RTS deasserted
	
	clr	XL
	clr	XH
	rcall	WaitMicro		; Wait for 65ms

Main_PowerOff:
	sbi	PORTD,bCTS		; Deassert CTS

	sbi	PORTB,bLEDACTIVE	; Light off...
	cbi	PORTD,bSERIALENABLE	; Disable serial transmission
	sbi	PORTB,bCS		; Deassert CS

	ldi	rTEMP,(1<<SE)|(1<<SM)	; Sleep enable, power down mode on sleep, low levels to wake
	out	MCUCR,rTEMP

	ldi	rTEMP,(1<<INT0)		; Enable int0
	out	GIMSK,rTEMP

	sleep
	
	out	GIMSK,rZERO		; Disable it again

	ldi	rTEMP,(1<<SE)		; Sleep enable, idle mode on sleep, low levels to wake
	out	MCUCR,rTEMP
	
	cbi	PORTB,bCS		; Assert CS
	sbi	PORTD,bSERIALENABLE	; Re-enable serial transmission
	cbi	PORTB,bLEDACTIVE	; Light on again

	cbi	PORTD,bCTS		; Assert CTS

	rjmp	Main_Loop
	
;-----------------------------------------------------------------
;
; CommandIdentify
; 49 20
; Identifies hardware and software versions
; Returns [Data] 'P' 'F' HH SS [OK]
; HH is the hardware version, SS is the software version.

CommandIdentify:
	rcall	ReceiveSpace
	brne	Main_ErrorUnk

	ldi	rPARAM1,Response_Data
	rcall	SendData
	
	ldi	rPARAM1,'P'
	rcall	SendData

	ldi	rPARAM1,'F'
	rcall	SendData

	ldi	rPARAM1,'0'
	rcall	SendData

	ldi	rPARAM1,'0'
	rcall	SendData
	
	rjmp	Main_ResponseOK
	
;-----------------------------------------------------------------
;
; CommandIdentifyCard
; 43 cc 20
; Reads identification information for card. If cc bit 0=0, returns
; CSD, else CID.
; Returns [Wait][Data] ...16 bytes... [OK]
; or      [Wait][Fail]

CommandIdentifyCard:
	rcall	ReceiveData
	rcall	ReceiveSpace
	breq	PC+2
	rjmp	Main_ErrorUnk
	
	push 	rRETURN

	ldi	rPARAM1,Response_Wait
	rcall	SendData

	pop	rRETURN
	
	ldi	rPARAM1,0x0A		; CMD10 = Send CID (Card ID)
	sbrs	rRETURN,0
	dec	rPARAM1			; CMD09 = Send CSD (Card Hardware Description)
	
	rcall	SPISendCommand	
	
	rcall	SPIReadResponse
	tst	rRETURN
	
	breq	PC+2
	rjmp	Main_ErrorFail
	
	rcall	SPIReadResponse
	mov	rTEMP,rRETURN
	cpi	rTEMP,0xFE	; Data token?
	breq	PC+2
	rjmp	Main_ErrorFail

	ldi	rPARAM1,Response_Data
	rcall	SendData

	ldi	rLOOPI,16

CommandIdentifyCard_Loop:
	rcall	SPIReadByte

	mov	rPARAM1,rRETURN
	rcall	SendData

	dec	rLOOPI
	brne	CommandIdentifyCard_Loop

	rjmp	Main_ResponseOK

;-----------------------------------------------------------------
;
; CommandRead
; 52 n3 n2 n1 n0 a3 a2 a1 a0 20
; Reads nn bytes from address aa.
; Returns [Wait][Data]...nn bytes of data.... crcH crcL [OK]
; or      [Wait][Fail]
	
CommandRead:
	cbi	PORTB,bLEDREAD		; Light "read" LED

	rcall	ReceiveData32		; Read length (32 bits)
	
	mov 	rLENGTH0,XL		; and save
	mov 	rLENGTH1,XH
	mov 	rLENGTH2,YL
	mov 	rLENGTH3,YH
	
	ldi	rPARAM1,0x10		; CMD16 = Set block length
	rcall	SPISendCommand	
	
	rcall	SPIReadResponse

	push 	rRETURN

	rcall	ReceiveData32		; Read address
	
	rcall	ReceiveSpace
	pop	rTEMP
	breq	PC+2
	rjmp	Main_ErrorUnk		; no space - unknown command
	
	cpi	rTEMP,0			; OK?
	breq	PC+2
	rjmp	Main_ErrorFail		; block length failed - error
	
	ldi	rPARAM1,Response_Wait
	rcall	SendData

	ldi	rPARAM1,0x11		; CMD17 = read block
	rcall	SPISendCommand	

	rcall	SPIReadResponse
	tst	rRETURN
	breq	PC+2
	rjmp	Main_ErrorFail
	
	rcall	SPIReadResponse
	mov	rTEMP,rRETURN
	cpi	rTEMP,0xFE		; Data token?
	breq	PC+2
	rjmp	Main_ErrorFail

	ldi	rPARAM1,Response_Data
	rcall	SendData

	rjmp	CommandRead_LoopEnd

CommandRead_Loop:
	rcall	SPIReadByte

	mov	rPARAM1,rRETURN
	rcall	SendData

	ldi	rTEMP,1
	sub	rLENGTH0,rTEMP
	sbc	rLENGTH1,rZERO
	sbc	rLENGTH2,rZERO
	sbc	rLENGTH3,rZERO
		
CommandRead_LoopEnd:

	mov	rSCRATCH,rLENGTH0
	or	rSCRATCH,rLENGTH1
	or	rSCRATCH,rLENGTH2
	or	rSCRATCH,rLENGTH3
	brne	CommandRead_Loop

	;
	; Now send CRC and OK.
	;

	rcall	SPIReadByte
	mov	rPARAM1,rRETURN
	rcall	SendData

	rcall	SPIReadByte
	mov	rPARAM1,rRETURN
	rcall	SendData
	
	rjmp	Main_ResponseOK
		
;-----------------------------------------------------------------
;
; CommandWrite
; 57 n3 n2 n1 n0 a3 a2 a1 a0 d0 d1 d2 ... dn c1 c2 20
; Write data to card
; Writes nn bytes to address aa. Valid values of nn are determined
; by the card capabilities, as are valid values of aa.
;
; Returns [Fail] After receipt of nn and aa if invalid.
; or	  [Wait] [OK]
; or      [Wait] [Fail]

CommandWrite:
	cbi	PORTB,bLEDWRITE		; Light "write" LED

	rcall	ReceiveData32		; Read length (32 bits)
	
	mov 	rLENGTH0,XL		; and save
	mov 	rLENGTH1,XH
	mov 	rLENGTH2,YL
	mov 	rLENGTH3,YH
	
	ldi	rPARAM1,0x10		; CMD16 = Set block length
	rcall	SPISendCommand	
	
	rcall	SPIReadResponse

	push 	rRETURN

	rcall	ReceiveData32		; Read address
	
	pop	rTEMP
	
	cpi	rTEMP,0			; OK?
	breq	PC+2
	rjmp	Main_ErrorFail		; block length failed - error
	
	ldi	rPARAM1,0x18		; CMD24 = write block
	rcall	SPISendCommand	

	rcall	SPIReadResponse
	tst	rRETURN
	breq	PC+2
	rjmp	Main_ErrorFail		; Failed if invalid
	
	ldi	rPARAM1,0xFE		; Data token first
	rcall	SPISendByte

	rjmp	CommandWrite_LoopEnd

CommandWrite_Loop:
	rcall	ReceiveData
	mov	rPARAM1,rRETURN
	rcall	SPISendByte

	ldi	rTEMP,1
	sub	rLENGTH0,rTEMP
	sbc	rLENGTH1,rZERO
	sbc	rLENGTH2,rZERO
	sbc	rLENGTH3,rZERO
		
CommandWrite_LoopEnd:

	mov	rSCRATCH,rLENGTH0
	or	rSCRATCH,rLENGTH1
	or	rSCRATCH,rLENGTH2
	or	rSCRATCH,rLENGTH3
	brne	CommandWrite_Loop

	;
	; Now send CRC and OK.
	;

	rcall	ReceiveData
	mov	rPARAM1,rRETURN
	rcall	SPISendByte

	rcall	ReceiveData
	mov	rPARAM1,rRETURN
	rcall	SPISendByte

	ldi	rPARAM1,Response_Wait
	rcall	SendData
	
	; Now read data token as response
	
	rcall	SPIReadResponse
	mov	rTEMP,rRETURN
	andi	rTEMP,0x1F		; Mask out don't care bits
	cpi	rTEMP,0x05		; 0x05 = OK!
	breq	PC+2
	rjmp	Main_ErrorFail		; else fail (CRC)

CommandWrite_Wait:
	rcall	SPIReadByte		; Read busy signal
	tst	rRETURN			; stays 0 until done
	breq	CommandWrite_Wait
	
	rcall	ReceiveSpace		; Now we expect a space to finish off
	breq	PC+2
	rjmp	Main_ErrorUnk

	rjmp	Main_ResponseOK
	
;-----------------------------------------------------------------
;
; CommandErase
; 45 sg a3 a2 a1 a0 b3 b2 b1 b0 20
; Erase range of sectors or groups. Bit 0 of sg is 0 if sectors are
; to be erased, 1 if groups. The sectors are addressed using addresses
; aa and bb rather than sector numbers. All sectors within this
; address range will be erased. The address aa must be less than or
; equal to bb. If addressing individual sectors, addresses aa and bb
; must fall within the same erase group of 32 sectors.
; Returns [Wait] [OK]
; or	  [Wait] [Fail]

CommandErase:
	cbi	PORTB,bLEDWRITE		; Light "write" LED

	rcall	ReceiveData		; Read sg flag
	clr	ZL			; ZL is offset for commands
	sbrc	rRETURN,0		; if rReturn:0=0, skip
	ldi	ZL,3			; Offset between group and sector cmds = 3

	rcall	ReceiveData32		; Read address AA
	
	mov 	rLENGTH0,XL		; and save
	mov 	rLENGTH1,XH
	mov 	rLENGTH2,YL
	mov 	rLENGTH3,YH
	
	rcall	ReceiveData32		; Read address BB
	
	rcall	ReceiveSpace
	breq	PC+2
	rjmp	Main_ErrorUnk

	ldi	rPARAM1,Response_Wait	; Send wait
	rcall	SendData
	
	cp	XL,rLENGTH0
	cpc	XH,rLENGTH1
	cpc	YL,rLENGTH2
	cpc	YH,rLENGTH3		; check if greater
	
	brcc	PC+2
	rjmp	Main_ErrorFail

	push	XL
	push	XH
	push	YL
	push	YH
	
	mov 	XL,rLENGTH0		; Get start sector
	mov 	XH,rLENGTH1
	mov 	YL,rLENGTH2
	mov 	YH,rLENGTH3

	ldi	rPARAM1,0x20		; CMD32 = Set Start Sector
	add	rPARAM1,ZL		; CMD35 = Set Start Group
	rcall	SPISendCommand
	
	pop	YH
	pop	YL
	pop	XH
	pop	XL

	rcall	SPIReadResponse
	tst	rRETURN
	breq	PC+2
	rjmp	Main_ErrorFail		; Failed if invalid

	ldi	rPARAM1,0x21		; CMD33 = Set End Sector
	add	rPARAM1,ZL		; CMD36 = Set End Group
	rcall	SPISendCommand

	rcall	SPIReadResponse
	tst	rRETURN
	breq	PC+2
	rjmp	Main_ErrorFail		; Failed if invalid

	ldi	rPARAM1,0x26		; CMD38 = Erase tagged sectors or groups
	rcall	SPISendCommand

	rcall	SPIReadResponse
	tst	rRETURN
	breq	PC+2
	rjmp	Main_ErrorFail		; Failed if invalid

	rjmp	Main_ResponseOK
	
;-----------------------------------------------------------------
;
; CommandStatus
; 3F 20
; Gets extended status information in case of error.
; Returns [Data] s1 s2 [OK]

CommandStatus:
	rcall	ReceiveSpace
	breq	PC+2
	rjmp	Main_ErrorUnk

	ldi	rPARAM1,Response_Data
	rcall	SendData

	ldi	rPARAM1,0x0D		; CMD13 = Get status
	rcall	SPISendCommand	
	
	rcall	SPIReadResponse
	mov	rPARAM1,rRETURN
	rcall	SendData
	
	rcall	SPIReadByte
	mov	rPARAM1,rRETURN
	rcall	SendData
	
	rjmp	Main_ResponseOK
	
;-----------------------------------------------------------------
;
; ReceiveData
; Receives data from UART in either raw or hex form

ReceiveData:
	lds	rRETURN,Debug

	sbrs	rRETURN,0
	rjmp	uart_readcharwait

	rcall	uart_readcharwait
	mov	rTEMP,rRETURN
	subi	rTEMP,'0'
	cpi	rTEMP,0x0A
	brcs	ReceiveData_Hex1
	subi	rTEMP,7
ReceiveData_Hex1:
	andi	rTEMP,0x0F
	swap	rTEMP
	
	rcall	uart_readcharwait
	mov	rTEMP2,rRETURN
	subi	rTEMP2,'0'
	cpi	rTEMP2,0x0A
	brcs	ReceiveData_Hex2
	subi	rTEMP2,7
ReceiveData_Hex2:
	andi	rTEMP2,0x0F
	or	rTEMP,rTEMP2
	mov	rRETURN,rTEMP

	ldi	rPARAM1,'>'
	rjmp	uart_writechar

;-----------------------------------------------------------------
;
; ReceiveSpace
; Receives a character anc checks it's a space. Returns eq if it is,
; ne if it isn't.

ReceiveSpace:
	push	rRETURN
	rcall	ReceiveData
	mov	rTEMP,rRETURN
	pop	rRETURN
	cpi	rTEMP,' '
	ret
;-----------------------------------------------------------------
;
; ReceiveData32
; Receives a 32 bit data value into YH:YL:XH:XL

ReceiveData32:	
	rcall	ReceiveData
	mov	YH,rRETURN
	rcall	ReceiveData
	mov	YL,rRETURN
	rcall	ReceiveData
	mov	XH,rRETURN
	rcall	ReceiveData
	mov	XL,rRETURN	; Y:X is address
	ret

;-----------------------------------------------------------------
;
; SendData
; Sends data to the UART in either raw or hex form

SendData:
	lds	rRETURN,Debug

	sbrs	rRETURN,0
	rjmp	uart_writechar

	rcall	uart_writehex
	ldi	rPARAM1,' '
	rjmp	uart_writechar

;-----------------------------------------------------------------
;
; WaitMicro
; Wait approximately X microseconds, in a tight loop.

WaitMicro:			; 4 cycles
	nop			; per loop
	dec	XL		; at 3.686 MHz
	brne	WaitMicro	; = about 1 uS per loop

	dec	XH
	brne	WaitMicro

	ret

;-----------------------------------------------------------------
;
; SPI/Flash routines

;-----------------------------------------------------------------
;
; SPIInitialize
; Initialise card and place into SPI mode.
; rRETURN is 0 if successful, otherwise it is an error code.

SPIInitialize:
	sbi	PORTB,bCS	; Ensure chip select high to start with
	
	; Send 80 "high" bits to get things started
	
	ldi	rPARAM1,0xFF
	rcall	SPISendByte
	rcall	SPISendByte
	rcall	SPISendByte
	rcall	SPISendByte
	rcall	SPISendByte
	rcall	SPISendByte
	rcall	SPISendByte
	rcall	SPISendByte
	rcall	SPISendByte
	rcall	SPISendByte

	; Now send CMD0 - go to idle state

	ldi	rPARAM1,0
	ldi	XL,0
	ldi	XH,0
	ldi	YL,0
	ldi	YH,0
	rcall	SPISendCommand
	
	rcall	SPIReadResponse
	ldi	rTEMP2,1
	ldi	rTEMP,1
	cp	rRETURN,rTEMP
	brne	SPIInitialize_Error	; 1 expected.

	; Now send CMD1 - initialise, and retry!
	
	clr	rLOOPJ			; up to 256 times
SPIInitialize_Loop:		
	ldi	rPARAM1,1
	ldi	XL,0
	ldi	XH,0
	ldi	YL,0
	ldi	YH,0
	rcall	SPISendCommand
	
	rcall	SPIReadResponse
	tst	rRETURN
	breq	SPIInitialize_OK
	
	dec	rLOOPJ
	brne	SPIInitialize_Loop

	breq	SPIInitialize_OK	; Error code in rRETURN already

SPIInitialize_Error:
	mov	rRETURN,rTEMP2
SPIInitialize_OK:
	ret

;-----------------------------------------------------------------
;
; SPIReadResponse
; Read a response byte from the card

SPIReadResponse:
	clr	rSCRATCH		;timeout (256 tries)

SPIReadResponse_Wait:
	rcall	SPIReadByte		; and read into rRETURN

	mov	rTEMP,rRETURN		; 0xFF indicates unready...
	cpi	rTEMP,0xFF
	brne	SPIReadResponse_End	; ...so wait for anything else

;	tst	rRETURN			; 0... (OK)
;	breq	SPIReadResponse_End
;
;	mov	rTEMP,rRETURN
;	cpi	rTEMP,1			; or 1... (Idle)
;	breq	SPIReadResponse_End
;	
;	cpi	rTEMP,0xFE		; or 0xFE... (Data token)
;	breq	SPIReadResponse_End

	dec	rSCRATCH		; else go round again
	brne	SPIReadResponse_Wait	; until time out

	;
	; Error code goes in here
	;
	
SPIReadResponse_End:
	ret

;-----------------------------------------------------------------
;
; SPISendCommand
; Send command. Command is in rPARAM1, Parameter is in X (low) and Y (high).
; CRC is a constant 0x95 - this is the CRC for the initial CMD0 which gets us
; into SPI mode, from then on the CRC is ignored.

SPISendCommand:
	sbi	PORTB,bCS	; CS high during flushing

	push	rPARAM1
	ldi	rPARAM1,0xff
	rcall	SPISendByte

	cbi	PORTB,bCS	; CS must be low to enable SPI mode
	
	pop	rPARAM1
	ori	rPARAM1,0x40
	rcall	SPISendByte
	
	mov	rPARAM1,YH
	rcall	SPISendByte
	mov	rPARAM1,YL
	rcall	SPISendByte
	mov	rPARAM1,XH
	rcall	SPISendByte
	mov	rPARAM1,XL
	rcall	SPISendByte
	
	ldi	rPARAM1,0x95	; CRC for init command, ignored otherwise
	
	;
	; ... and drop through to SPISendByte
	;


;-----------------------------------------------------------------
;
; SPISendByte
; Sends a byte from rPARAM1 in SPI mode to MOSI pin. At the same
; time, a byte is read from the MISO pin and returned in rRETURN.

SPISendByte:
	push	rLOOPI
	push	rPARAM1

	clr	rRETURN
	
	ldi	rLOOPI,8
SPISendByte_Loop:
	add	rRETURN,rRETURN
	add	rPARAM1,rPARAM1
	brcs	SPISendByte_1

	cbi	PORTB,bMOSI
	rjmp	SPISendByte_Clock

SPISendByte_1:
	sbi	PORTB,bMOSI

SPISendByte_Clock:	
	in	rSCRATCH,PINB

	sbi	PORTB,bSCK
	SCKDELAY

	cbi	PORTB,bSCK
	SCKDELAY
	
	sbrc	rSCRATCH,bMISO
	inc	rRETURN

	dec	rLOOPI
	brne	SPISendByte_Loop
	
	pop	rPARAM1
	pop	rLOOPI
	
	ret

;-----------------------------------------------------------------
;
; SPIReadByte
; Just sends an 0xFF, returns with the data as usual.

SPIReadByte:
	ldi	rPARAM1,0xFF
	rjmp	SPISendByte

;-----------------------------------------------------------------
	
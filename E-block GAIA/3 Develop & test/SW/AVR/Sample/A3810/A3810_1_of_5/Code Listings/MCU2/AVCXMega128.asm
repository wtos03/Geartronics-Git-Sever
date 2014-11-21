.NOLIST

.INCLUDE "m128def.inc"
.INCLUDE "Macros.inc"

.LIST
.LISTMAC

;##########################################################
;##                     ###################################
;## GENERAL DEFINITIONS ###################################
;##                     ###################################
;##########################################################

.EQU IN_PROGRESS = 0   ;Int_6 Processing in progress
                       ;For Int_6 use only
.EQU NULL = 0          ;Null string termination

.EQU RESET_Q      = $FF
.EQU STANDBY      = $00
.EQU COLOR        = $88
.EQU MOVE         = $90
.EQU PUT_CHAR     = $A0
.EQU PRINT_STRING = $B0
.EQU PICTURE      = $C0
.EQU CLEAR_SCR    = $43
.EQU PSET         = 4
.EQU ARG_X        = 1
.EQU ARG_Y        = 2
.EQU CRD_INC      = 0
.EQU CRD_DEC      = 8
.EQU LIGHT_ON     = $7F
.EQU LIGHT_OFF    = $40
.EQU FONT_HEIGH   = 1
.EQU FONT_WIDTH   = 1
.EQU ODD_BYTE     = 1


;##########################################################
;##                  ######################################
;## GLOBAL VARIABLES ######################################
;##                  ######################################
;##########################################################

;Register Variables
;==================
;Lower Registers
.DEF RxData = R2       ;For RS232 use only
.DEF RxWriteIndex = R3 ;For RS232 use only
.DEF RxReadIndex  = R4 ;For Int_6 use only
.DEF Status_6     = R5 ;Int_6 SREG, saved by Int_7
.DEF PC_Lo        = R6 ;Int_6 Program Counter, saved by Int_7
.DEF PC_Hi        = R7 ;

;Upper Registers
.DEF Vx        = R16   ;Video X coordinate
.DEF Vy        = R17   ;Video Y coordinate
.DEF Tmp0      = R18
.DEF Tmp1      = R19
.DEF SwFlags   = R20   ;Software Flags
.DEF Tmp3      = R21
.DEF GrCmd     = R22   ;Current Graphic Command
.DEF GrState   = R23   ;Graphic Command state machine variable
.DEF GrOldCmd  = R24   ;Last Graphic Command

.DSEG
.ORG $100

;RAM Variables
;=============
RxBuffer: .BYTE 256    ;RS232 Receive Buffer
Reg_ZL:   .BYTE 1      ;Int_6 Z, saved by Int_7
Reg_ZH:   .BYTE 1      ;
Reg_XL:   .BYTE 1      ;Int_6 X, saved by Int_7
Reg_XH:   .BYTE 1

;##########################################################
;##             ###########################################
;## STACK SPACE ###########################################
;##             ###########################################
;##########################################################

.EQU STACK_SIZE = 256

.DSEG

.ORG (RAMEND - STACK_SIZE)

StackBottom: .BYTE STACK_SIZE

.CSEG
;##########################################################
;##                   #####################################
;## INTERRUPT VECTORS #####################################
;##                   #####################################
;##########################################################
.INCLUDE "IntVect.inc"

;##########################################################
;##             ###########################################
;## ENTRY POINT ###########################################
;##             ###########################################
;##########################################################

Reset:
  ; Disable Interrupts during initializations
  cli

;##########################################################
;##                              ##########################
;## STACK POINTER INITIALIZATION ##########################
;##                              ##########################
;##########################################################
  ldi Tmp0, HIGH(RAMEND)
  out SPH, Tmp0
  ldi Tmp0, LOW(RAMEND)
  out SPL, Tmp0

;##########################################################
;##                          ##############################
;## I/O PORTS INITIALIZATION ##############################
;##                          ##############################
;##########################################################

  ; Initialize PORT G
  ; =================
  ; 4 3210 Bit No
  ; 0 0000 Value
  ; o ioii Direction
  ldi Tmp0, 0b01011
  sts PORTG, Tmp0
  ldi Tmp0, 0b10100
  sts DDRG, Tmp0

  ; Initialize PORT E
  ; =================
  ; 7654 3210 Bit No
  ; 1111 0000 Value
  ; iioo ooii Direction
  ldi Tmp0, 0b11111111
  out PORTE, Tmp0
  ldi Tmp0, 0b00101100
  out DDRE,  Tmp0

  ; Initialize PORT A
  ; =================
  ; 7654 3210 Bit No
  ; ---- ---- Value
  ; oooo oooo Direction
  ldi Tmp0, 0b11111111
  out DDRA, Tmp0

  ; Initialize PORT C
  ; =================
  ; 7654 3210 Bit No
  ; ---- ---- Value
  ; oooo oooo Direction
  out DDRC, Tmp0

  ; Initialize PORT B
  ; =================
  ; 7654 3210 Bit No
  ; ---- ---- Value
  ; oooo oooo Direction
  out DDRB, Tmp0

  ; Initialize all other ports
  ; as inputs with pull-up resistors
  ; 7654 3210 Bit No
  ; 1111 1111 Value
  ; iiii iiii Direction
  ldi Tmp0, $FF
  out PORTD, Tmp0
  sts PORTF,Tmp0
  ldi Tmp0, 0
  out DDRD, Tmp0
  sts DDRF, Tmp0

;##########################################################
;##                                    ####################
;## EXTERNAL INTERRUPTS INITIALIZATION ####################
;##                                    ####################
;##########################################################

  ; INT0: Off
  ; INT1: Off
  ; INT2: Off
  ; INT3: Off
  ; INT4: Off
  ; INT5: Off
  ; INT6: On, Falling Edge
  ; INT7: On, Falling Edge

  ldi Tmp0,  $00
  sts EICRA, Tmp0
  ldi Tmp0,  $A0
  out EICRB, Tmp0
  ldi Tmp0,  $C0
  out EIFR,  Tmp0
  ; Mask off all external interrupts for now
  ldi Tmp0,  $00
  out EIMSK, Tmp0

;##########################################################
;##                        ################################
;## USART 0 INITIALIZATION ################################
;##                        ################################
;##########################################################

  ; Receiver:    On, Interrupt: On
  ; Transmitter: On, Interrupt: Off
  ; Mode:        Asynchronous
  ; Parameters:  8 Data, 1 Stop, No Parity
  ; Baud rate:   115200
  ldi Tmp0,   $00
  out UCSR0A, Tmp0
  ldi Tmp0,   $98
  out UCSR0B, Tmp0
  ldi Tmp0,   $06
  sts UCSR0C, Tmp0
  ldi Tmp0,   3
  out UBRR0L, Tmp0
  ldi Tmp0,   $00
  sts UBRR0H, Tmp0

;##########################################################
;##                                  ######################
;## ANALOG COMPARATOR INITIALIZATION ######################
;##                                  ######################
;##########################################################

  ;Analog Comparator: Off
  ldi Tmp0,  $80
  out ACSR,  Tmp0
  ldi Tmp0,  $00
  out SFIOR, Tmp0


;##########################################################
;##                          ##############################
;## VARIABLES INITIALIZATION ##############################
;##                          ##############################
;##########################################################

  clr RxWriteIndex
  clr RxReadIndex
  clr SwFlags
  clr GrState      ; GrState = STANDBY


;##########################################################
;##                               #########################
;## VARIOUS FINAL INITIALIZATIONS #########################
;##                               #########################
;##########################################################

  ; Select Upper Page of Flash for fonts, bitmaps, etc.
  ldi Tmp1, 1
  out RAMPZ, Tmp1

  ;Clear Screen
  SET_COLOR 7, 7, 7
;  CLR_SCREEN
  ldi Vy, 159   ;
  cls1:
  out PORTC, Vy ;<-----+
  ldi Vx, 239   ;      |
  cls0:
  out PORTA, Vx ;<--+  |
  cbi PORTE, 3  ;   |  |
  sbi PORTE, 3  ;   |  |
  subi Vx, 1    ;   |  |
  brcc cls0     ;---+  |
  subi Vy, 1    ;      |
  brcc cls1     ;------+
  ldi Vx, 0     ;
  ldi Vy, 0     ;

  SET_COLOR 0, 0, 0

  ldi Tmp1, $B0
  mov  YL, RxWriteIndex
  clr  YH
  subi YL,  LOW(-RxBuffer)
  sbci YH, HIGH(-RxBuffer)
  st   Y+,  Tmp1
  ;RxWriteIndex = RxWriteIndex + 1 (modulo 256)
  inc RxWriteIndex

  ldi Tmp1, 'R'
  st   Y+,  Tmp1
  inc RxWriteIndex
  ldi Tmp1, 'e'
  st   Y+,  Tmp1
  inc RxWriteIndex
  ldi Tmp1, 'a'
  st   Y+,  Tmp1
  inc RxWriteIndex
  ldi Tmp1, 'd'
  st   Y+,  Tmp1
  inc RxWriteIndex
  ldi Tmp1, 'y'
  st   Y+,  Tmp1
  inc RxWriteIndex
  ldi Tmp1, 0
  st   Y+,  Tmp1
  inc RxWriteIndex


  ;Clear Int_6 & Int_7 flags
  ldi Tmp0,  0b11000000
  out EIFR,  Tmp0

  ;Unmask Int_6 & keep Int_7 masked
  ;Int_7 may be unmasked by Int_6 only
  ldi Tmp0, 0b01000000
  out EIMSK, Tmp0

  ;Release Video RAM and start Display Processor (ATmega8515)
  VRAM_RELEASE

  ;Enable Interrupts
   sei
;  cli

;##########################################################
;##                                   #####################
;## MAIN PROGRAM (Background Process) #####################
;##                                   #####################
;##########################################################
MainLoop:
  rjmp MainLoop

  ; We should never get here, so do the Software Reset
  jmp 0


;##########################################################
;##                           #############################
;## VRAM ACCESS START (Int_6) #############################
;##                           #############################
;##########################################################
Int_6:
  ; Any Data in buffer ?
  cp RxReadIndex, RxWriteIndex
  brne DataInBuffer
  ; Processing in progress ?
  sbrs SwFlags, IN_PROGRESS
  reti

DataInBuffer:
  ; Clear Int 7 flag
  ldi Tmp3,  0b10000000
  out EIFR,  Tmp3
  ;Mask all interrupts, with the exeption of Int_7
  cbi UCSR0B, 7
  ; Take control over Video RAM
  VRAM_CONNECT

  ;cbi UCSR0B, 7

  ;Unmask Int_7 & mask Int_6
  ;Int_6 will be unmasked by Int_7 or Main
  ldi tmp3, 0b10000000
  out EIMSK, tmp3

  ;Int_7 occured during last Int_6?
  ;================================
  sbrs SwFlags, IN_PROGRESS

  ;No - continue normal processing
  ;==
  rjmp Int6_L00

  ;Yes - restore and continue processing
  ;===   from the point it was interrupted by Int_7
  ori SwFlags, (1 << IN_PROGRESS)
  ;Restore SREG
  out SREG, Status_6
  ;Restore Z and X registers
  lds ZL, Reg_ZL
  lds ZH, Reg_ZH
  lds XL, Reg_XL
  lds XH, Reg_XH
  ;Restore Program Counter
  push PC_lo
  push PC_hi
  ;Program Counter is now at the top of the stack
  ;reti instruction will get stored Program Counter
  ;from the stack, jump to the address pointed by it
  ;and enable global interrupts making the process sensitive
  ;to Int_7 (VRAM ACCESS STOP)
  reti

  ;Continue normal processing
Int6_L00:
  ;Set IN_PROGRESS Flag
  ori SwFlags, (1 << IN_PROGRESS)

  ;Enable global interrupts to make the process sensitive
  ;to Int_7 (VRAM ACCESS STOP)
  sei

  ;####################################
  ;# Graphic Command Processing BEGIN #
  ;####################################

Int6_Main_Loop:
  ;BEGIN while (RxReadIndex != RxWriteIndex)
  cp RxReadIndex, RxWriteIndex
  brne Int6_L01
  jmp Int6_Exit

Int6_L01:
  ;Get next Graphic Command
  ;GrCmd = RxBuffer[RxReadIndex]
  mov  ZL, RxReadIndex
  clr  ZH
  subi ZL,  LOW(-RxBuffer)
  sbci ZH, HIGH(-RxBuffer)
  ld GrCmd, Z


  ;#######################
  ;# State Machine BEGIN #
  ;#######################

;------------------------------------------------------------------
GrState_STANDBY:
  cpi GrState, STANDBY
  breq PC+2
  rjmp GrState_PICTURE

  ;Decode Command
  mov  ZL, GrCmd
  andi ZL, 0b11000000
  cpi  ZL, 0b01000000
  brne MultiByteCommand
  rjmp SingleByteCommand

MultiByteCommand:
  mov  GrOldCmd, GrCmd
  mov  GrState, GrCmd
  andi GrState, $F8
  jmp GetNextCommand

SingleByteCommand:
ClrScreen:
  cpi GrCmd, CLEAR_SCR
  brne LightOn
  CLR_SCREEN
  jmp GetNextCommand

LightOn:
  cpi GrCmd, LIGHT_ON
  brne LightOff
  ldi GrCmd, 0b10000
  sts PORTG, GrCmd
  jmp GetNextCommand

LightOff:
  cpi GrCmd, LIGHT_OFF
  brne GrPset
  ldi GrCmd, 0b00000
  sts PORTG, GrCmd
  jmp GetNextCommand

GrPset:
  sbrs GrCmd, 2
  rjmp CrdDec
  VRAM_WR

CrdDec:
  sbrs GrCmd, 3
  rjmp CrdInc

  sbrs GrCmd, 0
  rjmp CrdDecVy
  dec Vx
  cpi Vx, $FF
  brne PC+2
  ldi Vx, 239
  out PORTA, Vx
  rjmp GetNextCommand

CrdDecVy:
  sbrs GrCmd, 1
  jmp GetNextCommand

  dec Vy
  cpi Vy, $FF
  brne PC+2
  ldi Vy, 159
  out PORTC, Vy
  jmp GetNextCommand

CrdInc:
  sbrs GrCmd, 0
  rjmp CrdIncVy
  inc Vx
  cpi Vx, 240
  brne PC+2
  ldi Vx, 0
  out PORTA, Vx
  jmp GetNextCommand

CrdIncVy:
  sbrs GrCmd, 1
  jmp GetNextCommand
  inc Vy
  cpi Vy, 160
  brne PC+2
  ldi Vy, 0
  out PORTC, Vy
  jmp GetNextCommand

;------------------------------------------------------------------


GrState_PICTURE:
  cpi GrState, PICTURE
  breq PC+2
  rjmp GrState_COLOR

  ; PICTURE Commands just issued?
  cpi GrOldCmd, PICTURE
  brne GrPict_00
  ldi Vx, 0
  ldi Vy, 159
  ldi tmp1, 0
  ldi GrOldCmd, 0

GrPict_00:
  cpi  tmp1, 0
  breq GrPict_EvenByte
  rjmp GrPict_OddByte

GrPict_EvenByte:
  ldi tmp1, 7
  out PORTC, Vy
  out PORTA, Vx
  out PORTB, GrCmd
  jmp GetNextCommand

GrPict_OddByte:
  ldi tmp1, 0
  cpi GrCmd, 0
  brne GrPict_01
  cbi  PORTE, 2
  rjmp GrPict_02
GrPict_01:
  sbi  PORTE, 2
GrPict_02:
  VRAM_WR
  inc Vx
  cpi Vx, 240
  breq GrPict_03
  jmp GetNextCommand

GrPict_03:
  ldi Vx, 0
  dec Vy
  cpi Vy, 255
  breq GrPict_04
  jmp GetNextCommand
GrPict_04:
  ldi Vy, 0
  jmp GrState_DEFAULT

GrState_COLOR:
  cpi GrState, COLOR
  breq PC+2
  rjmp GrState_MOVE

  lsl  GrCmd
  lsl  GrCmd
  lsr  GrOldCmd
  ror  GrCmd
  lsr  GrOldCmd
  ror  GrCmd
  cbi  PORTE, 2
  lsr  GrOldCmd
  brcc Color_00
  sbi PORTE, 2
Color_00:
  out PORTB, GrCmd

  jmp GrState_DEFAULT

;------------------------------------------------------------------
GrState_MOVE:
  cpi GrState, MOVE
  breq PC+2
  rjmp GrState_PRINT_STRING

  sbrs GrOldCmd, 0
  rjmp GrMoveVy
  mov Vx, GrCmd
  out PORTA, Vx
  rjmp GrMovePset

GrMoveVy:
  sbrs GrOldCmd, 0
  rjmp GrMoveEnd
  mov Vy, GrCmd
  out PORTC, Vy

GrMovePset:
  sbrs GrOldCmd, 2
  rjmp GrMoveEnd
  VRAM_WR

GrMoveEnd:
  jmp GrState_DEFAULT

;------------------------------------------------------------------
GrState_PRINT_STRING:
  cpi GrState, PRINT_STRING
  breq GrL_00

GrState_PUT_CHAR:
  cpi GrState, PUT_CHAR
  breq PC+2
  rjmp GrState_DEFAULT
  clr GrState
  rjmp GrL_01


; Check if NULL
GrL_00:
  cpi GrCmd, NULL
  brne GrL_01
  rjmp GrState_DEFAULT
GrL_01:
  jmp PutChar
GrL_02:

  jmp GetNextCommand
;------------------------------------------------------------------

GrState_DEFAULT:
  clr GrState

  ;#######################
  ;# State Machine END   #
  ;#######################

GetNextCommand:

  inc RxReadIndex
  jmp Int6_Main_Loop
  ;END while (RxReadIndex != RxWriteIndex)

Int6_Exit:

  ;####################################
  ;# Graphic Command Processing END   #
  ;####################################

  ;Disable global interrupts - reti will enable them
  cli
  ;Process is finished - clear IN_PROGRESS flag
  andi SwFlags, ~(1 << IN_PROGRESS)
  sbi UCSR0B, 7

reti


;##########################################################
;##                          ##############################
;## VRAM ACCESS STOP (Int_7) ##############################
;##                          ##############################
;##########################################################
Int_7:
  ;Disconnect VRAM
  VRAM_RELEASE
  ;Save Int_6 SREG
  in Status_6, SREG
  ;Save Z and X registers in RAM
  sts Reg_ZL, ZL
  sts Reg_ZH, ZH
  sts Reg_XL, XL
  sts Reg_XH, XH

  ;Mask Int_7 (It will be unmasked by the next Int_6)
  ;and unmask Int_6 and all other interrupts
  ldi tmp3, 0b01000000
  out EIMSK, tmp3
  sbi UCSR0B, 7

  ;Int_7 Occured during Int_6?
  sbrs SwFlags, IN_PROGRESS
  ;No - return
  reti
  ;Yes - Save Int_6 Program Counter and
  ;return to Main (not to Int_6)
  pop PC_Hi
  pop PC_Lo
reti


;##########################################################
;##                             ###########################
;## RS232 RECEIVE EVENT HANDLER ###########################
;##                             ###########################
;##########################################################
IntUart_0_Rx:
  ; Get Data from Rs232

  in RxData, UDR0
 ; out UDR0, GrState

  ; Store RxData to RxBuffer[RxWriteIndex]
  mov  YL, RxWriteIndex
  clr  YH
  subi YL,  LOW(-RxBuffer)
  sbci YH, HIGH(-RxBuffer)
  st   Y,  RxData

  ;RxWriteIndex = RxWriteIndex + 1 (modulo 256)
  inc RxWriteIndex

reti

;##########################################################
;##                                         ###############
;## PUTCHAR - Character printing subroutine ###############
;##                                         ###############
;##########################################################
PutChar:
  ldi GrOldCmd, FONT_WIDTH*8
  ; Get Font8x8[GrCmd][0]
  ldi ZL,  LOW(Font8x8*2)
  ldi ZH, HIGH(Font8x8*2)
  mov XL, GrCmd
  ldi XH, FONT_WIDTH*FONT_HEIGH*8
  mul XL, XH
  add ZL, R0
  adc ZH, R1
  mov XL, Vy
GrLoop_01:
  ldi GrCmd, FONT_HEIGH*8
  elpm Tmp1, Z+
GrLoop_02:
  lsr Tmp1
  brcc GrJump_01
  out PORTC, Vy
  cli
  cbi PORTE, 3
  sbi PORTE, 3
  sei
GrJump_01:
  inc Vy
  dec GrCmd
  brne GrLoop_02

  mov Vy, XL
  inc Vx
  cpi Vx, 240
  brlo GrJump_02
  ldi Vx, 0
  subi Vy, (255-FONT_HEIGH*8)+1
  mov XL, Vy
GrJump_02:
  out PORTA, Vx
  out PORTC, Vy

  dec  GrOldCmd
  brne GrLoop_01
  jmp GrL_02

;##########################################################
;##                     ###################################
;## FONTS & BITMAPS     ###################################
;##                     ###################################
;##########################################################
;##                     ###################################
;## Located in the      ###################################
;## Upper Page of Flash ###################################
;##                     ###################################
;##########################################################

.NOLIST
.CSEG
.ORG $8000

.INCLUDE "Font8x8.inc"

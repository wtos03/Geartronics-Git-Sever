.INCLUDE "m8515def.inc"

.equ BIT_0 = 1
.equ BIT_1 = 2
.equ BIT_2 = 4
.equ BIT_3 = 8
.equ BIT_4 = 16
.equ BIT_5 = 32
.equ BIT_6 = 64
.equ BIT_7 = 128

.equ LCD       = PORTD 
.equ LCD_DIR   = DDRD 
.equ HSYNC     = 7
.equ VSYNC     = 6
.equ SHUT_DOWN = 3
.equ CLOCK_ON  = 5

.equ RAM       = PORTB 
.equ RAM_DIR   = DDRB
.equ RAM_PIN   = PINB 
.equ OE        = 0
.equ RAM_START = 2
.equ RAM_STOP  = 1

.equ ADDR_LO     = PORTA
.equ ADDR_LO_DIR = DDRA
.equ ADDR_HI     = PORTC
.equ ADDR_HI_DIR = DDRC

.equ FREEZE_TIME = 100

.def tmp = r16
.def addr_lsb = r18
.def addr_msb = r19
.def pwrup_state = r20

.CSEG

.ORG 0x000
  rjmp Reset

.ORG 0x012

Reset:
  cli
  ldi tmp, 0
  out SPH, tmp
  ldi tmp, 255
  out SPL, tmp

  ; Release RAM Control to ATmega128
  ; ================================
  ; Disconnect 8515 address bus
  ldi tmp, 0
  out ADDR_LO_DIR, tmp
  ;  243 LCD dots
  out ADDR_HI_DIR, tmp
  out ADDR_LO, tmp
  ;  244 LCD dots
  out ADDR_HI, tmp
  
  ldi tmp, 1 | 1<<RAM_START | 1<<RAM_STOP
  out RAM, tmp

  ldi tmp, 1<<RAM_START | 1<<RAM_STOP
  out RAM_DIR, tmp

  ldi tmp, 1<<HSYNC | 1<<VSYNC | 1<<CLOCK_ON
  out LCD, tmp

  ldi tmp, 1<<HSYNC | 1<<VSYNC | 1<<SHUT_DOWN | 1<<CLOCK_ON
  out LCD_DIR, tmp

  ldi r24, LOW(FREEZE_TIME)
  ldi r25, HIGH(FREEZE_TIME)
  clr addr_lsb
  clr addr_msb
  clr pwrup_state
 ; sbi LCD, SHUT_DOWN


WaitForMega128:
  sbic RAM_PIN, OE
  rjmp WaitForMega128

  ldi tmp, 0xFF
  out ADDR_LO, addr_lsb
  out ADDR_HI, addr_msb
  out ADDR_HI_DIR, tmp
  out ADDR_LO_DIR, tmp

  nop
; Send 240 Dots
; =============
LCD_Line_Start:
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  out ADDR_LO, addr_lsb
  inc addr_lsb
  ;  240 LCD dots
LCD_HSync:
  ; Modify Address
  clr addr_lsb
  inc addr_msb
  ;  241 LCD dots
  ; End of frame?
  cpi addr_msb, 160
  ;  241.5 LCD dots
  brsh LCD_VSync
  ;  242 LCD dots
  ; Release RAM Control to ATmega128
  ; ================================
  ; Disconnect 8515 address bus
  ldi tmp, 0
  out ADDR_LO_DIR, tmp
  ;  243 LCD dots
  out ADDR_HI_DIR, tmp
  ; 243.5
  ; Send RAM_START Interrupt to ATmega128
  cbi RAM, RAM_START
  ;  244.5 LCD dots
  nop
  ;  245 LCD dots
  sbi RAM, RAM_START
  ;  246 LCD dots

Loop_01:
  cbi LCD, CLOCK_ON ; Freeze On
  ;  247 LCD dots
  sbiw r25:r24, 1
  ;  248 LCD dots
  breq Get_Ram
  rjmp Loop_01

Get_Ram:
  ldi r24, LOW(FREEZE_TIME)
  ldi r25, HIGH(FREEZE_TIME)
  sbi  LCD, CLOCK_ON ; Freeze Off
  ;  247 LCD dots (according to LCD)

  ; Wait 16th dots to activate HSync
  ; ================================
  rcall Wait_8_dots
  ; 255 LCD dots
  nop
  ; 255.5 LCD dots
  ; Activate HSync
  ; ==============
  cbi LCD, HSYNC
  ; 256.5 LCD dots
  nop
  ; 257 LCD dots 
  rcall Wait_4_dots
  ; 261 LCD dots 
  ; Reclaim RAM Control from ATmega128
  ; ==================================
  ; Send RAM_STOP Interrupt to ATmega128
  cbi RAM, RAM_STOP
  ; 262 LCD dots
  ldi tmp, 0xFF
  nop  
  ; 263 LCD dots
  sbi RAM, RAM_STOP
  ; 264 LCD dots
  out ADDR_HI, addr_msb
  out ADDR_LO, addr_lsb
  ; 265 LCD dots
  nop
  nop
  ; 266 LCD dots
  nop
  nop
  ; 267 LCD dots
  nop
  nop
  ; 268 LCD dots
  ; Turn off HSync
  ; ==============
  sbi LCD, HSYNC
  ; 269 LCD dots
  ; Connect 8515 address bus
  out ADDR_HI_DIR, tmp
  out ADDR_LO_DIR, tmp
  ; 270 LCD dots
  nop
  nop
  ; 271 LCD dots
  rjmp LCD_Line_Start


LCD_VSync:
  ;  242.5 LCD dots
  nop
  ;  243 LCD dots
  cpi addr_msb, 160
  brne WaitForLine_180

  ;  244 LCD dots
  ; Release RAM Control to ATmega128
  ; ================================
  ; Disconnect 8515 address bus
  ldi tmp, 0
  out ADDR_LO_DIR, tmp
  ;  245 LCD dots
  out ADDR_HI_DIR, tmp
  out ADDR_LO, tmp
  ;  246 LCD dots
  out ADDR_HI, tmp
  ;  246.5 LCD dots
  ; Send RAM_START Interrupt to ATmega128
  cbi RAM, RAM_START
  ;  247.5 LCD dots
  nop
  ;  248 LCD dots
  sbi RAM, RAM_START
  ;  249 LCD dots
  rcall Wait_6_dots
  ; 255 LCD dots
  nop
  ; 255.5 LCD dots
  ; Activate HSync
  ; ==============
  cbi LCD, HSYNC
  ; 256.5 LCD dots
  nop
  nop
  nop
  ; 258 LCD dots
  rcall Wait_10_dots
  ; 268 LCD dots
  ; Turn off HSync
  ; ==============
  sbi LCD, HSYNC
  ; 269 LCD dots
  nop
  nop
  ; 270 LCD dots
  nop
  nop
  ; 271 LCD dots
  rjmp LCD_Line_Start


WaitForLine_180:
  ;  244.5 LCD dots
  nop
  ;  245 LCD dots
  cpi addr_msb, 180
  breq Line_180
  ;  246 LCD dots
  brpl WaitForLine_185
  ;  246.5 LCD dots
  nop
  ;  247 LCD dots
  rcall Wait_8_dots
  ; 255 LCD dots
  nop
  ; 255.5 LCD dots
  ; Activate HSync
  ; ==============
  cbi LCD, HSYNC
  ; 256.5 LCD dots
  nop
  nop
  nop
  ; 258 LCD dots
  rcall Wait_10_dots
  ; 268 LCD dots
  ; Turn off HSync
  ; ==============
  sbi LCD, HSYNC
  ; 269 LCD dots
  nop
  nop
  ; 270 LCD dots
  nop
  nop
  ; 271 LCD dots
  rjmp LCD_Line_Start

Line_180:
  ;  246.5 LCD dots
  nop
  ;  247 LCD dots
  nop
  nop
  ;  248 LCD dots
  nop
  nop
  ;  249 LCD dots
  rcall Wait_6_dots
  ; 255 LCD dots
  nop 
  ; 255.5 LCD dots
  ; Activate HSync
  ; ==============
  cbi LCD, HSYNC
  ; 256.5 LCD dots
  nop
  nop
  nop
  ; 258 LCD dots
  rcall Wait_10_dots
  ; 268 LCD dots
  ; Turn off HSync
  ; ==============
  sbi LCD, HSYNC
  ; 269 LCD dots
  nop
  nop
  ; 270 LCD dots
  ; Activate VSync
  ; ==============
  cbi LCD, VSYNC
  ;  271 LCD dots
  rjmp LCD_Line_Start


WaitForLine_185:
  ;  247 LCD dots
  cpi addr_msb, 185
  breq Line_185
  ;  248 LCD dots
  brpl WaitForLine_200
  ;  248.5 LCD dots
  nop
  ;  249 LCD dots
  rcall Wait_6_dots
  ; 255 LCD dots
  nop
  ; 255.5 LCD dots
  ; Activate HSync
  ; ==============
  cbi LCD, HSYNC
  ; 256.5 LCD dots
  nop
  nop
  nop
  ; 258 LCD dots
  rcall Wait_10_dots
  ; 268 LCD dots
  ; Turn off HSync
  ; ==============
  sbi LCD, HSYNC
  ; 269 LCD dots
  nop
  nop
  ; 270 LCD dots
  nop
  nop
  ; 271 LCD dots
  rjmp LCD_Line_Start

Line_185:
  ;  248.5 LCD dots
  nop
  ;  249 LCD dots
  cbi RAM, RAM_STOP
  ; 250
  nop
  nop
  ; 251
  sbi RAM, RAM_STOP
  ; 252
  nop
  nop
  ; 253
  nop
  nop
  ; 254
  nop
  nop
  ; 255 LCD dots
  nop
  ; 255.5 LCD dots
  ; Activate HSync
  ; ==============
  cbi LCD, HSYNC
  ; 256.5 LCD dots
  nop
  nop
  nop
  ; 258 LCD dots
  rcall Wait_10_dots
  ; 268 LCD dots
  ; Turn off HSync
  ; ==============
  sbi LCD, HSYNC
  ; 269 LCD dots
  nop
  ; 269.5 LCD dots
  ; Turn off VSync
  ; ==============
  sbi LCD, VSYNC
  ; 270.5 LCD dots
  nop
  ; 271 LCD dots
  rjmp LCD_Line_Start


WaitForLine_200:
  ; 249 LCD dots
  cpi addr_msb, 200
  breq Line_200
  ; 250 LCD dots
  rcall Wait_4_dots
  ; 254 LCD dots
  nop
  nop
  ; 255 LCD dots
  nop
  ; 255.5 LCD dots
  ; Activate HSync
  ; ==============
  cbi LCD, HSYNC
  ; 256.5 LCD dots
  nop
  nop
  nop
  ; 258 LCD dots
  rcall Wait_10_dots
  ; 268 LCD dots
  ; Turn off HSync
  ; ==============
  sbi LCD, HSYNC
  ; 269 LCD dots
  nop
  nop
  ; 270 LCD dots
  nop
  nop
  ; 271 LCD dots
  rjmp LCD_Line_Start



Line_200:
  ; 250.5 LCD dots
  nop
  ; 251 LCD dots
  ; Reset Address
  clr addr_msb
  clr addr_lsb
  ; 252
  ; Reclaim RAM Control from ATmega128
  ; ==================================
  ; Send RAM_STOP Interrupt to ATmega128
;  cbi RAM, RAM_STOP
     nop
     nop
  ; 253
  nop
  nop
  ; 254
;  sbi RAM, RAM_STOP
     nop
     nop
  ; 255
  nop
  ; 255.5
  ; Activate HSync
  ; ==============
  cbi LCD, HSYNC
  ; 256.5 LCD dots
  
  rcall Wait_4_dots
  ; 260.5 LCD dots
  nop
  nop
  ; 261.5 LCD dots
  nop  
  ; 262 LCD dots
  nop
  nop
  ; 263 LCD dots
  cpi pwrup_state, 3
  brsh Turn_On_LCD
  ; 264 LCD dots
  inc pwrup_state
  ; 264.5 LCD dots
  nop
  ; 265 LCD dots
  rjmp Dots_266

Turn_On_LCD:
  ; 264.5 LCD dots
  nop
  ; 265 LCD dots
  sbi LCD, SHUT_DOWN

Dots_266:
  ; 266 LCD dots
  out ADDR_HI, addr_msb
  out ADDR_LO, addr_lsb
  ; 267 LCD dots
  ldi tmp, 0xFF
  nop
  ; 268 LCD dots
  ; Turn off HSync
  ; ==============
  sbi LCD, HSYNC
  ; 269 LCD dots
  ; Connect 8515 address bus
  out ADDR_HI_DIR, tmp
  out ADDR_LO_DIR, tmp
  ; 270 LCD dots
  nop
  nop
  ; 271 LCD dots
  rjmp LCD_Line_Start

  
; #########
; ## End ##
; #########
  rjmp reset


  
; ##################
; ## Soubroutines ##
; ##################
Wait_4_dots:
  ; + 1.5 LCD dots
  nop
  ; + 2 LCD dots
  ret 

Wait_6_dots:
  ; + 1.5 LCD dots
  nop
  ; + 2 LCD dots
  nop
  nop
  ; + 3 LCD dots
  nop
  nop
  ; + 4 LCD dots
  ret 

Wait_8_dots:
  ; + 1.5 LCD dots
  nop
  ; + 2 LCD dots
  rcall Wait_4_dots
  ; + 6 LCD dots
  ret 

Wait_10_dots:
  ; + 1.5 LCD dots
  nop
  ; + 2 LCD dots
  rcall Wait_6_dots
  ; + 8 LCD dots
  ret 

Wait_12_dots:
  ; + 1.5 LCD dots
  nop
  ; + 2 LCD dots
  rcall Wait_8_dots
  ; + 10 LCD dots
  ret 


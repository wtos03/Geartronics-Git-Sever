# Simple Makefile
# Volker Oth (c) 1999

########### change this lines according to your project ##################

#put the name of the target mcu here (at90s8515, at90s8535, attiny22, atmega603 etc.)
    MCU = at90s8515

#put the name of the target file here (without extension)
	TRG	= gcctest9

#put your C sourcefiles here 
	SRC	= $(TRG).c uart.c timer.c

#put additional assembler source file here
	ASRC = sqrtMike.s sqrtJack.s

#additional libraries and object files to link
	LIB	=

#additional includes to compile
	INC	= 

#compiler flags
	CPFLAGS	= -g -O2 -Wall -Wstrict-prototypes -Wa,-ahlms=$(<:.c=.lst)

#linker flags
	LDFLAGS = -Wl,-Map=$(TRG).map,--cref

########### you should not need to change the following line #############
include $(AVR)/avrfreaks/avr_make
		  
###### dependecies, add any dependencies you need here ###################

$(TRG).o: $(TRG).c uart.h timer.h global.h
uart.o: uart.c uart.h timer.h global.h
timer.o : timer.c timer.h uart.h
sqrtJack.o: sqrtJack.s
sqrtMike.o: sqrtMike.s

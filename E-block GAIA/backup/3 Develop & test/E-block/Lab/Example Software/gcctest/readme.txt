./include/make1 and make2 in the avrgcc-distribution from AVRfreaks
have been merged into a single file, and some minor errors corrected.
This requires a single #include statement:

include $(AVR)/avrfreaks/avr_make

instead of the previous two. This change has been made to all the makefiles included
with gcctest1-9.

NOTE: this means if you are using the gcctest archive distributed from AVRfreaks.net after 2001-11-29; you also need the AVRfreaks avrgcc release of nov.2001 or later!

- www.AVRfreaks.net
- 2001-11-29
----------------


./include/make2 has been modified to get a smoother
integrasjon with AVR Studio.

- www.AVRfreaks.net
- 2001-06-08
----------------

./include/make2 file has been modified to handle these known issues to 
the latest avr-gcc distribution.

1) Bug with avr-objcopy with eeprom section:
   avr-objcopy incorrectly add the eeprom section
   to both the rom and the AVR Studio obj file.
Workaround: add -R .eeprom to both the .rom and .obj target

2) Avr-gcc incorrectly adds an offset to the eeprom section
Workaround: add --change-section-lma .eeprom=0 to the .eep target.

3) Several reported bugs with flag -O3:
   strlen() built-in line reports wrong result...
Workaround: use -O2 or -Os

- www.AVRfreaks.net
- 2001-02-19
----------------

This is V4 of my gcctest stuff. Nearly all programs
should work on 2313/8515/8535/Megas now. To change mcu
simply edit the "MCU =" line in the makefile.
Please copy make1/make2 inside your avr-gcc include directory
(.\include).
This will also make the examples compilable under Linux.

You might need to get my latest avr-gcc ditribution from my website:
http://members.xoom.xom/volkeroth

regards,
Volker
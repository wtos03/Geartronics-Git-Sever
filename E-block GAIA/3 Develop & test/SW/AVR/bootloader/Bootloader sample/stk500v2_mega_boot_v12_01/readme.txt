Bootloader/monitor with STK500v2 protocol for ATmega128/AT90can128
(c) Milosz Klosowicz, MikloBit

Home page: http://www.miklobit.com
Contact:   support@miklobit.com

2005.07.05	version 1.0
		Require AVR Studio 4.11 build 401 or later.

		STK500 v2 command/parameter supported:
			CMD_SIGN_ON:
        		CMD_GET_PARAMETER:
					- PARAM_HW_VER
					- PARAM_SW_MAJOR
					- PARAM_SW_MINOR
					- PARAM_VTARGET (dummy)
					- PARAM_VADJUST (dummy)
					- PARAM_OSC_PSCALE (dummy)
					- PARAM_OSC_CMATCH (dummy)
					- PARAM_SCK_DURATION (dummy)
					- PARAM_TOPCARD_DETECT
        		CMD_SET_PARAMETER:
        		CMD_ENTER_PROGMODE_ISP:
        		CMD_LEAVE_PROGMODE_ISP:
        		CMD_CHIP_ERASE_ISP: (1st page)
        		CMD_READ_SIGNATURE_ISP:
        		CMD_LOAD_ADDRESS:
        		CMD_PROGRAM_FLASH_ISP:
        		CMD_READ_FLASH_ISP:
        		CMD_PROGRAM_EEPROM_ISP:
        		CMD_READ_EEPROM_ISP:
        		CMD_READ_FUSE_ISP:
        		CMD_READ_LOCK_ISP:			
        		
		Cpu supported:
			AT90can128
			ATmega128
			ATmega64 (not tested)
			ATmega32 (not tested)
		Other ATmega's with bootblock size less that 4kb should work, 
		but it require to comment out some command in source file  
		
		Bootloader start conditions:
		
		Atmega128:
			if (( memory($0000) != $FF) && (PORTF.7 ==1) && (PORTF.6 == 1) ) -> start application 
			if (( memory($0000) != $FF) && (PORTF.7 ==0) ) -> boot from UART0
			if (( memory($0000) != $FF) && (PORTF.6 ==0) ) -> boot from UART1
			if (( memory($0000) == $FF) && (PORTF.7 ==1) && (PORTF.6 == 1) && ) -> boot from UART0
		
		AT90CAN128:
			if (( memory($0000) != $FF) && (PORTF.7 ==1) && (PORTF.6 == 1) && ) -> start application 
			if (( memory($0000) != $FF) && (PORTF.7 ==0) ) -> boot from UART1
			if (( memory($0000) != $FF) && (PORTF.6 ==0) ) -> boot from UART0
			if (( memory($0000) == $FF) && (PORTF.7 ==1) && (PORTF.6 == 1) && ) -> boot from UART1	
			
		Other ATmega:	
			if ( memory($0000) != $FF) && (PORTD.6 ==1) -> start application 
			if ( memory($0000) != $FF) && (PORTD.6 ==0) ) -> boot from UART
			if ( memory($0000) == $FF) && (PORTD.6 ==1)  -> boot from UART		
	
2005.09.09	version 1.2	
		Monitor functions added:
		
		!!!					monitor enter
		F AAAAA				dump Flash memory starting from  address AAAAA 
		X AAAA				dump RAM(external RAM) memory from address AAAA
		X AAAA MM			modify RAM at address AAAA with value MM
		E AAA				dump EEPROM memory from address AAA
		E AAA MM			modify EEPROM at address AAAA with value MM
		T					display time from RTC72423 chip
		T HHMMSS			set RTC time to HH:MM:SS
		D					display date from RTC72423 chip
		D YYMMDD			set RTC date to YY/MM/DD
		Q					quit monitor
		[ENTER]				repeat last command ( with address autoincrement )
		
		Notes:
				monitor can be entered when bootlader is active and running.
				all memory address are byte values in hex 	
				RTC base address is $6000	  
		
		
To do:
		CAN interface monitor
		I2C interface monitor			
									
							   
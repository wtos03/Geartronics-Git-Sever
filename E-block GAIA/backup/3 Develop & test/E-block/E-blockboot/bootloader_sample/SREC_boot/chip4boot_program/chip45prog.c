//
// chip45prog.c
// ------------
//
// chip45prog programming tool for the chip45boot bootloader
//
// Copyright 2006 by Erik Lins, www.chip45.com
//
// The program reads an s-record file from disk and send it to
// an AVR MCU with chip45boot bootloader installed.
//
// License
// -------
// chip45prog is distributed under the Lesser General Public License (LGPL).
// You should have received a copy (lgpl.txt) of the license along with
// this file or go to http://www.gnu.org/licenses/lgpl.html.
//
// Usage
// -----
// chip45prog <port> <memory> <filename.srec>\n");
// where <port> is the serial port to be used (com1, com2, etc.) (*)\n");
//       <memory> is either 'flash' or 'eeprom'\n");
//       <filename.srec> is the s-record file to program\n");
// (*) when running under linux, the serial ports have different names
//
// When you start chip45prog with proper parameters, ít will open the
// serial port and send 'i' characters continously. The user then has
// to reset the target and the bootloader will go into interactive mode.
// chip45prog waits for the prompt character '>' to be received and will
// then send 'f' or 'e' depending on the memory option specified.
// The s-record file will be send line by line with 250msec delay in between
// until and end-of-file s-record line is reached.
//
// Compiling chip45prog
// --------------------
// Progam can be compiled with the GCC compiler:
// gcc -o chip45prog.exe chip45prog.c
//
// Adjusting a Makefile to Work with chip45prog
// --------------------------------------------
// Add the following lines to your Makefile somewhere below the "program:"
// entry:
// ---- cut from here ----
// # Program the device with preloaded chip45boot
// # adjust the virtual COM port number
// chip45boot: $(TARGET).hex $(TARGET).eep
//     srec_cat $(TARGET).hex -Intel -Output $(TARGET).srec -Motorola;
//     chip45prog com7 flash $(TARGET).srec;
// ---- to here ----
// Note: The indention before srec_cat and chip45prog MUST be a <tab>, no
// space characters are allowed in the Makefile at that point!
//
// You can now download your program with "make chip45boot".
// 
// TODO
// ----
// - Currently there is no error code of the bootloader checked.
// - The progress indicators (line of '.') are buffered by the terminal
//  and become visible simultaneously at end of program.
//
// See Also
// --------
// See chip45boot.c source code for a description how the bootloader works.
//
//
// Cheers, ER!K
//


// Subversion keywords
//
// $Rev$
// $Date$
//


//
// includes
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/select.h>


//
// defines
//
#define MSEC 1000
#define VERSION "0.9"


//
// show usage of program
//
void usage(void) {
    printf("usage: chip45prog <port> <memory> <filename.srec>\n");
    printf("where <port> is the serial port to be used and\n");
    printf("      <memory> is either 'flash' or 'eeprom'\n");
    printf("      <filename.srec> is the s-record file to program\n");
}


//
// initialize the serial port
//
int initSerialPort(char *port) {

    struct termios options;
    int fd;


    // try to open the port
    if( (fd = open(port, O_RDWR | O_NOCTTY)) < 0) {
	printf("Connot open port: %s\n", port);
	exit(-1);
    }

    
    // read current options from serial port
    tcgetattr(fd, &options);


    // set baud rate to 115200
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);


    // enable
    options.c_cflag |= (CLOCAL | CREAD);


    // 1 stop bit, no parity, 8 data bits
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;


    // noncanonical, no parity check, no flow control
    options.c_lflag &= ~ICANON; 
    options.c_iflag &= ~INPCK;
    options.c_iflag &= ~IXOFF;


    // actually set the options
    if(!tcsetattr(fd, TCSANOW, &options)) {
	printf("Cannot set options for port: %s\n", port);
	close(fd);
	return 0;
    }


    // force read to return immediately and not wait for character
    fcntl(fd, F_SETFL, FNDELAY);


    // return the file descriptor to the main program
    return fd;
}


//
// main program starts here
//
int main(int argc, char **argv)
{

    // local variables
    char str[2];
    char srecord[150];

    int serialFileDesc;  // file descriptor for serial port
    FILE *diskFilePtr;   // file pointer for reading the s-record file


    // welcome message
    printf("chip45prog V%s, (C) E.Lins, www.chip45.com\n\n", VERSION);


    // check if we have enough arguments
    if (argc != 4) {
	// if not, show usage and exit
	usage();
	exit(0);
    }


    // check if memory parameter is correct
    if(strcmp(argv[2], "flash") && strcmp(argv[2], "eeprom")) {
	// if not, show usage and exit
	usage();
	exit(0);
    }


    // open the s-record file on disk
    if( (diskFilePtr = fopen(argv[3], "r")) == NULL) {
	printf("Cannot open file: %s\n", argv[3]);
	return 0;
    }


    // debug output of parameters
    //    printf("%s, %s, %s, %s\n\n", argv[0], argv[1], argv[2], argv[3]);


    // initialize the serial port
    serialFileDesc = initSerialPort(argv[1]);


    // tell the user to reset the target
    printf("Please reset target MCU!\n");


    // wait for character '>' from serial port
    do {
	// send character 'i' to enter interactive mode of chip45boot
	write(serialFileDesc, "i", 1);

	// read from serial port
	read(serialFileDesc, str, 1);

	// loop until the interactive mode prompt is received
    } while(str[0] != '>');


    // enter flash or eeprom programming mode
    if(strcmp(argv[2], "flash") == 0) {

	printf("Entering flash programming mode.\n");

	// sending 'f' enters flash programming mode
	write(serialFileDesc, "f", 1);  

    } else if(strcmp(argv[2], "eeprom") == 0) {

	printf("Entering eeprom programming mode.\n");

	// sending 'e' enters eeprom programming mode
	write(serialFileDesc, "e", 1);  
    }


    // read the s-record file and send to target
    do {

	// read one s-record line
	fscanf(diskFilePtr, "%s", srecord);
	putchar('.');

	// send s-record line to the target
	write(serialFileDesc, srecord, strlen(srecord));
	write(serialFileDesc, "\r", 1);

	// wait 250 milliseconds to let the MCU program the flash page
	usleep(250 * MSEC);

	// exit loop when an end of file s-record line was read
    } while((srecord[1] != '9') && (srecord[1] != '8') && (srecord[1] != '7'));


    // close the s-record file
    fclose(diskFilePtr);


    // success message 
    printf("\nSuccessfully loaded target!\n");

    // close the serial port
    close(serialFileDesc);


    // cheers, ER!K
    return 1;
}

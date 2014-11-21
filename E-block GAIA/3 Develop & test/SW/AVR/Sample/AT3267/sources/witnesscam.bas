'**************************************************************************************
'*
'*   AT3267 - THE WITNESS CAMERA - MAIN FILE
'*
'**************************************************************************************
'*  Purposes of this module:
'*
'*   -   System definitions, hardware assignments, I/O ports
'*   -   System initialization
'*   -   Main loop
'*   -   Handling of disk insertion/removal
'*   -   Interrupt handlers (except remote control)
'*
'**************************************************************************************

declare sub diskInsertion()
declare sub powerCheck()
declare sub beep()
declare sub recordPictureFile()
declare sub diskCleanup( byval isIdle as byte )

const TRUE = 1
const FALSE = 0

$regfile = "M32def.dat"
$crystal = 7372800
$hwstack = 250                'hardware stack
$swstack = 250                'SW stack
$framesize = 250              'frame space

'**************************************************************************************
'*  SERIAL PORTS CONFIGURATION
'**************************************************************************************

' Debug and diagnostic channel: it is a soft-port (that is, bitbanged)
' used for printing diagnostic and status messages
' Soft ports can be created on any pin, by specifying "COM C.5" as a name
' we select port C, bit 5. We use an "inverted" polarity in order to
' connect the pin direct to the PC serial port (works on most PCs).
'
Open "comc.5:9600,8,n,1,inverted" For Output As #1

' Camera port: this is an UART-based port at 115200 baud.
' Instruct the compiler to automatically set up an IRQ-based input FIFO for incoming data.
'
$baud = 115200
config SERIALIN = BUFFERED , SIZE = 68
open "Com1:" as binary as #2
waitmS 100


'**************************************************************************************
'*  REAL TIME CLOCK
'**************************************************************************************
'
' Instructs the compiler to set up and RTC service running on the
' 32.768 kHz crystal. Don't be fooled by the SOFT, actually it makes use of AVR hardware
' RTC resources (opposed to HARD that specifies to resort to an external clock chip)
'
config CLOCK = Soft
config DATE = Ymd , Separator = .

'**************************************************************************************
'*  I/O PIN CONFIGURATION AND FLAGS
'**************************************************************************************

config PinA.0 = output          ' Disk semaphore, GREEN light = can remove the flash card
greenLed Alias PortA.0
greenLed = 1

config PinA.1 = output          ' Disk semaphore, RED light = don't remove the flash card
redLed Alias PortA.1
redLed = 1

config PinA.2 = output          ' Disk write activity LED
diskWriteLed Alias PortA.2
diskWriteLed = 1

config PinA.3 = output          ' P.I.R. Movement sensor feedback LED
pirLed Alias PortA.3
pirLed = 1

config PinC.3 = output          ' Tamper Relay output
relay Alias PortC.3
relay = 0

config PinC.4 = input           ' Lid-open sensing switch
PortC.4 = 1                     ' Enable pullup on lid-switch input
lidOpened alias PinC.4

config PortB.1 = input          ' Disk present/missing switch (from the SD-card connector)
config PortB.0 = input
PortB.1 = 1                     ' Enable pullup on this pin
diskMissing alias PinB.1

config PinB.3 = input           ' Disk write protect (from the SD-card connector)
PortB.3 = 1                     ' Enable pullup on this pin
writeProtect alias PinB.3

config PinD.2 = input           ' Remote Control input
PortD.2 = 1                     ' Enable pullup on remote control input
config RC5 = Pind.2             ' Tell the compiler to link remote control library to this pin
on INT0 remoteControlInterrupt  ' Name the interrupt service routine
set MCUCR.ISC00                 ' Configure Interupt Sense Control bits
set MCUCR.ISC01                 ' for requesting an interrupt on rising edge
set GICR.INT0                   ' Enable external interrupt #0
dim remoteControlFlag as bit    ' Flags any activity by the remote control
reset remoteControlFlag

config PinD.3 = input           ' PIR sensor input and associated interrupt flag and handler
PirInput alias PinD.3
on INT1 pirInterrupt            ' Name the interrupt service routine
set MCUCR.ISC10                 ' Configure Interupt Sense Control bits
set MCUCR.ISC11                 ' for requesting an interrupt on rising edge
set GICR.INT1                   ' Enable external interrupt #1
dim pirFlag as bit              ' Flags a movement detected by the PIR sensor
reset pirFlag

config PinB.2 = input           ' External trigger switch and associated flag and handler
PortB.2 = 1                     ' Enable pullup on this pin
on INT2 extTriggerInterrupt     ' Name of the interrupt service routine
reset MCUCSR.ISC2               ' Configure Interupt Sense Control bit = falling edge
set   GICR.INT2                 ' Enable external interrupt #2
dim extTriggerFlag as bit
reset extTriggerFlag

dim diskInitialized as bit
diskInitialized = FALSE
dim powerGood as bit
powerGood = FALSE
dim mainsPowerMode as bit
mainsPowerMode = FALSE
dim clockIsSet as bit
clockIsSet = FALSE

'**************************************************************************************
'*  ADC CONFIGURATION
'*
'*  Instructs the compiler how to setup the ADC.
'*  The SINGLE mode must be used with the GETADC() function
'*  The AUTO feature, will select the highest clockrate possible for the given crystal
'*  frequency (internally sets the ADC prescaler for a clock in the range 50-200 kHz)
'*  The Vref is set to be the AVCC pin (3.3 Volts)
'*
'*  Pins used: battery power monitor = ADC channel #4
'*            mains power monitor   = ADC channel #5
'**************************************************************************************

config ADC = SINGLE, PRESCALER = AUTO, REFERENCE = AVCC
const batteryADC    = 4
const mainsPowerADC = 5
start ADC                       ' Time to power-up the ADC circuitry
                                ' Later, with STOP ADC, you can remove the power
                                ' from the ADC internal hardware


enable interrupts


print #1, "***************************"     ' Print header on the Diagnostic/Log stream.
print #1, "* WITNESSCAM DEBUG STREAM *"
print #1, "***************************"
print #1, ""
print #1, "POWER UP"

'**************************************************************************************
'*  INITIALIZATION/INCLUSION OF EXTERNAL MODULES
'**************************************************************************************

$include "Config_MMC.bas"
$include "Config_AVR-DOS.BAS"

$include "recorder.bas"         ' Initialize all modules.
$include "camera.bas"
call camera_initialize()
$include "speech_synth.bas"
$include "archive.bas"
$include "voice_menu.bas"

call beep()
print #1 , "STARTING SYSTEM"    ' Log that we are ready to work.

'**************************************************************************************
'*
'*  MAIN LOOP
'*
'**************************************************************************************


do
    call powerCheck                                                 ' Update power flags
    if powerGood = TRUE then                                        ' Is power reliable?
            call voiceMenu                                          ' Yes, check for any remote-control interaction

            if diskInitialized = FALSE and lidOpened = FALSE then   ' Check for disk insertions/removals
                if diskMissing = FALSE then                         ' If a disk insertion is detected,
                    waitMs 1000                                     ' wait a little to allow things to settle,
                    if diskMissing = FALSE then                     ' and if the disk looks still in place
                        call diskInsertion()                        ' attempt adisk initialization.
                    endif
                endif
            endif

            if lidOpened = TRUE or diskInitialized = FALSE then

                ' When lid is open, or power isn't OK,
                ' stop any disk activity
                ' in order to prevent data corruption.

                greenLed = 1                                        ' Set "disk semaphore" to green in order
                redLed   = 0                                        ' to inform users the card can be extracted.
            else

                ' Lid is closed, power is good:
                ' these are the normal operating conditions.

                greenLed = 0                                        ' Set "disk semaphore" to red,
                redLed   = 1                                        ' as we're using the disk.
                if mainsPowerMode = TRUE then                       ' Pause recorder when mains isn't
                    call recorder()                                 ' available (you can change this line
                endif                                               ' if you prefer recording on batteries)
            endif
    else
        diskInitialized = FALSE                                     ' If power is not reliable,
        greenLed = 0                                                ' reset the disk and set "disk semaphore"
        redLed   = 1                                                ' to red for prudence.
        call beep()                                                 ' Try getting attention.
        waitmS 500
    endif

    ' Time to perform periodic housekeeping,
    ' like clearing LEDs and updating flags.

    if pirInput = 0 then                                            ' Led mimics sensor's status
        reset pirLed
    endif

    if diskMissing = 1 then
        diskInitialized = FALSE                                     ' Force initialization and cleanup
    endif

    if diskInitialized = FALSE or lidOpened = TRUE then             ' If the box is opened or the disk
        reset relay                                                 ' does not work, change tamper output
    else                                                            ' (usually connected to 24-hour
        set relay                                                   ' tamper input of burglar alarms).
    endif
loop

end



'**************************************************************************************
'*
'*  Get disk parameters and reset archive status.
'*  This routine is meant to be called when a disk change is detected,
'*  or when the system is powered-up.
'*  At power up, attempts setting the clock according to the time of most recent file,
'*  and salutes users with a system settings summary.
'*  Logs disk parameters on I/O stream #1 (connect to PC serial port for debug)
'*
'**************************************************************************************

sub diskInsertion()

    local errorCode as byte

    diskInitialized = FALSE
    print #1 , "Initialize disk... ";
    Gbdriveerror = driveinit()
    if Gbdriveerror = 0 then

        print #1 , "Initialize File System ... ";
        ' Select partition 1 (or use 0 for drive without Master Boot Record)
        errorCode = initFileSystem(1)
        if errorCode <> 0 then
            print #1, "Error: " ; errorCode ; " while initializing file system"
        else
            print #1, "Filesystem type: " ; Gbfilesystem
            print #1, "FAT Start Sector: " ; Glfatfirstsector
            print #1, "Root Start Sector: " ; Glrootfirstsector
            print #1, "Data First Sector: " ; Gldatafirstsector
            print #1, "Max. Cluster Nummber: " ; Glmaxclusternumber
            print #1, "Sectors per Cluster: " ; Gbsectorspercluster
            print #1, "Root Entries: " ; Gwrootentries
            print #1, "Sectors per FAT: " ; Glsectorsperfat
            print #1, "Number of FATs: " ; Gbnumberoffats
            print #1, "Disk size: "; disksize(); " kB"
            print #1, "Disk free: "; diskfree(); " kB"

            diskInitialized = TRUE
            call initializeArchive()
            call beep()
            waitMs 200
            if clockIsSet = FALSE then          ' Upon reset, clock settings are gone,
                call dateTimeGuess()            ' so try to guess current date and time
                clockIsSet = TRUE               ' based on the last file found.
                call say( "GREETING" )          ' Tell the audience we're alive...
                call sayStatus()                ' ...and give a summary of current settings.
            endif
        end If
    else
        print #1 , "Error during Drive Init: " ; Gbdriveerror
    end if

end sub


'**************************************************************************************
'*
'* Update power flags:
'*  -   powerGood tells the overall status of available power;
'*  -   mainsPowerMode tells the actual power sorce
'*
'**************************************************************************************

sub powerCheck()
    powerGood = FALSE
    if getAdc( batteryADC ) > 350 then
        powerGood = TRUE
    endif
    if getAdc( mainsPowerADC ) > 350 then
        mainsPowerMode = TRUE
    else
        mainsPowerMode = FALSE
    endif
end sub



'**************************************************************************************
'* Interrupt handlers
'**************************************************************************************

pirInterrupt:
    set pirFlag
    set pirLed
    return

extTriggerInterrupt:
    set extTriggerFlag
    return

remoteControlInterrupt:
    set remoteControlFlag
    return
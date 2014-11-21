'**************************************************************************************
'*
'*   AT3267 - THE WITNESS CAMERA - VOICE-PROMPT, REMOTE CONTROL OPERATED MENU SYSTEM
'*
'**************************************************************************************
'*
'*  This module implements the voice-prompt editing of system settings
'*  Remote control must use RC5 encoding to work.
'*  Uses the speech primitives from speech_synth.bas
'*
'**************************************************************************************


'**************************************************************************************
'*  Public functions
'**************************************************************************************

declare sub voiceMenu()

'**************************************************************************************
'*  Private functions
'**************************************************************************************
declare function getRemoteControl( byval timeout as byte ) as byte
declare function getPassword() as byte
declare sub editDateTime()
declare sub editRecordMode()
declare function editVariable( variable as byte, byval button as byte, byval minValue as byte, byval maxValue as byte  ) as byte



'**************************************************************************************
'*  Keyboard table ( numbers are Philips-RC5 key codes )
'*  Change this table if you want to use a different key setup
'**************************************************************************************

const Key_EDIT_MODE     = 55    ' key for changing the recording mode
const Key_EDIT_TIME     = 52    ' key for setting up date and time
const Key_REPORT_STATUS = 50    ' key for getting a summary of current settings
const Key_NEXT          = 32    ' key for advancing to next menu entry
const Key_PREV          = 33    ' key for going back to previous menu entry
const Key_INCREASE      = 16    ' key for increasing current value
const Key_DECREASE      = 17    ' key for decreasing current value
const Key_0             = 0     ' first numeric key for the password (please don't change)
const Key_9             = 9     ' last  numeric key for the password (please don't change)
const Key_RECORD        = 42    ' key for taking an immediate snapshot

dim pvsRemoteCmd as byte    ' Buffer for the last remote-control code received, used for debounce
pvsRemoteCmd = 255          ' initialize to 255 = no key-code received



'**************************************************************************************
'*  Setup MCUCR so that the falling edge of INT0 (routed to the infrared receiver)
'*  generates an interrupt request.
'*  Although no ISR is in place, we use the external interrupt flag in order to latch
'*  any remote control signal being received.
'**************************************************************************************

reset MCUCR.ISC00
set   MCUCR.ISC01
goto remoteInitDone     'this statement required by BASCOM compiler



'**************************************************************************************
'*  Voice-prompt menu system entry point.
'*  Checks the remote control, if no key pressed returns immediately.
'*  Otherwise enters one of the password protected, voice-prompted setup menus.
'**************************************************************************************

sub voiceMenu()
    local button as byte

    if remoteControlFlag = 1 then
        reset remoteControlFlag
        button = getRemoteControl( 1 )
        select case button

            case Key_EDIT_TIME:
                button = getPassword()
                if button <> 0 then
                    call editDateTime()
                endif

            case Key_EDIT_MODE
                button = getPassword()
                if button <> 0 then
                    call editRecordMode()
                endif

            case Key_REPORT_STATUS:
                call sayStatus()

        end select
    endif
    call audioOff()
end sub



'**************************************************************************************
'*  Gets and debounces the remote control. Takes a timeout before giving up.
'*  Returns the code of the key pressed, or 255 if nothing received.
'**************************************************************************************

function getRemoteControl( byval timeout as byte ) as byte
    local remoteAddr as byte
    local remoteCmd as byte

    while timeout > 0
        timeout = timeout - 1
        getRC5( remoteAddr, remoteCmd )
        if remoteAddr = 0 and remoteCmd <> 255 and remoteCmd <> pvsRemoteCmd then
            pvsRemoteCmd = remoteCmd
            remoteCmd = remoteCmd and 127
            print #1 , "RC5: addr=" ; remoteAddr ; " cmd=" ; remoteCmd
            exit while
        endif
    wend
    getRemoteControl = remoteCmd
end function



'**************************************************************************************
'*  Gets a 4-digit numeric password.
'*  Returns TRUE and says "OK" if password match, otherwise aborts silently
'*  and returns FALSE
'*  NOTE: default password is: 1-2-3-4
'**************************************************************************************

function getPassword() as byte
    local correct as byte
    local button as byte

    call say( "PASSWORD")
    correct = 0
    button = getRemoteControl(100)
    if button = 1 then
        correct = correct + 1
        button = getRemoteControl(100)
        if button = 2 then
            correct = correct + 1
            button = getRemoteControl(100)
            if button = 3 then
                correct = correct + 1
                button = getRemoteControl(100)
                if button = 4 then
                    correct = correct + 1
                endif
            endif
        endif
    endif
    if correct = 4 then
        getPassword = TRUE
        call say( "OK" )
    else
        getPassword = FALSE
    endif
end function



'**************************************************************************************
'*  Voice-prompt menu for editing system date and time
'**************************************************************************************

sub editDateTime()
    local button as byte         ' button pressed on the remote control
    local exitCode as byte       ' code returned by the editing routine
    local stage as byte          ' stage 0 = year, 1 = month, 2 = day....

    print #1, "EDIT DATE/TIME"
    call sayDateTime()

    stage = 0
    do
            select case stage
            case 0:
                call say( "YEAR" )
                do
                    call sayNumber( _year )
                    button = getRemoteControl( 100 )
                    exitCode = editVariable( _year, button, 6, 30 )
                loop until exitCode <> 0

            case 1:
                call say( "MONTH" )
                do
                    call sayMonth( _month )
                    button = getRemoteControl( 100 )
                    exitCode = editVariable( _month, button, 1, 12 )
                loop until exitCode <> 0

            case 2:
                call say( "DAY" )
                do
                    call sayNumber( _day )
                    button = getRemoteControl( 100 )
                    exitCode = editVariable( _day, button, 1, 31 )
                loop until exitCode <> 0

            case 3:
                call say( "HOUR" )
                do
                    call sayNumber( _hour )
                    button = getRemoteControl( 100 )
                    exitCode = editVariable( _hour, button, 0, 23 )
                loop until exitCode <> 0

            case 4:
                call say( "MINUTE" )
                do
                    call sayNumber( _min )
                    button = getRemoteControl( 100 )
                    exitCode = editVariable( _min, button, 0, 59 )
                loop until exitCode <> 0

            end select

            '''''''''''''''''''''''''''''''''''''''''''''''''''''''

            select case exitCode
                case 1:                         ' code = 1 means advance to next menu
                    if stage < 4 then           ' is this the last menu?
                        stage = stage + 1       ' no, advance to next menu
                    else                        '
                        exit do                 ' yes, exit the menu system
                    endif                       '
                case 2:                         ' code = 2 means go back to previous menu
                    if stage > 0 then           ' is this the first menu?
                        stage = stage - 1       ' no, go back one level
                    endif                       ' otherwise ignore request
                case 3:                         ' code = 3 means exit the menu chain
                    exit do                     ' exit the menu chain
            end select

    loop
    call sayDateTime()                          ' exiting menus: repeat date and time for user check

end sub



'**************************************************************************************
'*  Voice-prompt menu for editing record parameters (resolution, shooting mode,
'*  number of frames and intervals)
'**************************************************************************************

sub editRecordMode()
    local button as byte
    local exitCode as byte

    print #1, "EDIT RECORD MODE"

    do
        call sayResolution()
        button = getRemoteControl( 100 )
        exitCode =  editVariable( cameraResolution, button, 0, 1 )
    loop until exitCode <> 0


    do
        call sayRecordingMode( FALSE )
        button = getRemoteControl( 100 )
        exitCode =  editVariable( recorderMode, button, 0, 3 )
    loop until exitCode <> 0


    if recorderMode = movementRecorder or recorderMode = extTriggerRecorder then
            '
            ' PIR or external trigger modes require to specify also
            ' the number of frames to take after the trigger ends
            '
            call say( "FRAMENUM" )
            do
                call sayNumber( numberOfFrames )
                button = getRemoteControl( 100 )
                exitCode = editVariable( numberOfFrames, button, 1, 50 )
            loop until exitCode <> 0

    elseif recorderMode = timerRecorder then
            '
            ' timer recording mode require to specify also
            ' the interval in minutes between snapshots
            '
            call say( "MINUTES" )
            do
                call sayNumber( intervalMinutes )
                button = getRemoteControl( 100 )
                exitCode = editVariable( intervalMinutes, button, 1, 100 )
            loop until exitCode <> 0
    endif
    '
    ' Set the new preferences and save on non-volatile memory
    '
    call camera_setup( cameraResolution )
    call saveEepromVariables()
    call say( "OK" )
end sub



'**************************************************************************************
'*  Generic editing routine.
'*  Edits a byte variable passed by reference pressing key_INCREASE and key_DECREASE.
'*    -  Navigates between menus pressing key_NEXT and key_PREVIOUS.
'*    -  Takes the variable to be edited, the button pressed on the remote control,
'*       the minimum and maximum range admissible for the value to be edited.
'*    -  Returns an exit code indicating editing status:
'*          0 to continue editing,
'*          1 for moving to next menu,
'*          2 for moving to previous menu,
'*          3 to exit menus.
'**************************************************************************************

function editVariable( variable as byte, byval button as byte, byval minValue as byte, byval maxValue as byte  ) as byte
    local returnValue as byte

    returnValue = 0

    select case button
    case Key_INCREASE:
            if variable < maxValue then
                variable = variable + 1
            else
                variable = minValue
            endif

    case Key_DECREASE:
            if variable > minValue then
                variable = variable - 1
            else
                variable = maxValue
            endif

    case Key_NEXT:
            returnValue = 1

    case Key_PREV:
            returnValue = 2

    case else
            returnValue = 3
    end select

    editVariable = returnValue
end function


'**************************************************************************************

remoteInitDone:     'statement required by BASCOM compiler
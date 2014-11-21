'**************************************************************************************
'*
'*   AT3267 - THE WITNESS CAMERA - SPEECH SYNTHESIS PRIMITIVES
'*
'**************************************************************************************
'*
'*  This module provides speech primitives to play files, spell numbers and dates,
'*  describe system status (resolution, recording modes...).
'*  It implements also a beep() that works even with the card removed, and
'*  an audio_off() routine to remove power from the DC-coupled speaker.
'*
'**************************************************************************************

declare sub say( byval filename as string )
declare sub sayNumber( byval number as byte )
declare sub say16bitNumber( byval number as word )
declare sub sayDateTime()
declare sub sayRecordingMode( byval includeDetails as byte )
declare sub sayResolution()
declare sub sayStatus()
declare sub sayMonth( m as byte )
declare sub sayYear( y as byte )
declare sub audioOff()


'**************************************************************************************
'*  Configure TIMER1 for PWM output, maximum frequency, 8-bit mode
'**************************************************************************************
config TIMER1 = Pwm , Prescale = 1 , Compare A Pwm = Clear Down , Pwm = 8

goto speechInitDone     ' This statement required by BASCOM compiler



'**************************************************************************************
'*  Plays a speech file given its filename.
'*  Do not include the .PCM extension to the filename
'**************************************************************************************

sub say( byval filename as string * 12 )
    local sample as byte                                ' Current sample being played.
    local busy as byte                                  ' Busy loop temporary counter.
                                                        '
    if diskInitialized = TRUE then                      ' Is disk available?
        chdir "\"                                       ' Yes, move to "\SPEECH" directory.
        chdir "SPEECH."                                 '
        filename = filename + ".PCM"                    ' Complete filename with extension.
        open filename for binary as #3                  ' Open speech file.
        print #1, filename; " ("; lof(#3); " bytes)"    ' Log filename and duration.
        while eof(#3) <> 255                            ' Any more samples?
            get #3 , sample                             ' Yes, get a sample from file
            PWM1A = sample                              ' and bang it to the PWM.
            for busy = 1 To 7                           ' Wait a while (just a rude
                NOP                                     ' busy-wait loop).
            next                                        '
        wend                                            ' And look for other samples.
        close #3                                        ' No more samples, close file.
    else                                                '
        call beep()                                     ' If disk in unavailable, just
    endif                                               ' play a tone instead.
    waitMs 100                                          ' Add a little pause between words
end sub



'**************************************************************************************
'*  Says a number (current range is 0 to 100)
'**************************************************************************************
sub sayNumber( byval number as byte )
    local filename as string * 12

    filename = str(number )
    call say( filename )
end sub



'**************************************************************************************
'*  Just like sayNumber, but takes a word parameter instead.
'*  Unfortunately BASCOM does not support automatic type conversions
'**************************************************************************************

sub say16bitNumber( byval number as word )
    local filename as string * 12

    filename = str(number )
    call say( filename )
end sub



'**************************************************************************************
'* Says system clock and calendar settings
'**************************************************************************************

sub sayDateTime()

    call sayMonth( _month )
    call sayNumber( _day )
    call sayYear( _year )
    waitMs 200

    call say( "TIME" )
    call sayNumber( _hour )
    call say( "HOURS" )
    call say( "AND" )
    call sayNumber( _min )
    call say( "MINUTES")
    call audioOff()
end sub



'**************************************************************************************
'* Says month name given its number, e.g. sayMonth( 1 ) --> "January"
'**************************************************************************************

sub sayMonth( m as byte )
    local monthname as string * 12

    monthname = "MONTH"
    if m < 10 then
        monthname = monthname + "0"
    endif
    monthname = monthname + str( m )
    call say( monthname )
end sub



'**************************************************************************************
'* Says year given its number as per internal 2-digit calendar format (06 = 2006)
'**************************************************************************************

sub sayYear( y as byte )
    local year as word

    year = 2000
    year = year + _year
    call say16bitNumber( year )
end sub



'**************************************************************************************
'* Says camera's resolution
'**************************************************************************************

sub sayResolution()
    if cameraResolution = vgaResolution then
        call say( "VGARES" )
    else
        call say( "CCTVRES" )
    endif
end sub



'**************************************************************************************
'* Says recorder's recording mode, with relevant details as number of frames for
'* trigger modes or timer interval
'**************************************************************************************

sub sayRecordingMode( byval includeDetails as byte )

    select case recorderMode

    case movementRecorder:
        call say("MOVEMENT")
        if includeDetails = TRUE then
            waitMs 100
            call say( "FRAMENUM" )
            call sayNumber( numberOfFrames )
        endif

    case timerRecorder:
        call say( "TIMER" )
        if includeDetails = TRUE then
            waitMs 100
            call say( "MINUTES" )
            call sayNumber( intervalMinutes )
        endif

    case extTriggerRecorder:
        call say("EXTTRIG")
        if includeDetails = TRUE then
            waitMs 100
            call say( "FRAMENUM" )
            call sayNumber( numberOfFrames )
        endif

    case continuousRecorder:
        call say( "CONTINUO" )

    end select
end sub



'**************************************************************************************
'* Camera status briefing
'**************************************************************************************

sub sayStatus()
    call sayDateTime()
    waitMs 200
    call sayRecordingMode( TRUE )
    waitMs 200
    call sayResolution()
end sub



'**************************************************************************************
'* Beeps the speaker with a short 500 Hz tone.
'* Does not need the file system, so it can be used even if the card is removed
'**************************************************************************************

sub beep()
    local i as byte
    config TIMER1 = Pwm , Prescale = 1 , Compare A Pwm = Clear Down , Pwm = 8
    for i=0 to 20
        PWM1A = 50
        waitMs 1
        PWM1A = 0
        waitMs 1
    next
end sub



'**************************************************************************************
'* Removes power to the speaker. Speaker is DC coupled, so a 50% PWM is a waste
'* of power even if it corresponds to silence.
'* To prevent annoying clicks, the routine ramps down to 0 gradually.
'**************************************************************************************

sub audioOff()
    local level as byte

    level = PWM1A
    while level > 0
        level = level - 1
        PWM1A = level
        waitMs 1
    wend

end sub



'**************************************************************************************
speechInitDone:     ' required by BASCOM compiler
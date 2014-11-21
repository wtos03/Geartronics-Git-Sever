'**************************************************************************************
'*
'*  AT3267 - THE WITNESS CAMERA - TIME-LAPSE RECORDER
'*
'**************************************************************************************
'*  This module implements the time-lapse recorder.
'*  Note: It is mandatory that the recorder be invoked only when the disk is already
'*  initialized.
'**************************************************************************************


'**************************************************************************************
'*  Public functions:
'**************************************************************************************

declare sub recorder()
declare sub saveEepromVariables()
declare sub loadEepromVariables()


'**************************************************************************************
'*  Variables
'**************************************************************************************

const movementRecorder    = 0       ' Record when PIR sensor triggers
const timerRecorder       = 1       ' Record every intervalMinutes
const extTriggerRecorder  = 2       ' Record when the trigger pin goes low
const continuousRecorder  = 3       ' Record continuosly
dim recorderMode as byte            ' Recorder mode of operation, can be one of above values


const vgaResolution  = 0            ' 640x480 = VGA resolution
const cctvResolution = 1            ' 320x200 = sort of VHS/CCTV resoution
dim cameraResolution as byte        ' resolution to use, can be one of above values

dim numberOfFrames as byte          ' Number of consecutive frames to take for each trigger

dim framesRemaining as byte         ' Frames to take in the recorder loop
framesRemaining = 0

'
' Local variables used for interval recording mode
'
dim elapsedSeconds    as long      ' Elased seconds from last snaphot
dim intervalSeconds   as long      ' Number of seconds between snapshots
dim intervalMinutes   as byte      ' Same as above, but expressed in minutes
dim lastShotTime      as long      ' Records the time of last snapshot
lastShotTime = 0



call loadEepromVariables()         ' automatically
goto recorderLoaded                ' required by BASCOM compiler



'**************************************************************************************
'*  Time lapse recorder. Uses and modifies global flags: pirFlag and extTriggerFlag.
'**************************************************************************************

sub recorder()

    select case recorderMode

        case continuousRecorder:
                framesRemaining = 1

        case movementRecorder:
                if pirFlag = 1 then
                    reset pirFlag
                    framesRemaining = numberOfFrames
                endif

        case extTriggerRecorder:
                if extTriggerFlag = 1 then
                    reset extTriggerFlag
                    if framesRemaining = 0 then
                        call beep()
                    endif
                    framesRemaining = numberOfFrames
                endif

        case timerRecorder:
                intervalSeconds = intervalMinutes
                intervalSeconds = intervalSeconds * 60
                elapsedSeconds  = sysSecElapsed( lastShotTime )
                if elapsedSeconds >= intervalSeconds then
                    lastShotTime = sysSec()
                    framesRemaining = 1
                endif
    end select

    if framesRemaining > 0 then                 ' Any frame to take?
        call recordPictureFile()                ' Yes, take it
        framesRemaining = framesRemaining - 1   ' Count it...
        call diskCleanUp( FALSE )               ' ...and perform a busy-cleanup
    else
        call diskCleanup( TRUE )                ' Otherwise do an idle-cleanup
    endif

end sub

'**************************************************************************************
'*  Load setup preferences from non-volatile memory.
'*  Detects automatically a freshly-programmed AVR-Mega32 chip, loading
'*  appropriate default values.
'**************************************************************************************

sub loadEepromVariables()
    local signature as word

    readEeprom signature, 1
    print #1, "Loading eeprom";
    if signature = 12345 then
        print #1, "signature OK"

        readEeprom recorderMode, 3
        readEeprom numberOfFrames, 4
        readEeprom intervalMinutes, 5
        readEeprom cameraResolution, 6
    else
        print #1, "using defaults"                ' Signature not found, means a
        recorderMode = movementRecorder           ' blank chip. Use defaults instead
        numberOfFrames = 5
        intervalMinutes = 1
        cameraResolution = cctvResolution
        call saveEepromVariables()
    endif
end sub


'**************************************************************************************
'*  Save setup preferences to non-volatile memory.
'**************************************************************************************

sub saveEepromVariables()
    local signature as word
    signature = 12345
    print #1, "Saving eeprom"
    writeEeprom signature, 1
    writeEeprom recorderMode, 3
    writeEeprom numberOfFrames, 4
    writeEeprom intervalMinutes, 5
    writeEeprom cameraResolution, 6
end sub


'**************************************************************************************

recorderLoaded:     ' required by BASCOM compiler
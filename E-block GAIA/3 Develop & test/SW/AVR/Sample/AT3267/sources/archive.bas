'**************************************************************************************
'*
'*  AT 3267 - THE WITNESS CAMERA - DISK ARCHIVE MANAGER
'*
'**************************************************************************************
'*  -  Creates new JPG files placing it in a folder according to date and time.
'*     New folders are created as necessary.
'*     Example: on 2006, August, 2 15:05:34
'*     ROOT/YEAR06/MONTH08/DAY02/HOUR15/MIN05/34.JPG
'*  -  Keeps the disk filled to capacity, deleting oldest JPG files if necessary.
'*  -  Provide a date and time guess from the most recent file, used to set the clock
'*     on first power-up or when replacing the batteries.
'**************************************************************************************

'**************************************************************************************
'*  Public functions
'**************************************************************************************

declare sub initializeArchive()
declare sub dateTimeGuess()
'Next two declarations moved to main module because of BASCOM scope rules
'declare sub recordPictureFile()
'declare sub diskCleanup( byval isIdle as byte )

'**************************************************************************************
'* Private functions
'**************************************************************************************
declare sub into_subdirectory( byval prefix as string, byval index as byte )
declare sub removeAncestor( byval prefix as string, byval number as byte )
declare sub findFirstFile()
declare sub findLastFile()
declare sub deleteOldFiles( byval filesToDelete as long )
declare function isDirectory() as byte
declare function buildName( prefix as string, byval number as byte ) as string
declare function changeDirectory( byval prefix as string, number as byte ) as byte
declare function scanDirectory( byval prefix as string, byval rangeFirst as byte , byval rangeLast as byte, returnValue as byte ) as byte
declare function deleteJpgFile( byval prefix as string, number as byte )as byte


dim firstYear    as byte    ' Date and time of the oldest file found on disk
dim firstMonth   as byte
dim firstDay     as byte
dim firstHour    as byte
dim firstMinute  as byte
dim firstSecond  as byte

dim lastYear     as byte    ' Date and time of the most recent file found on disk
dim lastMonth    as byte
dim lastDay      as byte
dim lastHour     as byte
dim lastMinute   as byte
dim lastSecond   as byte

                             ' Variables used for disk cleanup euristics

dim lastCleanUpTime as long  ' Time of last cleanup policy check
dim cleanUpInterval as long  ' Interval between cleanup policy checks
dim deleteOnWrite   as long  ' # of files to delete each time a new file is created
dim deleteWhenIdle  as word  ' # of files to delete only if the recorder is idle

goto archiveLoaded           ' statement required by BASCOM compiler



'**************************************************************************************
'* Init routine
'**************************************************************************************

sub initializeArchive()
    deleteOnWrite = 0
    deleteWhenIdle = 0
    cleanupInterval = 0
    lastCleanUpTime = sysSec()
    call findFirstFile()
    call findLastFile()
    call diskCleanUp( FALSE )
end sub



'**************************************************************************************
'*
'* Set the clock guessing a date/time from the last file on disk
'*
'**************************************************************************************

sub dateTimeGuess()
    _year   = lastYear
    _month  = lastMonth
    _day    = lastDay
    _hour   = lastHour
    _min    = lastMinute
    _sec    = lastSecond
    print #1 , "Clock set to " ; lastYear ; "/" ; lastMonth ; "/" ; lastDay ; " " ; lastHour; ":" ; lastMinute ; " +" ; lastSecond
end sub





'**************************************************************************************
'*
'* Disk cleanup heuristics. Called by the time-lapse recorder.
'*   -  If isIdle = FALSE it assumes an image is currently being taken, delaying any
'*      time-consuming delete operations. However, if there is very little disk space,
'*      delete operations may occur anyway.
'*   -  If isIdle = TRUE it assumes there is time for disk housekeeping, freeing
'*      some extra space just in case.
'*
'**************************************************************************************
const MINUTES_1  = 60
const MINUTES_5  = 300
const MINUTES_15 = 900
const MINUTES_30 = 1800
const HOUR_1     = 3600
const PREEMPTION = 1000

sub diskCleanup( byval isIdle as byte )

    local elapsed as long
    local freeKB as long

    elapsed = sysSecElapsed( lastCleanUpTime )                  ' time for a check?
    if elapsed > cleanupInterval then
        freeKB = diskFree()                                     ' yes, determine how many kilobytes free
        select case freeKB                                      ' in order to balance the number of files to delete

            ' IMPORTANT: do not alter the order of the following CASE statements
            ' as they MUST be evaluated from bigger to smaller values

            case is > 60000:    deleteOnWrite = 0               ' More than 60 MB free, keep
                                deleteWhenIdle = 0              ' all of the files,
                                cleanupInterval = HOUR_1        ' very relaxed checks

            case is > 30000:    deleteOnWrite = 0               ' More than 30 MB free,
                                deleteWhenIdle = PREEMPTION     ' delete old stuff only when idle
                                cleanupInterval = MINUTES_30    ' but schedule more frequent checks

            case is > 20000:    deleteOnWrite = 1               ' More than 20 MB free, delete 1 file before
                                deleteWhenIdle = PREEMPTION     ' recording new stuff, possibly more when idle
                                cleanupInterval = MINUTES_30    ' next check in 30 minutes

            case is > 15000:    deleteOnWrite = 2               ' About 15 MB free, delete 2 files every time,
                                deleteWhenIdle = PREEMPTION     ' possibly more when idle,
                                cleanupInterval = MINUTES_15    ' next check in 15 minutes

            case is > 10000:    deleteOnWrite = 3               ' Just 10 MB free, delete 3 files at a time,
                                deleteWhenIdle = PREEMPTION     ' possibly more when idle,
                                cleanupInterval = MINUTES_15    ' next check in 15 minutes

            case is > 5000:     deleteOnWrite = 10              ' Disk almost full, delete at least 10 files,
                                deleteWhenIdle = PREEMPTION     ' possibly more when idle,
                                cleanupInterval = MINUTES_5     ' check again very soon

            case is > 2000:     deleteOnWrite = 30              ' Disk is full, concentrate on deleting
                                deleteWhenIdle = PREEMPTION     ' old stuff, keep checking
                                cleanupInterval = MINUTES_5     ' check again very soon

            case else:          deleteOnWrite = 100             ' You should never get here: almost no
                                deleteWhenIdle = PREEMPTION     ' space left, give maximum priority to
                                cleanupInterval = MINUTES_1     ' the delete process.
        end select

        lastCleanUpTime = sysSec()
        print #1, "Disk cleanup at "; time$; ", "; freeKB; " kB free, delete level: "; deleteOnWrite; " extra files: "; deleteWhenIdle; " interval: "; cleanUpInterval
    endif

    call deleteOldFiles( deleteOnWrite )
    if isIdle = TRUE and deleteWhenIdle > 0 then
        call deleteOldFiles( 1 )
        deleteWhenIdle = deleteWhenIdle - 1
    endif

end sub



'**************************************************************************************
'*
'* Archives a .JPG picture in the sorted directory tree, creating new directories
'* if necessary
'*
'**************************************************************************************

sub recordPictureFile()
    Local filename As String * 12

    ' Start from root directory...
    chdir "\"

    ' ...and move to the subdirectory matching current year, month, day, hour & minute
    ' If such a directory does not exist, into_subdirectory() will create it.
    call into_subdirectory( "YEAR",  _year  )
    call into_subdirectory( "MONTH", _month )
    call into_subdirectory( "DAY",   _day   )
    call into_subdirectory( "HOUR",  _hour  )
    call into_subdirectory( "MIN",   _min   )

    ' Current file position is now set to a path matching date and time.
    ' Create a .JPG file named after the seconds and save it.
    filename = "SEC"
    filename = buildName( fileName, _sec )
    filename = fileName + ".JPG"
    print #1, "Writing file: "; filename
    diskWriteLed = 1
    open filename for binary as #3
        camera_snapshot
        flush #3
    close #3
    diskWriteLed = 0
end sub



'**************************************************************************************
'* Like chdir (change directory), but creates a new directory if the directory
'* does not exist.
'* The directory is specified as a prefix+index pair.
'* Example:
'*         into_subdirectory( "DAY", 25 ) --> create directory "DAY25" and
'*                                            jump into it.
'**************************************************************************************

sub into_subdirectory( byval prefix as string * 12, byval index as byte )

    local targetName as string * 12
    local subDirectory as string * 12

    targetName = buildName( prefix, index )                 'Build target subdirectory name...
    subDirectory = dir(targetName )                         '...and look for it.
    if len( subDirectory ) = 0 then                         'Is the target subdirectory already on disk?
        print #1, "Creating ["; targetName; "]"
        mkdir targetName                                    'No, create it.
    end if
    chdir targetName                                        'Move into subdirectory.
end sub



'**************************************************************************************
'*
'* Orderly scans the entire filesystem from oldest to newer files.
'* Keeps deleting files until the given file count is done.
'*
'**************************************************************************************

sub deleteOldFiles( byval filesToDelete as long )

    local filesDeleted as long
    local year as byte
    local month as byte
    local day as byte
    local hour as byte
    local minute as byte
    local second as byte


    filesDeleted = 0

    ' Starting from root directory, orderly scan through
    ' years, months, days, hours, minutes.
    ' Apparently is slow, but as files are organized
    ' in a sorted directory tree, it won't take much
    ' to get to the job done.

    if filesToDelete = 0 then
        goto finished
    endif

    print # 1, "Deleting old stuff ( up to "; filesToDelete; " files )...";

    chdir "\"
    for year = 0 to 50
        'print #1, "for year "; year
        if filesDeleted >= filesToDelete then
            goto finished
        endif
        if changeDirectory( "YEAR", year ) = TRUE then
            for month = 1 to 12
                'print #1, "for month "; month
                if filesDeleted >= filesToDelete then
                    goto finished
                endif
                if changeDirectory( "MONTH", month ) = TRUE then
                    for day = 1 to 31
                        'print #1, "for day "; day
                        if filesDeleted >= filesToDelete then
                            goto finished
                        endif
                        if changeDirectory( "DAY", day ) = TRUE then
                            for hour = 0 to 23
                                'print #1, "for hour "; hour
                                if filesDeleted >= filesToDelete then
                                    goto finished
                                endif
                                if changeDirectory( "HOUR", hour ) = TRUE then
                                    for minute = 0 to 59
                                        'print #1, "for minute "; minute
                                        if filesDeleted >= filesToDelete then
                                            goto finished
                                        endif
                                        if changeDirectory( "MIN", minute ) = TRUE then
                                            for second = 0 to 59
                                                'print #1, "for second "; second
                                                if filesDeleted >= filesToDelete then
                                                    goto finished
                                                endif
                                                if deleteJpgFile( "SEC", second ) = TRUE then
                                                    filesDeleted = filesDeleted + 1
                                                    'print #1, "deleted "; second; ".JPG"
                                                endif
                                            next second
                                            call removeAncestor( "MIN", minute )
                                        endif
                                    next minute
                                    call removeAncestor( "HOUR", hour )
                                endif
                            next hour
                            call removeAncestor( "DAY", day )
                        end if
                    next day
                    call removeAncestor( "MONTH", month )
                end if
            next month
            call removeAncestor( "YEAR", year )
        endif
    next year

finished:

end sub




'**************************************************************************************
'*
'* Given a prefix/number pair, deletes a a .JPG file from current directory.
'* Returns TRUE if file was found and deleted.
'*
'* Example: deleteJpgFile( "SEC", 5 ) --> deletes SEC05.JPG
'*
'**************************************************************************************

function deleteJpgFile( byval prefix as string * 12, number as byte )as byte
    local fileName as string * 12
    local retval as byte

    retval = FALSE
    fileName = buildName( prefix, number )
    fileName = fileName + ".JPG"
    filename = dir( filename )
    if len( filename ) > 0 then
        print #1, "kill "; fileName
        kill fileName
        retval = TRUE
    endif
    deleteJpgFile = retval
end function



'**************************************************************************************
'* Goes up one level on the directory tree, and prunes current directory.
'* Note: current directory must be empty.
'**************************************************************************************

sub removeAncestor( byval prefix as string * 12, byval number as byte )
    local dirName as string * 12
    chdir "."
    dirName = buildName( prefix, number )
    rmdir dirName
    print # 1, "remove directory "; dirName
end sub



'**************************************************************************************
'*
'* Starting from current directory position, orderly scans the subdirecories in the
'* given range. Stops as it finds one, making it the current filesystem position.
'* Can search backwards (from newer to older), if you give it a range whose first is
'* greater than last.
'*
'* Returns TRUE and sets reference parameter if a matching file or directory is found,
'* otherwise returns FALSE and sets "returnValue" parameter to zero
'*
'* example: scanDirectory( "MONTH", 3, 7, found )
'*
'*          Searches from March to July; supposing the disk has directories
'*          for May and June only, it sets the current directory to May and
'*          returns TRUE.
'*
'**************************************************************************************

function scanDirectory( byval prefix as string * 12, byval rangeFirst as byte , byval rangeLast as byte, returnValue as byte ) as byte

    local index as integer
    local rF as integer
    local rL as integer
    local direction as integer
    local directory as string * 12
    local filename as string * 12
    local found as byte
    local temp as byte

    found = FALSE
    if rangeFirst > rangeLast then                          ' Determine if we are searching upwards
        direction = -1                                      ' or downwards.
    else
        direction = 1
    endif

    rF = rangeFirst                                         ' BASCOM needs integer variables for the
    rL = rangeLast                                          ' decrement to work; with bytes the FOR would fail.
    for index = rF to rL step direction                     ' Browse through the given directory range.
        returnValue = low(index)                            ' BASCOM needs explicit type conversion.
        found =  changeDirectory( prefix, returnValue )     ' Any subdirectory found?
        if found = TRUE then
            exit for                                        ' Yes, no need to search anymore.
        endif
    next
    scanDirectory = found
end function




'**************************************************************************************
'*
'* If the subidrectory exists, make it the current directory and return TRUE.
'*
'**************************************************************************************

function changeDirectory( byval prefix as string * 12, number as byte ) as byte
    local dirName as string * 12
    local found  as string * 12
    local retval as byte
    dirName = buildName( prefix, number )
    found = dir( dirName )
    if len( found ) > 0 then                        'Any subdirectory found?
        chdir dirName                               'Yes, move into it...
        retval = TRUE                               '...and return TRUE.
    else
        retval = FALSE                              'No, return FALSE.
    endif
    changeDirectory = retval
end function



'**************************************************************************************
'*
'* Returns TRUE if current the last call to dir() returned a directory,
'*         FALSE if it returned a file
'*
'**************************************************************************************

function isDirectory() as byte

    local attr as byte
    local retval as byte

    attr = getAttr()
    retval = FALSE
    if attr.4 = 1 then
        retval = TRUE
    endif
    isDirectory = retval
end function



'**************************************************************************************
'*
'* Scans entire disk to set the date for the oldest .JPG file found
'*
'**************************************************************************************

sub findFirstFile()
    local unused as byte

    firstYear   = 6
    firstMonth  = 1
    firstDay    = 1
    firstHour   = 0
    firstMinute = 0
    firstSecond = 0

    chdir "\"     'start from root directory
    if scanDirectory( "YEAR", 0, 50, firstYear ) = TRUE then
        if scanDirectory( "MONTH", 1, 12, firstMonth ) = TRUE then
            if scanDirectory( "DAY", 1, 31, firstDay ) = TRUE then
                if scanDirectory( "HOUR", 0, 23, firstHour ) = TRUE then
                    if scanDirectory( "MIN", 0, 59, firstMinute ) = TRUE then
                        unused = scanDirectory( "SEC", 0, 59, firstSecond )
                    endif
                endif
            endif
        endif
    endif
    print #1 , "FIRST FILE: " ; firstYear ; "/" ; firstMonth ; "/" ; firstDay ; " " ; firstHour ; ":" ; firstMinute ; " +" ; firstSecond
end sub



'**************************************************************************************
'*
'* Scan entire disk to set the date of the most recent .JPG file found
'*
'**************************************************************************************

sub findLastFile()
    local unused as byte

    LastYear   = 6
    LastMonth  = 1
    LastDay    = 1
    LastHour   = 0
    LastMinute = 0
    LastSecond = 0

    chdir "\"                                         'start from root directory
    if scanDirectory( "YEAR", 50, 0, LastYear ) = TRUE then
        if scanDirectory( "MONTH", 12, 1, LastMonth ) = TRUE then
            if scanDirectory( "DAY", 31, 1, LastDay ) = TRUE then
                if scanDirectory( "HOUR", 23, 0, LastHour ) = TRUE then
                    if scanDirectory( "MIN", 59, 0, LastMinute ) = TRUE then
                        unused = scanDirectory( "SEC", 59, 0, LastSecond )
                    endif
                endif
            endif
        endif
    endif

    print #1 , "LAST FILE: " ; lastYear ; "/" ; lastMonth ; "/" ; lastDay ; " " ; lastHour; ":" ; lastMinute ; " +" ; lastSecond
end sub



'**************************************************************************************
'*
'* Utility function, returns a filename from a prefix + index pair,
'* eg. buildName( "MONTH", 5 ) -> "MONTH05"
'*
'**************************************************************************************

function buildName( prefix as string * 12, byval number as byte ) as string * 12
    buildName = prefix
    if number < 10 then
        buildName = buildName + "0"
    endif
    buildName = buildName + str( number )
end function


'**************************************************************************************

archiveLoaded:      ' statement required by BASCOM compiler
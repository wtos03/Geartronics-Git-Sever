'**************************************************************************************
'*
'*  AT3267 - THE WITNESS CAMERA - CAMERA DRIVER
'*
'**************************************************************************************
'*  This module implements control of an Intertech ITM-C-328 camera module.
'*  Assumes the camera is connected to the input/output stream #2.
'*  Can write on the debug/log channel on stream #1.
'**************************************************************************************


'**************************************************************************************
'*  Public functions:
'**************************************************************************************

declare sub camera_initialize()
declare sub camera_setup( byval resolution as byte )
declare sub camera_connect()
declare sub camera_snapshot()


'**************************************************************************************
'* Private functions
'**************************************************************************************

declare sub send(byval b1 as byte , byval b2 as byte , byval b3 as byte , byval b4 as byte , byval b5 as byte , byval b6 as byte )
declare sub receive(b1 as byte , b2 as byte , b3 as byte , b4 as byte , b5 as byte , b6 as byte )
declare sub acknowledge()

goto camera_done    ' required by BASCOM compiler


'**************************************************************************************
'*  Connects to the camera and initializes parameters (resolution, packet size)
'**************************************************************************************

sub camera_initialize()
    call camera_setup( cameraResolution )
end sub



'**************************************************************************************
'*  (Re) connects and resynchronize to the camera, possibly waking it
'*  up from auto-power save feature.
'**************************************************************************************

sub camera_connect()
    local retries as integer

    print #1, "Connect: try #" ;
    for retries = 1 to 1000                             ' Keep trying
        call send(&HAA , &H0D , 0 , 0 , 0 , 0 )         ' sending SYNC packets
        print #1, retries; " ";
        if isCharWaiting(#2) = TRUE then
            call acknowledge()                          ' until camera responds with an ACK
            call acknowledge()                          ' then a SYNC packet.
            exit for
        endif
    next
    call send(&HAA , &H0E , &H0D , 0 , 0 , 0 )          ' Confirm SYNC with an ACK packet.
    print #1, "done."
End sub



'**************************************************************************************
'*  Connects to the camera and sets the resolution given.
'*  Sets buffer size for picture data transfers to 64 bytes.
'*  Resolution can be one of two values: vgaResolution (640x480)
'*  and cctvResolution (320x200)
'**************************************************************************************

sub camera_setup( byval resolution as byte )

    ' Wake-up and resync to camera.
    call camera_connect()

    ' Set picture resolution.
    if resolution = vgaResolution then
        print #1, "setting VGA resolution"
        call send(&HAA , &H01 , &H00 , &H07 , &H07 , &H07 )       '640x480
    else
        print #1, "setting CCTV resolution"
        call send(&HAA , &H01 , &H00 , &H07 , &H03 , &H05 )       '320x200
    endif
    call acknowledge()

    ' Set buffer size to 64 bytes.
    call send(&HAA , &H06 , &H08 , &H40 , &H00 , &H00 )
    call acknowledge()
End sub


'**************************************************************************************
'*  Takes a snapshot and dumps picture data to input/output stream #3
'*  The caller must open the stram prior to call - this routine knows nothing
'*  of where data is actually going.
'**************************************************************************************

sub camera_snapshot()
    local b1 as byte                ' buffers for response packet: byte 1
    local b2 as byte                ' byte 2
    local b3 as byte                ' byte 3
    local b4 as byte                ' byte 4
    local b5 as byte                ' byte 5
    local b6 as byte                ' byte 6
    local fileLength as long        ' total JPG file length (data is fragmented in packets)
    local bytes as long             ' counts # of data bytes received
    local packetLength as byte      ' received packet length
    local packet as word            ' current incoming packet number
    local lsb as byte               ' least significant byte of current packet number
    local msb as byte               ' most  significant byte of current packet number
    local i as byte                 ' index of current byte in incoming data packet
    local temp as byte              ' temporary - used for trashing unused characters

    call camera_connect()           ' reconnect to camera (wakes-up camera
                                    ' if it entered automatic power saving mode)

    print #1 , "Snaphot: ";                                     ' Log event.
    call send(&HAA , &H05 , &H00 , &H00 , &H00 , &H00 )         ' Send SNAPSHOT command...
    waitmS 2                                                    ' ...give time for taking picture
    call acknowledge()                                          ' ...and get acknowledge.
    call send(&HAA , &H04 , &H01 , &H00 , &H00 , &H00 )         ' Request snapshot data header:
    call acknowledge()                                          ' get acknowledge first...
    call receive(b1 , b2 , b3 , b4 , b5 , b6 )                  ' ...then command response.
    waitmS 2
    call send(&HAA , &H0E , &H00 , &H00 , &H00 , &H00 )
    fileLength = b6                                             ' compute file length (tricky: the
    fileLength = fileLength * 256                               ' compiler needs some help reducing
    fileLength = fileLength + b5                                ' expressions and doing automatic
    fileLength = fileLength * 256                               ' type conversions).
    fileLength = fileLength + b4
    print #1 , "length = " ; fileLength;                        ' Log file length
    waitmS 2
    bytes = 0
    packet = 0
    while bytes < fileLength                                    ' Receive all the bytes, packet after
        temp = waitkey(#2)                                      ' packet, byte after byte.
        temp = waitkey(#2)
        packetLength = waitkey(#2)                              ' Get packet length (usually the same as
        temp = waitkey(#2)                                      ' buffer length,can be less on last packet).
        for i = 1 to packetLength
            temp = waitkey(#2)                                  ' Receive and dump all packetLength bytes.
            put #3 , temp
        next
        temp = waitkey(#2)                                      ' Skip CRC.
        temp = waitkey(#2)                                      '
        bytes = bytes + packetLength                            ' Account for bytes just received.
        packet = packet + 1
        lsb = low(packet )
        msb = high(packet )
        if bytes = filelength then                              ' Any more data left?
            print #1 , "."                                      ' Log file complete
            call send(&HAA , &H0E , &H00 , &H00 , &HF0 , &HF0 ) ' Send END packet before exit.
        else
            call send(&HAA , &H0E , &H00 , &H00 , lsb , msb )   ' Acknowledge packet and continue.
        endif
    wend
End sub



'**************************************************************************************
'*  Sends a 6-byte long packet to the camera.
'**************************************************************************************

sub send(b1 as byte , b2 as byte , b3 as byte , b4 as byte , b5 as byte , b6 as byte )
    print #2 , chr(b1);
    print #2 , chr(b2);
    print #2 , chr(b3);
    print #2 , chr(b4);
    print #2 , chr(b5);
    print #2 , chr(b6);
end sub



'**************************************************************************************
'*  Receive a 6-bytes long packet. Returns values through reference parameters.
'**************************************************************************************

sub receive(b1 as byte , b2 as byte , b3 as byte , b4 as byte , b5 as byte , b6 as byte )
    b1 = waitKey(#2)
    b2 = waitKey(#2)
    b3 = waitKey(#2)
    b4 = waitKey(#2)
    b5 = waitKey(#2)
    b6 = waitKey(#2)
end sub



'**************************************************************************************
'*  Receives an ACKNOWLEDGE packet. Logs a message on the debug stream in
'*  case of errors.
'**************************************************************************************

sub acknowledge()
    local b1 as byte
    local b2 as byte
    local b3 as byte
    local b4 as byte
    local b5 as byte
    local b6 as byte
    local error as byte
    call receive(b1 , b2 , b3 , b4 , b5 , b6 )
    error = 0
    if b1 <> &HAA then
            error = 1
    endif
    if b2 <> &H0E then
        error = 1
    endif
    if error = 1 then
        print #1, "NACK: "; hex(b1); " "; hex(b2); " "; hex(b3); " "; hex(b4); " "; hex(b5); " "; hex(b6)
    endif
end sub




'**************************************************************************************

camera_done:     ' required by BASCOM compiler
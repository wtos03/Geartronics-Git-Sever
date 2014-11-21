package dio_test;

import java.io.*;
import javax.comm.*;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author unascribed
 * @version 1.0
 */

public class dio_test
{
  private static CommPortIdentifier cpi;
  private static SerialPort comm_port;
  private static InputStream inputStream;
  private static OutputStream outputStream;

  private static UByteBuffer tx_cmd = new UByteBuffer( 10 );

  static int PORTA = 0x1B;
  static int DDRA  = 0x1A;
  static int PINA  = 0x19;
  static int PORTB = 0x18;
  static int DDRB  = 0x17;
  static int PINB  = 0x16;
  static int PORTC = 0x15;
  static int DDRC  = 0x14;
  static int PINC  = 0x13;
  static int PORTD = 0x12;
  static int DDRD  = 0x11;
  static int PIND  = 0x10;

  static int[] port_out_addr = { PORTA, PORTB, PORTC, PORTD };
  static int[] port_in_addr  = { PINA, PINB, PINC, PIND };

  static int CMD_WRITE_REG = 0x57;
  static int CMD_READ_REG  = 0x52;
  static int CMD_GET_ERROR = 0x45;

  //Errors
  static int ERROR_NONE                       = 0;
  static int ERROR_PORT_IS_OPEN               = 1;
  static int ERROR_PORT_NOT_AVAILABLE1        = 2;
  static int ERROR_PORT_NOT_AVAILABLE2        = 3;
  static int ERROR_PORT_NOT_AVAILABLE3        = 4;
  static int ERROR_NO_LINK                    = 5;
  static int ERROR_UNABLE_TO_SET_COMM_PARMS   = 6;
  static int ERROR_SERIAL_PORT_READ           = 7;


  public static void main(String[] args)
  {
    boolean thread_exit = false;

    //Setup the serial port
    int error = SetupSerialPort( 1 );
    if( error != ERROR_NONE )
    {
      System.out.println( "Serial port error: " + error );
      System.exit( 0 );
    }

    //Start formatting the command message
    tx_cmd.PutI( 0, 0x02 );
    tx_cmd.PutI( 4, 0x03 );
    tx_cmd.SetNumBytes( 5 );

    //Test suite start
    System.out.println( "dio_test" );
    System.out.println( "Tests the functionality of writing and reading digital I/O in the avr_io project" );
    System.out.println( "" );

    System.out.println( "\nStep 1 - write ports as digital outputs" );
    TestWriteDigitalOutputs();

    System.out.println( "\nStep 2 - check start frame error" );
    TestStartFrameError();

    System.out.println( "\nStep 3 - check end frame error" );
    TestEndFrameError();

    System.out.println( "\nStep 4 - check invalid command error" );
    TestInvalidCommandError();

    System.out.println( "\nStep 5 - test receive byte timeout" );
    TestRxTimeout();

    System.out.println( "\nStep 6 - read ports as digital inputs" );
    TestReadDigitalInputs();

    /*TBD
    System.out.println( "\nStep 4 - read a/d" );
    TestReadAD();
    */

    //Test suite finish

    //Close the serial port
    CloseSerialPort();

    System.out.println( "dio_test Done" );
  }

  /****************************************************************************
   WriteRegister()
  ****************************************************************************/
  static int WriteRegister( int register, int data )
  {
    tx_cmd.PutI( 1, CMD_WRITE_REG );
    tx_cmd.PutI( 2, register );
    tx_cmd.PutI( 3, data );
    SendCommand();

    return( 0 );
  }

  /****************************************************************************
   ReadRegister()
  ****************************************************************************/
  static int ReadRegister( int register )
  {
    //Make sure no data is pending in the input stream
    try
    {
      if( inputStream.available() > 0 )
      {
        java.awt.Toolkit.getDefaultToolkit().beep();
        System.out.println( "ERROR - ReadRegister: input stream not empty" );
        return( 0x100 );
      }
    }
    catch( IOException e )
    {
    }

    //Send read register command
    tx_cmd.PutI( 1, CMD_READ_REG );
    tx_cmd.PutI( 2, register );
    tx_cmd.PutI( 3, 0 );
    SendCommand();

    try
    {
      //Get the response
      long timeout = 500 + System.currentTimeMillis();
      while( inputStream.available() == 0 )
      {
        if( System.currentTimeMillis() > timeout )
        {
          return( 0x100 );
        }

      }

      //Good data
      return( inputStream.read() );
    }
    catch( IOException e )
    {
      java.awt.Toolkit.getDefaultToolkit().beep();
      System.out.println( "Error while getting response" );
    }
    return( 0x100 );
  }


  /****************************************************************************
   TestStartFrameError()
  ****************************************************************************/
  static void TestStartFrameError()
  {
    int avr_error = 0;

    System.out.println( "Hardware: LEDs on port D" );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Hit ENTER key to cont." );
    WaitForUserKeyInput();

    //Set all port D as all outputs, set all LEDs on
    WriteRegister( DDRD, 0xFF );
    WriteRegister( PORTD, 0x00 );

    //Send packet with bad STX
    tx_cmd.PutI( 0, 0x21 );                                                     //Bad STX
    tx_cmd.PutI( 1, CMD_WRITE_REG );
    tx_cmd.PutI( 2, PORTD );
    tx_cmd.PutI( 3, 0x0F );
    try
    {
      outputStream.write( tx_cmd.BufferDirect(), 0, tx_cmd.available() );       //Send packet with bad STX
      long timeout = System.currentTimeMillis() + 20;                           //Wait 20msec before requesting the error
      while( System.currentTimeMillis() < timeout )
      {
        ;
      }
    }
    catch( IOException e )
    {
      System.out.println( "Error sending command" );
    }

    //Restore STX byte
    tx_cmd.PutI( 0, 0x02 );

    //Get error
    avr_error = GetErrorFromAVR_IO();
    System.out.print( "(" + avr_error + ")" );                                  //Show the error number
    if( avr_error == 1 )                                                        //error 1 = start frame error
    {
      System.out.println( "error code is CORRECT" );
    }
    else
    {
      System.out.println( "error code is NOT CORRECT" );
    }
    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Verify error code. Verify all LEDs: 11111100.  Hit ENTER to cont." );
    WaitForUserKeyInput();

    //Now send a good cmd
    WriteRegister( PORTD, 0x0F );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Verify LEDs: 11110000.  Hit ENTER to end test" );
    WaitForUserKeyInput();
  }


  /****************************************************************************
   TestEndFrameError()
  ****************************************************************************/
  static void TestEndFrameError()
  {
    int avr_error = 0;

    System.out.println( "Hardware: LEDs on port D" );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Hit ENTER key to cont." );
    WaitForUserKeyInput();

    //Set all port D as all outputs, set all LEDs on
    WriteRegister( DDRD, 0xFF );
    WriteRegister( PORTD, 0x00 );

    //Send a packet with bad ETX
    tx_cmd.PutI( 1, CMD_WRITE_REG );
    tx_cmd.PutI( 2, PORTD );
    tx_cmd.PutI( 3, 0x0F );
    tx_cmd.PutI( 4, 0x33 );                                                     //Bad ETX
    try
    {
      outputStream.write( tx_cmd.BufferDirect(), 0, tx_cmd.available() );       //Send packet with bad ETX
      long timeout = System.currentTimeMillis() + 20;                           //Wait 20msec before requesting the error
      while( System.currentTimeMillis() < timeout )
      {
        ;
      }
    }
    catch( IOException e )
    {
      System.out.println( "Error sending command" );
    }

    //Restore STX byte
    tx_cmd.PutI( 4, 0x03 );

    //Get the error
    avr_error = GetErrorFromAVR_IO();
    System.out.print( "(" + avr_error + ")" );                                  //Show the error number
    if( avr_error == 2 )                                                        //error 2 = end frame error
    {
      System.out.println( "error code is CORRECT" );
    }
    else
    {
      System.out.println( "error code is NOT CORRECT" );
    }
    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Verify error code. Verify all LEDs: 11111100.  Hit ENTER to cont." );
    WaitForUserKeyInput();

    //Now send a good cmd
    WriteRegister( PORTD, 0x0F );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Verify LEDs: 11110000.  Hit ENTER to end test" );
    WaitForUserKeyInput();
  }


  /****************************************************************************
   TestInvalidCommandError()
  ****************************************************************************/
  static void TestInvalidCommandError()
  {
    int avr_error = 0;

    System.out.println( "Hardware: LEDs on port D" );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Hit ENTER key to cont." );
    WaitForUserKeyInput();

    //Set all port D as all outputs, set all LEDs on
    WriteRegister( DDRD, 0xFF );
    WriteRegister( PORTD, 0x00 );

    //Send packet with invalid command id
    tx_cmd.PutI( 1, 0xF5 );                                                     //0xF5 is not a valid command
    tx_cmd.PutI( 2, PORTD );
    tx_cmd.PutI( 3, 0x0F );
    try
    {
      outputStream.write( tx_cmd.BufferDirect(), 0, tx_cmd.available() );       //Send packet with bad command
      long timeout = System.currentTimeMillis() + 20;                           //Wait 20msec before requesting the error
      while( System.currentTimeMillis() < timeout )
      {
        ;
      }
    }
    catch( IOException e )
    {
      System.out.println( "Error sending command" );
    }

    //Get the error, verify it.
    avr_error = GetErrorFromAVR_IO();
    System.out.print( "(" + avr_error + ")" );                                  //Show the error number
    if( avr_error == 3 )                                                        //error 3 = invalid command error
    {
      System.out.println( "error code is CORRECT" );
    }
    else
    {
      System.out.println( "error code is NOT CORRECT" );
    }
    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Verify error code. Verify all LEDs: 11111100.  Hit ENTER to cont." );
    WaitForUserKeyInput();

    //Now send a good cmd
    WriteRegister( PORTD, 0x0F );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Verify LEDs: 11110000.  Hit ENTER to end test" );
    WaitForUserKeyInput();
  }


  /****************************************************************************
   TestRxTimeout()
  ****************************************************************************/
  static void TestRxTimeout()
  {
    int avr_error = 0;

    System.out.println( "Test: Receive timeout" );
    System.out.println( "Hardware: one LED on port D, pin 7" );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Hit ENTER to start the test:" );
    WaitForUserKeyInput();

    //Set all port D as all outputs, set all LEDs on
    WriteRegister( DDRD, 0xFF );
    WriteRegister( PORTD, 0x00 );

    //Part A
    System.out.print( "Part A - partial packet: " );
    //Send STX,command, wait for 20msec, then check to make sure "timeout error" is the current error
    tx_cmd.PutI( 1, CMD_WRITE_REG );
    tx_cmd.PutI( 2, PORTD );
    tx_cmd.PutI( 3, 0x80 );
    try
    {
      outputStream.write( tx_cmd.BufferDirect(), 0, 2 );
      long timeout = System.currentTimeMillis() + 20;
      while( System.currentTimeMillis() < timeout )
      {
        ;
      }
      avr_error = GetErrorFromAVR_IO();
    }
    catch( IOException e )
    {
      System.out.println( "Error sending command" );
    }
    System.out.print( "(" + avr_error + ")" );
    if( avr_error == 4 )
    {
      System.out.println( "error code is CORRECT" );
    }
    else
    {
      System.out.println( "error code is NOT CORRECT" );
    }
    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Hit ENTER to cont." );
    WaitForUserKeyInput();

    //Part B
    System.out.println( "Part B - delay between bytes" );
    //Send STX,command, wait for 20msec, then send register, data, ETX (Try to set bit 7 off
    tx_cmd.PutI( 1, CMD_WRITE_REG );
    tx_cmd.PutI( 2, PORTD );
    tx_cmd.PutI( 3, 0x80 );
    try
    {
      outputStream.write( tx_cmd.BufferDirect(), 0, 2 );
      long timeout = System.currentTimeMillis() + 20;
      while( System.currentTimeMillis() < timeout )
      {
        ;
      }
      outputStream.write( tx_cmd.BufferDirect(), 2, 3 );
    }
    catch( IOException e )
    {
      System.out.println( "Error sending command" );
    }
    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Verify that the PD7 is ON. Press ENTER to cont." );
    WaitForUserKeyInput();

    //Now, set LED off, no delay
    WriteRegister( PORTD, 0x80 );
    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Verify that the PD7 is OFF. Remove hardware. Hit ENTER to end test" );
    WaitForUserKeyInput();
  }


  /****************************************************************************
   GetErrorFromAVR_IO()
  ****************************************************************************/
  static int GetErrorFromAVR_IO()
  {
    tx_cmd.PutI( 1, CMD_GET_ERROR );                                            //Don't care about register & data bytes, just leave them
    SendCommand();

    try
    {
      //Get the response
      long timeout = 500 + System.currentTimeMillis();
      while( inputStream.available() == 0 )
      {
        if( System.currentTimeMillis() > timeout )
        {
          java.awt.Toolkit.getDefaultToolkit().beep();
          System.out.println( "ERROR: no response from AVR_IO" );
          return( 0x100 );
        }

      }

      //Good data
      return( inputStream.read() );
    }
    catch( IOException e )
    {
      java.awt.Toolkit.getDefaultToolkit().beep();
      System.out.println( "Error while getting response" );
    }
    return( 0x100 );
  }


  /****************************************************************************
   TestWriteDigitalOutputs()
  ****************************************************************************/
  static void TestWriteDigitalOutputs()
  {
    boolean skip_test = false;

    System.out.println( "Test: Write Digital Outputs, (hit ENTER for each test, 'q' and ENTER to quit)" );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.println( "Connect LEDs to ports, hit ENTER when ready." );
    WaitForUserKeyInput();

    //Set all ports as outputs
    WriteRegister( DDRA, 0xFF );
    WriteRegister( DDRB, 0xFF );
    WriteRegister( DDRC, 0xFF );
    WriteRegister( DDRD, 0xFF );

    WriteRegister( PORTA, 0x00 );
    WriteRegister( PORTB, 0x00 );
    WriteRegister( PORTC, 0x00 );
    WriteRegister( PORTD, 0x00 );

    for( int i = 0; i < 4; i++ )
    {
      int test_byte = 1;
      for( int b = 0; b < 9; b++ )
      {
        //Wait for user to hit space bar
        System.out.print( "Ready for dio_test/WriteDigitalOutputs/" + i + "/" + b + ":" );
        if( ((i == 3) && ((b == 0) || (b == 1))) ||                             //UART pins
            ((i == 1) && ((b == 5) || (b == 6) || (b == 7))) )                  //The STK200 has extra resistors on port B, pins 5 and 7, which will affect the signal; so lets just skip pins 5-7.
        {
          java.awt.Toolkit.getDefaultToolkit().beep();
          System.out.print( "****Port pin not available****" );
          skip_test = true;
        }
        WaitForUserKeyInput();

        if( skip_test == false )
        {
          int error = WriteRegister( port_out_addr[i], test_byte );
          if( error != 0 )
          {
            java.awt.Toolkit.getDefaultToolkit().beep();
            System.out.println( "ERROR from avr_io: " + error );
            return;
          }
        }
        skip_test = false;
        test_byte <<= 1;
      }
    }

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.println( "Test done, remove LEDs, hit ENTER." );
    WaitForUserKeyInput();
  }


  /****************************************************************************
   TestWriteDigitalOutputs()
  ****************************************************************************/
  static void TestReadDigitalInputs()
  {
    boolean skip_test = false;

    System.out.println( "Test: Read Digital Inputs, (hit ENTER for each test, 'q' and ENTER to quit)" );

    //Set all ports as outputs
    WriteRegister( DDRA, 0x00 );
    WriteRegister( DDRB, 0x00 );
    WriteRegister( DDRC, 0x00 );
    WriteRegister( DDRD, 0x00 );

    //Disable internal pullups
    WriteRegister( PORTA, 0x00 );
    WriteRegister( PORTB, 0x00 );
    WriteRegister( PORTC, 0x00 );
    WriteRegister( PORTD, 0x00 );

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.print( "Connect switches to ports, hit ENTER when ready." );
    WaitForUserKeyInput();

    for( int i = 0; i < 4; i++ )
    {
      int test_byte = 1;
      for( int b = 0; b < 9; b++ )
      {
        //Wait for user to hit ENTER
        System.out.print( "Ready for dio_test/ReadDigitalInputs/" + i + "/" + b + ":" );
        if( ((i == 3) && ((b == 0) || (b == 1))) ||                             //UART pins
            ((i == 1) && ((b == 5) || (b == 6) || (b == 7))) )                  //The STK200 has extra resistors on port B, pins 5 and 7, which will affect the signal; so lets just skip pins 5-7.
        {
          java.awt.Toolkit.getDefaultToolkit().beep();
          System.out.print( "****Port pin not available****" );
          skip_test = true;
        }
        WaitForUserKeyInput();

        if( skip_test == false )
        {
          int data = ReadRegister( port_in_addr[i] );
          if( data >= 0x100 )
          {
            java.awt.Toolkit.getDefaultToolkit().beep();
            System.out.println( "ERROR from avr_io" );
            return;
          }
          System.out.println( "data=" + data );

          if( data != (~test_byte & 0x00FF) )
          {
            java.awt.Toolkit.getDefaultToolkit().beep();
            System.out.println( "Test Fail" );
          }
        }
        skip_test = false;
        test_byte <<= 1;
      }
    }

    java.awt.Toolkit.getDefaultToolkit().beep();
    System.out.println( "Test done, remove switches, hit ENTER." );
    WaitForUserKeyInput();
  }


  /****************************************************************************
   SendCommand()
  ****************************************************************************/
  static void SendCommand()
  {
    try
    {
      outputStream.write( tx_cmd.BufferDirect(), 0, tx_cmd.available() );
    }
    catch( IOException e )
    {
      System.out.println( "Error sending command" );
    }
  }


  /****************************************************************************
   WaitForUserKeyInput()
  ****************************************************************************/
  static void WaitForUserKeyInput()
  {
    int inchar;

    try
    {
      inchar = System.in.read();
      while( System.in.available() > 0 )
      {
        System.in.read();
      }

      if( inchar == 0x71 ) //'q'
      {
        System.out.println( "Test Cancelled" );
        System.exit( 0 );
      }
    }
    catch( IOException e )
    {
    }
  }


  /****************************************************************************
   SetupSerialPort()
  ****************************************************************************/
  public static int SetupSerialPort( int comm_port_nbr )
  {
    //Check the status of the comm port.
    try
    {
      cpi = CommPortIdentifier.getPortIdentifier( "COM" + comm_port_nbr );

      //Check if anyone is using this comm port
      if( cpi.isCurrentlyOwned() == true )
      {
        return( ERROR_PORT_NOT_AVAILABLE1 );
      }
    }
    catch( NoSuchPortException e )
    {
      return( ERROR_PORT_NOT_AVAILABLE1 );
    }

    //Open the comm port
    try
    {
      comm_port = (SerialPort)cpi.open( "JWSDirect", 10 );
    }
    catch( PortInUseException e )
    {
      return( ERROR_PORT_NOT_AVAILABLE3 );
    }

    //Get a link to the receive buffer
    try
    {
      inputStream = comm_port.getInputStream();
      outputStream = comm_port.getOutputStream();
    }
    catch( IOException e )
    {
      comm_port.close();
      return( ERROR_NO_LINK );
    }

    //Set the communications parameters: 19200,8,N,1
    try
    {
      comm_port.setSerialPortParams( 19200, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE );
    }
    catch( UnsupportedCommOperationException e )
    {
      comm_port.close();
      return( ERROR_UNABLE_TO_SET_COMM_PARMS );
    }

    return( ERROR_NONE );
  }


  /****************************************************************************
   CloseSerialPort()
  ****************************************************************************/
  public static void CloseSerialPort()
  {
    comm_port.close();
  }
}
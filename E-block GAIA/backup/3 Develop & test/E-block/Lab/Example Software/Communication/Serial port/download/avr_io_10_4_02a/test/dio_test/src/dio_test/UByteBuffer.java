package dio_test;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2002</p>
 * <p>Company: </p>
 * @author unascribed
 * @version 1.0
 */

class UByteBuffer
{
  private int numbytes;
  private byte data[];
  private int get, put;
  private int size;

  UByteBuffer( int buffer_size )
  {
    size = buffer_size;
    data = new byte[buffer_size];
    numbytes = 0;
    get = 0;
    put = 0;
  }

  public void reset()
  {
    numbytes = 0;
    get = 0;
    put = 0;
  }

  public int available()
  {
    return( numbytes );
  }

  public int Get()
  {
    int temp = data[get];
    if( ++get >= size )
    {
      get = 0;
    }
    if( numbytes > 0 )
    {
      --numbytes;
    }
    return( temp );
  }

  public void Put( int data_byte )
  {
    data[put] = (byte)data_byte;
    if( ++put >= size )
    {
      put = 0;
    }
    if( numbytes < size )
    {
      ++numbytes;
    }
  }

  public int GetI( int idx )
  {
    return( (int)data[idx] & 0xFF );
  }

  public void PutI( int idx, int data_byte )
  {
    data[idx] = (byte)data_byte;
  }

  public void SetNumBytes( int new_numbytes )
  {
    numbytes = new_numbytes;
  }

  public byte[] BufferDirect()
  {
    return( data );
  }
}
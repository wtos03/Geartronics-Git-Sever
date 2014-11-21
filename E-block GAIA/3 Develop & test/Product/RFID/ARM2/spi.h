#ifndef __SPI_H__
#define __SPI_H__

void SPI_Init( void );
void SPI_Send( BYTE *Buf, DWORD Length );
void SPI_Receive( BYTE *Buf, DWORD Length );
BYTE SPI_ReceiveByte( void );

#endif /* __SPI_H__ */
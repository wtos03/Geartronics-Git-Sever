#ifndef __MMC_H__
#define __MMC_H__

/* The SPI data is 8 bit long, the MMC use 48 bits, 6 bytes */
#define MMC_CMD_SIZE 6

#define SPI_SEL 0x00100000 // use pin 0.20 as SPI enable

/* The max MMC flash size is 256MB */
#define MMC_DATA_SIZE 512 /* 16-bit in size, 512 bytes */
#define MAX_TIMEOUT 0xFF
#define IDLE_STATE_TIMEOUT 1
#define OP_COND_TIMEOUT 2
#define SET_BLOCKLEN_TIMEOUT 3
#define WRITE_BLOCK_TIMEOUT 4
#define WRITE_BLOCK_FAIL 5
#define READ_BLOCK_TIMEOUT 6
#define READ_BLOCK_DATA_TOKEN_MISSING 7
#define DATA_TOKEN_TIMEOUT 8
#define SELECT_CARD_TIMEOUT 9
#define SET_RELATIVE_ADDR_TIMEOUT 10

int mmc_init(void);
int mmc_response(BYTE response);
int mmc_read_block_int(WORD block_number,int MMCRDData[MMC_DATA_SIZE/4]);
int mmc_read_block_char(WORD block_number,char MMCRDData[MMC_DATA_SIZE]);
int mmc_write_block_int(WORD block_number,int MMCWRData[MMC_DATA_SIZE/4]);
int mmc_write_block_char(WORD block_number,char MMCWRData[MMC_DATA_SIZE]);
int mmc_wait_for_write_finish(void);

#endif /* __MMC_H__ */
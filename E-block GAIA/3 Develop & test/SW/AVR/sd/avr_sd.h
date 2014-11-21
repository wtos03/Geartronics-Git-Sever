/* 
 Include file for led_matrix.c
 
 Written by 			: 	Wichai Tossamartvorakul (wichai@geartronics.net)
 Date					:   21/3/2006
 
 Summary for include files
 -------------------------
 Constant variable				X
 Function prototype				-
 Variable (Flash memory)		-
 Memory usage					0 bytes
 Update History
 
 Date			By 					Comments
 ----			--					---------
 
 
 
*/



/* External Dependents    Need outside parameters 
CPU_CLOCK_SPEED

include  file 			chargen.h
*/


#include "cpu.h"

 
// SD commands  
#define SD_GO_IDLE_STATE			0		///< initialize card to SPI-type access
#define SD_SEND_OP_COND				1		///< set card operational mode
#define SD_SEND_CSD				9		///< get card's CSD
#define SD_SEND_CID				10		///< get card's CID
#define SD_SEND_STATUS				13
#define SD_SET_BLOCKLEN				16		///< Set number of bytes to transfer per block
#define SD_READ_SINGLE_BLOCK			17		///< read a block
#define SD_WRITE_BLOCK				24		///< write a block
#define SD_PROGRAM_CSD				27
#define SD_SET_WRITE_PROT			28
#define SD_CLR_WRITE_PROT			29
#define SD_SEND_WRITE_PROT			30
#define SD_TAG_SECTOR_START			32
#define SD_TAG_SECTOR_END			33
#define SD_UNTAG_SECTOR				34
#define SD_TAG_ERASE_GROUP_START 		35		///< Sets beginning of erase group (mass erase)
#define SD_TAG_ERARE_GROUP_END			36		///< Sets end of erase group (mass erase)
#define SD_UNTAG_ERASE_GROUP			37		///< Untag (unset) erase group (mass erase)
#define SD_ERASE				38		///< Perform block/mass erase
#define SD_CRC_ON_OFF				59		///< Turns CRC check on/off
// R1 Response bit-defines
#define SD_R1_BUSY				0x80	///< R1 response: bit indicates card is busy
#define SD_R1_PARAMETER				0x40
#define SD_R1_ADDRESS				0x20
#define SD_R1_ERASE_SEQ				0x10
#define SD_R1_COM_CRC				0x08
#define SD_R1_ILLEGAL_COM			0x04
#define SD_R1_ERASE_RESET			0x02
#define SD_R1_IDLE_STATE			0x01
// Data Start tokens
#define SD_STARTBLOCK_READ			0xFE	///< when received from card, indicates that a block of data will follow
#define SD_STARTBLOCK_WRITE			0xFE	///< when sent to card, indicates that a block of data will follow
#define SD_STARTBLOCK_MWRITE			0xFC
// Data Stop tokens
#define SD_STOPTRAN_WRITE			0xFD
// Data Error Token values
#define SD_DE_MASK				0x1F
#define SD_DE_ERROR				0x01
#define SD_DE_CC_ERROR				0x02
#define SD_DE_ECC_FAIL				0x04
#define SD_DE_OUT_OF_RANGE			0x04
#define SD_DE_CARD_LOCKED			0x04
// Data Response Token values
#define SD_DR_MASK				0x1F
#define SD_DR_ACCEPT				0x05
#define SD_DR_REJECT_CRC			0x0B
#define SD_DR_REJECT_WRITE_ERROR		0x0D

// functions

//! Initialize AVR<->SD hardware interface.
/// Prepares hardware for SD access.
void avr_sInit(void);

//! Initialize the card and prepare it for use.
/// Returns zero if successful.
u08 mmcReset(void);

//! Send card an SD command.
/// Returns R1 result code.
u08 mmcSendCommand(u08 cmd, u32 arg);

//! Read 512-byte sector from card to buffer
/// Returns zero if successful.
u08 mmcRead(u32 sector, u08* buffer);

//! Write 512-byte sector from buffer to card
/// Returns zero if successful.
u08 mmcWrite(u32 sector, u08* buffer);

//! Internal command function.
/// Issues a generic SD command as specified by cmd and arg.
u08 mmcCommand(u08 cmd, u32 arg);

// Function prototypes



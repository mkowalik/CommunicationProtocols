#include <LPC213X.H>
#include "periph.h"
#include "spi.h"
#include "delay.h"

#define SPI0_MCP4921_CS_PISEL0_bm	(0x00)<<20	// pin P0.10
#define	SPI0_MCP4921_CS_SETCLR_bm	1<<10		// pin P0.10

#define SPI0_MCP23S09_CS_PISEL0_bm	(0x00)<<22	// pin P0.11
#define	SPI0_MCP23S09_CS_SETCLR_bm	1<<11		// pin P0.11

#define	DAC_AB_bm										1<<7
#define	DAC_BUF_bm									1<<6
#define DAC_GA_bm										1<<5
#define	DAC_SHDN_bm									1<<4
#define	DAC_DATA_HIHG_bm						0x0F
#define	DAC_DATA_LOW_bm							0xFF
#define DAC_DATA_BYTES_NR						2
#define DAC_DATA_BYTES_OFFSET				0

#define EXP_DIR_REG_ADDRESS					0x00
#define EXP_DIR_SET_VALUE						0x00
#define EXP_DIR_GET_VALUE						0xFF

#define	EXP_GPIO_REG_ADDRESS				0x09

#define	EXP_DATA_bm									0xFF

#define	EXP_TX_WRITE_OPCODE					0x40
#define EXP_TX_WRITE_DATA_BYTES_nr	3
#define EXP_TX_WRITE_DATA_OFFSET		0

#define	EXP_TX_READ_OPCODE					0x41
#define EXP_TX_READ_DATA_BYTES_nr		2
#define EXP_TX_READ_DATA_OFFSET			0

#define EXP_RX_READ_DATA_BYTES_nr		1
#define EXP_RX_READ_DATA_OFFSET			2

void DAC_MCP4921_InitCSPin(){
	
	IODIR0 |= SPI0_MCP4921_CS_SETCLR_bm;
	IOSET0 |= SPI0_MCP4921_CS_SETCLR_bm;
	
	PINSEL0 |= SPI0_MCP4921_CS_PISEL0_bm;
	
}

void DAC_MCP4921_Set_Adv (unsigned int uiData){
	
	unsigned char ucValuesTx[DAC_DATA_BYTES_NR];
	struct SPI_TransactionParams sSendStruct;
	
	ucValuesTx[0]  = 0x00;
	ucValuesTx[0] |= DAC_GA_bm;
	ucValuesTx[0] |= DAC_SHDN_bm;
	ucValuesTx[0] |= (uiData >> 8) & DAC_DATA_HIHG_bm;
	ucValuesTx[1]  = (uiData & DAC_DATA_LOW_bm);
	
	sSendStruct.pucBytesForTx = ucValuesTx;
	sSendStruct.ucNrOfBytesForTx = DAC_DATA_BYTES_NR;
	sSendStruct.ucTxBytesOffset = DAC_DATA_BYTES_OFFSET;
	
	sSendStruct.pucBytesForRx = 0;
	sSendStruct.ucNrOfBytesForRx = 0;
	sSendStruct.ucRxBytesOffset = 0;
	
	IOCLR0 = SPI0_MCP4921_CS_SETCLR_bm;
	SPI_ExecuteTransaction(&sSendStruct);
	IOSET0 = SPI0_MCP4921_CS_SETCLR_bm;
	
}

void Port_MCP23S09_InitCSPin (){
	
	IODIR0 |= SPI0_MCP23S09_CS_SETCLR_bm;
	IOSET0 |= SPI0_MCP23S09_CS_SETCLR_bm;
	PINSEL0 |= SPI0_MCP23S09_CS_PISEL0_bm;
	
}

void Port_MCP23S09_Set (unsigned char ucData){
	
	unsigned char ucValuesTx[EXP_TX_WRITE_DATA_BYTES_nr];
	struct SPI_TransactionParams sSendStruct;
	
	sSendStruct.pucBytesForTx = ucValuesTx;
	sSendStruct.ucNrOfBytesForTx = EXP_TX_WRITE_DATA_BYTES_nr;
	sSendStruct.ucTxBytesOffset = EXP_TX_WRITE_DATA_OFFSET;
	
	sSendStruct.pucBytesForRx = 0;
	sSendStruct.ucNrOfBytesForRx = 0;
	sSendStruct.ucRxBytesOffset = 0;
	
	ucValuesTx[0] = EXP_TX_WRITE_OPCODE;
	ucValuesTx[1] = EXP_DIR_REG_ADDRESS;
	ucValuesTx[2] = EXP_DIR_SET_VALUE;
	
	IOCLR0 = SPI0_MCP23S09_CS_SETCLR_bm;
	SPI_ExecuteTransaction(&sSendStruct);
	IOSET0 = SPI0_MCP23S09_CS_SETCLR_bm;
	
	ucValuesTx[0] = EXP_TX_WRITE_OPCODE;
	ucValuesTx[1] = EXP_GPIO_REG_ADDRESS;
	ucValuesTx[2] = (ucData & EXP_DATA_bm);
	
	IOCLR0 = SPI0_MCP23S09_CS_SETCLR_bm;
	SPI_ExecuteTransaction(&sSendStruct);
	IOSET0 = SPI0_MCP23S09_CS_SETCLR_bm;
	
}

unsigned char Port_MCP23S09_Get (void){
	
	unsigned char ucValuesTx[EXP_TX_WRITE_DATA_BYTES_nr];
	unsigned char ucValuesRx[EXP_RX_READ_DATA_BYTES_nr];
	struct SPI_TransactionParams sSendStruct;
	
	sSendStruct.pucBytesForTx = ucValuesTx;
	sSendStruct.ucNrOfBytesForTx = EXP_TX_WRITE_DATA_BYTES_nr;
	sSendStruct.ucTxBytesOffset = EXP_TX_WRITE_DATA_OFFSET;
	
	sSendStruct.pucBytesForRx = 0;
	sSendStruct.ucNrOfBytesForRx = 0;
	sSendStruct.ucRxBytesOffset = 0;
	
	ucValuesTx[0] = EXP_TX_WRITE_OPCODE;
	ucValuesTx[1] = EXP_DIR_REG_ADDRESS;
	ucValuesTx[2] = EXP_DIR_GET_VALUE;
	
	IOCLR0 = SPI0_MCP23S09_CS_SETCLR_bm;
	SPI_ExecuteTransaction(&sSendStruct);
	IOSET0 = SPI0_MCP23S09_CS_SETCLR_bm;
	
	sSendStruct.pucBytesForTx = ucValuesTx;
	sSendStruct.ucNrOfBytesForTx = EXP_TX_READ_DATA_BYTES_nr;
	sSendStruct.ucTxBytesOffset = EXP_TX_READ_DATA_OFFSET;
	
	sSendStruct.pucBytesForRx = ucValuesRx;
	sSendStruct.ucNrOfBytesForRx = EXP_RX_READ_DATA_BYTES_nr;
	sSendStruct.ucRxBytesOffset = EXP_RX_READ_DATA_OFFSET;
	
	ucValuesTx[0] = EXP_TX_READ_OPCODE;
	ucValuesTx[1] = EXP_GPIO_REG_ADDRESS;
	
	
	IOCLR0 = SPI0_MCP23S09_CS_SETCLR_bm;
	SPI_ExecuteTransaction(&sSendStruct);
	IOSET0 = SPI0_MCP23S09_CS_SETCLR_bm;
	return sSendStruct.pucBytesForRx[0];
	
}

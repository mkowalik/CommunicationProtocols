#include <LPC213X.H>
#include "spi.h"


#define	SPI_CPHA_MASK_bp	3	// Clock phase control
#define SPI_CPOL_MASK_bp	4	// Clock polarity control
#define SPI_MSTR_MASK_bp	5	// Master mode select
#define	SPI_LSBF_MASK_bp	6	// LSB First controls which direction each byte is shifted when transferred
#define	SPI_SPIE_MASK_bp	7	// Serial peripheral interrupt enable.

#define SPI_SPIF_MASK	1<<7	// SPI transfer complete flag.

#define SPI0_SCK_PINSEL0_MASK	(0x01)<<8		// pin P0.4
#define SPI0_MISO_PINSEL0_MASK	(0x01)<<10	// pin P0.5
#define SPI0_MOSI_PINSEL0_MASK	(0x01)<<12	// pin P0.6	

void SPI_ConfigMaster(struct SPI_FrameParams sFrameParams){
	
	PINSEL0 |= SPI0_SCK_PINSEL0_MASK;
	PINSEL0 |= SPI0_MISO_PINSEL0_MASK;
	PINSEL0 |= SPI0_MOSI_PINSEL0_MASK;
		
	S0SPCR |= (sFrameParams.ucCpha  << SPI_CPHA_MASK_bp) |
						(sFrameParams.ucCpol  << SPI_CPOL_MASK_bp) | 
						(sFrameParams.ucClsbf << SPI_LSBF_MASK_bp) | 
						(1 << SPI_MSTR_MASK_bp);
	
	S0SPCCR = sFrameParams.ClkDivider;
	
}

unsigned int max(unsigned int a, unsigned int b){
	if (a>b)
		return a;
	return b;
}

void SPI_ExecuteTransaction(struct SPI_TransactionParams* sTransactionParams){
	
	unsigned int uiTransferByteCounter;
	unsigned char txByte;
	unsigned char rxByte;
	
	for (uiTransferByteCounter = 0; uiTransferByteCounter < max(sTransactionParams->ucNrOfBytesForTx + sTransactionParams->ucTxBytesOffset,
																															sTransactionParams->ucNrOfBytesForRx + sTransactionParams->ucRxBytesOffset); uiTransferByteCounter++){

		if ((uiTransferByteCounter < sTransactionParams->ucTxBytesOffset) || (uiTransferByteCounter >= sTransactionParams->ucNrOfBytesForTx + sTransactionParams->ucTxBytesOffset )){	// transmit offset zone | after last byte zone
			txByte = 0x00;// whatever
		} else { // transmit data zone
			txByte = sTransactionParams->pucBytesForTx[uiTransferByteCounter - sTransactionParams->ucTxBytesOffset];
		}
	
		S0SPDR = txByte;
		while ((S0SPSR & SPI_SPIF_MASK) == 0){ }
		rxByte = S0SPDR;
		
		if ((uiTransferByteCounter >= sTransactionParams->ucRxBytesOffset) && (uiTransferByteCounter < sTransactionParams->ucRxBytesOffset + sTransactionParams->ucNrOfBytesForRx)){ // receive data zone
			sTransactionParams->pucBytesForRx[uiTransferByteCounter - sTransactionParams->ucRxBytesOffset] = rxByte;
		}
			
	}
	
}



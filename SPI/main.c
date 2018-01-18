#include <LPC21xx.H>
#include <math.h>

#define	SPI_CPHA_MASK	1<<3	// Clock phase control
#define SPI_CPOL_MASK	1<<4	// Clock polarity control
#define SPI_MSTR_MASK	1<<5	// Master mode select
#define	SPI_LSBF_MASK	1<<6	// LSB First controls which direction each byte is shifted when transferred
#define	SPI_SPIE_MASK	1<<7	// Serial peripheral interrupt enable.

#define SPI_SPIF_MASK	1<<7	// SPI transfer complete flag.

#define SPI0_SCK_PINSEL0_MASK	(0x01)<<8		// pin P0.4
#define SPI0_MISO_PINSEL0_MASK	(0x01)<<10	// pin P0.5
#define SPI0_MOSI_PINSEL0_MASK	(0x01)<<12	// pin P0.6	
#define SPI0_CHIP_SELECT_PISEL0_MASK	(0x00)<<20	// pin P0.10

#define	SPI0_CHIP_SELECT_SETCLR_MASK	1<<10		// pin P0.10

#define SPI0_CLOCK_DIVIDER	64

#define	DAC_AB_MASK		1<<15
#define	DAC_BUF_MASK	1<<14
#define DAC_GA_MASK		1<<13
#define	DAC_SHDN_MASK	1<<12
#define	DAC_DATA_MASK	0x0FFF

#define MILLS_ROUNDS 5520

#define DAC_MAX_MILIVOLTAGE	3300
#define DAC_MAX_DATA_VALUE	4096

#define M_PI 3.1415926
 
void delay(unsigned int uiMilisecs){
 
    unsigned int uiCounter;
 
    for (uiCounter=0; uiCounter<MILLS_ROUNDS * uiMilisecs; uiCounter++) {}
}

void SPI_Init(){
	
	IODIR0 |= SPI0_CHIP_SELECT_SETCLR_MASK;
	IOSET0 |= SPI0_CHIP_SELECT_SETCLR_MASK;
	
	PINSEL0 |= SPI0_SCK_PINSEL0_MASK;
	PINSEL0 |= SPI0_MISO_PINSEL0_MASK;
	PINSEL0 |= SPI0_MOSI_PINSEL0_MASK;
	PINSEL0 |= SPI0_CHIP_SELECT_PISEL0_MASK;
	
	S0SPCR |= SPI_MSTR_MASK;
	
	S0SPCCR = SPI0_CLOCK_DIVIDER;
}

void DAC_MCP4921_Set_mV(unsigned int uiVoltage){
	unsigned int uiSendValue = 0;
	unsigned int uiDataValue;
	
	uiDataValue = (uiVoltage * DAC_MAX_DATA_VALUE) / DAC_MAX_MILIVOLTAGE;
	
	SPI_Init();
	
	uiSendValue |= DAC_GA_MASK;
	uiSendValue |= DAC_SHDN_MASK;
	uiSendValue |= (uiDataValue & DAC_DATA_MASK);
	
	IOCLR0 = SPI0_CHIP_SELECT_SETCLR_MASK;
	
	S0SPDR = (uiSendValue >> 8) & 0xFF;
	
	while ((S0SPSR & SPI_SPIF_MASK) == 0){ }
	
	S0SPDR = uiSendValue & 0xFF;
	
	while ((S0SPSR & SPI_SPIF_MASK) == 0){ }
	IOSET0 = SPI0_CHIP_SELECT_SETCLR_MASK;
	
}

unsigned int uiDACValues[360];

int main(){
	 
	unsigned int i;

	for (i=0; i<360; i++){
		uiDACValues[i] = ((int)(sin(((i*1.0)/180.0) * M_PI) * (DAC_MAX_MILIVOLTAGE/2))) + (DAC_MAX_MILIVOLTAGE/2);
	}
	
	while (1){
		for(i=0; i<360; i++){
			// dopasowac dzialanie do nazwy 0 0,5 1V
			// dorobic sinusa rozdzielczosc 1 stopien
			// czasy t glownego zegara, zegar peryferow, zegar SPI, okresy!
			// bajt, jeden sampl, okres sinusa
			// na podstawie teoretczynych oblcizen
			
			DAC_MCP4921_Set_mV(uiDACValues[i]);
		}
	}
	
}

#include <LPC21xx.H>
#include <math.h>
#include "spi.h"
#include "periph.h"
#include "delay.h"

#include "command_decoder.h"
#include "uart.h"
#include "string_buffer.h"

#define M_PI 3.1415926

#define SPI0_CLOCK_DIVIDER  128
#define	SPI0_LSBF_value		0
#define SPI0_CPHA_value		0
#define SPI0_CPOL_value		0

#define DAC_MAX_DATA_VALUE	4096

#define UART_BAUD_RATE	9600

extern Token asToken[];

unsigned char ucUnknownCommandStatus = 0;
unsigned char ucPortGet = 0;

int main(){
    
  char pcStringBuffer[40];
	struct SPI_FrameParams sFrameParams;
	
	sFrameParams.ucClsbf = SPI0_LSBF_value;
	sFrameParams.ucCpha = SPI0_CPHA_value;
	sFrameParams.ucCpol = SPI0_CPOL_value;
	sFrameParams.ClkDivider = SPI0_CLOCK_DIVIDER;
	SPI_ConfigMaster(sFrameParams);
	
	DAC_MCP4921_InitCSPin();
	Port_MCP23S09_InitCSPin();
	
	UART_InitWithInt(UART_BAUD_RATE);
	
	Transmiter_SendString("init");
	
	while (1){
		
		if (Transmiter_GetStatus()==FREE){
				if (ucPortGet==1){
						UIntToHexStr (Port_MCP23S09_Get(), pcStringBuffer);
						ucPortGet = 0;
						Transmiter_SendString(pcStringBuffer);
				} else if (ucUnknownCommandStatus==1){
						CopyString("unknowncommand", pcStringBuffer);
						ucUnknownCommandStatus = 0;
						Transmiter_SendString(pcStringBuffer);
				}
		}
	
		if(eReciever_GetStatus()==READY){
			Reciever_GetStringCopy(pcStringBuffer);
			DecodeMsg(pcStringBuffer);
		
			if (ucGetTokenNumber() > 0){
					if ((asToken[0]).eType == KEYWORD){
							switch((asToken[0]).uValue.eKeyword){
									case PORTSET:
											if ((ucGetTokenNumber() == 2) && (asToken[1].eType == NUMBER)){
												Port_MCP23S09_Set(asToken[1].uValue.uiNumber & 0xFF);
											} else {
												ucUnknownCommandStatus = 1;
											}
											break;
									case PORTGET:
											if (ucGetTokenNumber() == 1){
												ucPortGet = 1;
											} else {
												ucUnknownCommandStatus = 1;
											}
											break;
									case DACSET:
											if ((ucGetTokenNumber() == 2) && (asToken[1].eType == NUMBER)){
												DAC_MCP4921_Set_Adv(asToken[1].uValue.uiNumber);
											} else {
												ucUnknownCommandStatus = 1;
											}
											break;
									default:
											ucUnknownCommandStatus = 1;
							}
					} else {
							ucUnknownCommandStatus = 1;
					}
			}
		}
	}
}

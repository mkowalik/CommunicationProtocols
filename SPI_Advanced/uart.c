#include <LPC210X.H>
#include "uart.h"
#include "string.h"
#include "string_buffer.h"

#define	NULL	0

/************ UART ************/
// U0LCR Line Control Register
#define mDIVISOR_LATCH_ACCES_BIT                   0x00000080
#define m8BIT_UART_WORD_LENGTH                     0x00000003

// UxIER Interrupt Enable Register
#define mRX_DATA_AVALIABLE_INTERRUPT_ENABLE        0x00000001
#define mTHRE_INTERRUPT_ENABLE                     0x00000002

// UxIIR Pending Interrupt Identification Register
#define mINTERRUPT_PENDING_IDETIFICATION_BITFIELD  0x0000000F
#define mTHRE_INTERRUPT_PENDING                    0x00000002
#define mRX_DATA_AVALIABLE_INTERRUPT_PENDING       0x00000004

/************ Interrupts **********/
// VIC (Vector Interrupt Controller) channels
#define VIC_UART0_CHANNEL_NR  6
#define VIC_UART1_CHANNEL_NR  7

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE                           0x00000020

////////////// Zmienne globalne ////////////
char cOdebranyZnak;


///////////////////////////////////////////

#define TRANSMITER_SIZE 64

typedef struct {
    char cData[TRANSMITER_SIZE];
    enum eTransmiterStatus eStatus;
    unsigned char fLastCharacter;
    unsigned char ucCharCtr;
} TransmiterBuffer;

TransmiterBuffer sTransmiterBuffer;

#define TERMINATOR '\r'
#define TERMINATOR2 '\n'

char Transmiter_GetCharacterFromBuffer(void){
    if (sTransmiterBuffer.cData[sTransmiterBuffer.ucCharCtr]==NULL){
        if (sTransmiterBuffer.fLastCharacter==0){
			sTransmiterBuffer.fLastCharacter = 1;
            return TERMINATOR;
        } else if (sTransmiterBuffer.fLastCharacter==1){
			sTransmiterBuffer.fLastCharacter = 2;
            return TERMINATOR2;
		} else {
            sTransmiterBuffer.eStatus = FREE;
            return NULL;
        }
    } else {
        sTransmiterBuffer.ucCharCtr++;
        return sTransmiterBuffer.cData[sTransmiterBuffer.ucCharCtr-1];
    }
}

void setCharInUARTTransmitRegister(){
    
    char cCharToSend = Transmiter_GetCharacterFromBuffer();
    if (cCharToSend!=NULL){
        U0THR = cCharToSend;
    }
    
}

void Transmiter_SendString(char cString[]){
    CopyString(cString, sTransmiterBuffer.cData);
    sTransmiterBuffer.eStatus = BUSY;
    sTransmiterBuffer.fLastCharacter = 0;
    sTransmiterBuffer.ucCharCtr = 0;
    setCharInUARTTransmitRegister();
}

enum eTransmiterStatus Transmiter_GetStatus(void){
    return sTransmiterBuffer.eStatus;
}



///////////////////////////////////////////
__irq void UART0_Interrupt (void) {
    // jesli przerwanie z odbiornika (Rx)
   
    unsigned int uiCopyOfU0IIR=U0IIR; // odczyt U0IIR powoduje jego kasowanie wiec lepiej pracowac na kopii

    if      ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mRX_DATA_AVALIABLE_INTERRUPT_PENDING) // odebrano znak
    {
        Reciever_PutCharacterToBuffer(U0RBR);
    } 
   
    if ((uiCopyOfU0IIR & mINTERRUPT_PENDING_IDETIFICATION_BITFIELD) == mTHRE_INTERRUPT_PENDING)              // wyslano znak - nadajnik pusty 
    {
        setCharInUARTTransmitRegister();
    }

    VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////

#define RXD0_bm (1<<2)
#define RXD1_bm (1<<3)
#define TXD0_bm (1<<0)
#define TXD1_bm (1<<1)

void UART_InitWithInt(unsigned int uiBaudRate){

    // UART0
    PINSEL0 = PINSEL0 | RXD0_bm;     
    PINSEL0 = PINSEL0 & (~RXD1_bm);                               // ustawic pina na odbiornik uart0

    PINSEL0 = PINSEL0 | TXD0_bm;     
    PINSEL0 = PINSEL0 & (~TXD1_bm);

    U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // długosc słowa, DLAB = 1
    U0DLL   = (((15000000)/16)/uiBaudRate);                      // predkosc transmisji
    U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
    U0IER  |= (mRX_DATA_AVALIABLE_INTERRUPT_ENABLE | mTHRE_INTERRUPT_ENABLE);               // ??? co tu robinmy - wlaczamy wywolanie przerwania gdy zostanie odebrany bajt przez UART

    // INT
    VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
    VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
    VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
    
    
    sTransmiterBuffer.eStatus = FREE;
}

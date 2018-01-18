#include "string_buffer.h"
#include "string.h"

#define RECEIVER_SIZE 64
#define TERMINATOR '\r'

typedef struct {
    char cData[RECEIVER_SIZE];
    unsigned char ucCharCtr;
    enum eRecieverStatus eStatus;
} ReceiverBuffer;

ReceiverBuffer receiverBuffer;

void Reciever_PutCharacterToBuffer(char cCharacter){
    if (receiverBuffer.ucCharCtr >= RECEIVER_SIZE){
        receiverBuffer.eStatus = OVERFLOW;
        return;
    }
    receiverBuffer.cData[receiverBuffer.ucCharCtr] = cCharacter;
    receiverBuffer.ucCharCtr++;
    if (cCharacter==TERMINATOR){
        receiverBuffer.eStatus = READY;
        receiverBuffer.ucCharCtr = 0;
    }
}

enum eRecieverStatus eReciever_GetStatus(void){
	return receiverBuffer.eStatus;
}

void Reciever_GetStringCopy(char * ucDestination){
    ReplaceCharactersInString(receiverBuffer.cData, TERMINATOR, NULL);
    CopyString (receiverBuffer.cData, ucDestination);
    receiverBuffer.eStatus = EMPTY;
}

void StringBuffer_init(){
    receiverBuffer.eStatus = EMPTY;
    receiverBuffer.ucCharCtr = 0;
}

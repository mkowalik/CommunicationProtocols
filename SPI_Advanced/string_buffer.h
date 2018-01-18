#ifndef __string_buffer_h__
#define __string_buffer_h__

#define	NULL	0

enum eRecieverStatus { EMPTY, READY, OVERFLOW};

void Reciever_PutCharacterToBuffer(char cCharacter);
enum eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
void StringBuffer_init(void);

#endif

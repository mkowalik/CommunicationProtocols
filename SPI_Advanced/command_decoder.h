#ifndef __token_operations_h__
#define __token_operations_h__

#include "string.h"

void DecodeMsg(char* pcString);

enum TokenType { KEYWORD, NUMBER, STRING};

enum KeywordCode {PORTSET, PORTGET, DACSET};

union TokenValue {
	enum KeywordCode eKeyword; // jezeli KEYWORD
	unsigned int uiNumber; // jezeli NUMBER
	char * pcString; // jezeli STRING
};

typedef struct Token {
	enum TokenType eType; // KEYWORD, NUMBER, STRING
	union TokenValue uValue; // enum, unsigned int, char*
} Token;

struct Token * getAsToken(void);

unsigned char ucGetTokenNumber(void);

#endif

#include "command_decoder.h"

#define FOUR_LAST_BITS_DIGIT_MASK 	0x000F

#define NULL 						0

#define MAX_TOKEN_NR 				2

struct Token asToken[MAX_TOKEN_NR]; // wypelniana przez DecodeMsg na podstawie cUartRxBuffer i asCommandList

#define MAX_KEYWORD_STRING_LTH 		10 // mksymalna dlugosc komendy

struct Keyword {
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
};	

#define MAX_KEYWORD_NR 				3

struct Keyword asKeywordList[MAX_KEYWORD_NR]= { // uzywana przez bStringToCommand
	{PORTSET,"portset"},
	{PORTGET, "portget"},
	{DACSET, "dacset"}
};

unsigned char ucTokenNr; // liczba tokenów w zdekodowanym komunikacie

enum TokenFinderState {	TOKEN, DELIMETER };

//enum Result {OK, ERROR};

unsigned char ucFindTokenInString(char* pcString){
	
	unsigned int uiStringIndex;
	enum TokenFinderState eActualState = DELIMETER;
	unsigned char ucTokenNr = 0;

	for (uiStringIndex = 0; ; uiStringIndex++){
		switch (eActualState){
			case TOKEN:
				if (pcString[uiStringIndex]==NULL){
					return ucTokenNr;
				} else if (pcString[uiStringIndex]==' '){
					eActualState = DELIMETER;
				} else {
					eActualState = TOKEN;
				}
				break;
			case DELIMETER:
				if (pcString[uiStringIndex]==NULL){
					return ucTokenNr;
				} else if (pcString[uiStringIndex]==' '){
					eActualState = DELIMETER;
				} else {
					if (ucTokenNr < MAX_TOKEN_NR){
						asToken[ucTokenNr].uValue.pcString = (pcString + uiStringIndex);
					}
					eActualState = TOKEN;
					ucTokenNr++;
				}
				break;
		}
	}
}

enum Result eStringToKeyword(char pcString[], enum KeywordCode* peKeywordCode){
	
	unsigned int uiStringIndex;
	unsigned char ucKeywordStringIndex;
	
	for (ucKeywordStringIndex = 0; ucKeywordStringIndex < MAX_KEYWORD_NR; ucKeywordStringIndex++){
		for (uiStringIndex = 0; pcString[uiStringIndex]!=NULL; uiStringIndex++){
			if (pcString[uiStringIndex]!=asKeywordList[ucKeywordStringIndex].cString[uiStringIndex]){
				break;
			}
		}
		
		if (pcString[uiStringIndex]==NULL){
			*peKeywordCode = asKeywordList[ucKeywordStringIndex].eCode;
			return OK;
		}
	}
	
	return ERROR;
	
}

void DecodeTokens(void){
	
	unsigned int uiActualTokenIndex;
	
	for (uiActualTokenIndex=0; uiActualTokenIndex<ucTokenNr; uiActualTokenIndex++){
		
		char* pcActualTokenStringPointer = asToken[uiActualTokenIndex].uValue.pcString;
		enum KeywordCode eActualKeywordCode;
		unsigned int uiActualIntValue;
		
		if (eStringToKeyword(pcActualTokenStringPointer, &eActualKeywordCode) == OK){
			asToken[uiActualTokenIndex].eType = KEYWORD;
			asToken[uiActualTokenIndex].uValue.eKeyword = eActualKeywordCode;
		} else if (eHexStringToUInt(pcActualTokenStringPointer, &uiActualIntValue) == OK){
			asToken[uiActualTokenIndex].eType = NUMBER;
			asToken[uiActualTokenIndex].uValue.uiNumber = uiActualIntValue;
		} else {
			asToken[uiActualTokenIndex].eType = STRING;
		}
		
	}
}

void DecodeMsg(char* pcString){

	ucTokenNr = ucFindTokenInString(pcString);
	ReplaceCharactersInString(pcString, ' ', NULL);
	DecodeTokens();
	
}


struct Token * getAsToken(void){
    return asToken;
}

unsigned char ucGetTokenNumber(){
    return ucTokenNr;
}

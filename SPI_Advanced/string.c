#include "string.h"

#define FOUR_LAST_BITS_DIGIT_MASK 0x000F

void CopyString (char pcSource[], char pcDestination[]){
	
	unsigned char ucStringIterator;
	
	for (ucStringIterator = 0; pcSource[ucStringIterator] != NULL; ucStringIterator++){
		pcDestination[ucStringIterator] = pcSource[ucStringIterator];
	}
	
	pcDestination[ucStringIterator] = NULL;
	
}

enum CompResult eCompareString (char pcStr1[], char pcStr2[]){
	
	unsigned char ucStringIterator;
	
	for (ucStringIterator = 0; pcStr1[ucStringIterator]!=NULL && pcStr2[ucStringIterator]!=NULL; ucStringIterator++){ }

	if (pcStr1[ucStringIterator]!=pcStr2[ucStringIterator]){
		return DIFFERENT;
	}
	return EQUAL;
	
}

void AppendString (char pcSourceStr[], char pcDestinationStr[]){
		
	unsigned char ucDestStringIterator;
	
	for (ucDestStringIterator=0; pcDestinationStr[ucDestStringIterator]!=NULL; ucDestStringIterator++){ }

	CopyString(pcSourceStr, pcDestinationStr + ucDestStringIterator);
	
}

void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar){
	
	unsigned char ucStringIterator;
	
	for (ucStringIterator=0; pcString[ucStringIterator]!=NULL; ucStringIterator++){
		if (pcString[ucStringIterator]==cOldChar){
			pcString[ucStringIterator] = cNewChar;
		}
	}
	
}

enum Result eHexStringToUInt (char pcStr[], unsigned int* puiValue){
	
	int iStringIterator;
	char cActualChar;
	
	*puiValue = 0;
	
	if (pcStr[0] != '0' || pcStr[1] != 'x' || pcStr[2]==NULL){
		return ERROR;
	}
	
	for (iStringIterator = 0; iStringIterator < 4 && pcStr[iStringIterator+2] != NULL; iStringIterator++){
		cActualChar = pcStr[iStringIterator+2];
		if ((cActualChar < '0' || cActualChar > '9') && (cActualChar < 'A' || cActualChar > 'F')){
			return ERROR;
		}
		if (cActualChar >= '0' && cActualChar <= '9'){
			*puiValue = ((*puiValue)<<4) | ((cActualChar-'0') & FOUR_LAST_BITS_DIGIT_MASK);
		} else {
			*puiValue = ((*puiValue)<<4) | ((cActualChar-'A'+10) & FOUR_LAST_BITS_DIGIT_MASK);
		}
	}
	
	if (pcStr[iStringIterator+2]!=0){
		return ERROR;
	}
	
	return OK;
}


void UIntToHexStr (unsigned int uiValue, char* pcStr){
	
	int iValue4BitsIterator;
	
	pcStr[0] = '0';
	pcStr[1] = 'x';
	
	for (iValue4BitsIterator = 3; iValue4BitsIterator >= 0; iValue4BitsIterator--){
		unsigned char ucActualDigit = uiValue >> (iValue4BitsIterator*4);
		ucActualDigit = ucActualDigit & FOUR_LAST_BITS_DIGIT_MASK;
		if (ucActualDigit < 10){
			pcStr[2+(3-iValue4BitsIterator)] = '0' + ucActualDigit;
		} else {
			pcStr[2+(3-iValue4BitsIterator)] = 'A' + ucActualDigit - 10;
		}
	}
	
	pcStr[2 + 4] = NULL;
	
}

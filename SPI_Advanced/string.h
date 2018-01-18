#ifndef __string_operations_h__
#define __string_operations_h__

#define NULL 0

void CopyString (char pcSource[], char pcDestination[]);
enum CompResult {DIFFERENT, EQUAL};
enum CompResult eCompareString (char pcStr1[], char pcStr2[]);
void AppendString (char pcSourceStr[], char pcDestinationStr[]);
void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar);

enum Result {OK, ERROR};

enum Result eHexStringToUInt (char pcStr[], unsigned int* puiValue);
void UIntToHexStr (unsigned int uiValue, char* pcStr);

#endif

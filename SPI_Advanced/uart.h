
void UART_InitWithInt(unsigned int uiBaudRate);

enum eTransmiterStatus {FREE, BUSY};

enum eTransmiterStatus Transmiter_GetStatus(void);
void Transmiter_SendString(char cString[]);

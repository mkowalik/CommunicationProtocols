
void Can1_InitAsTX(void);
void Can2_InitAsRX(void);
unsigned char ucCan1_TxFree(void);
void Can1_SendByte(unsigned int value);
unsigned char ucCan2_RxReady(void);
unsigned int ucCan2_ReceiveByte(void);

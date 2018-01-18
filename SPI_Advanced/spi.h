
struct SPI_FrameParams{
	unsigned char ucCpha;
	unsigned char ucCpol;
	unsigned char ucClsbf;
	unsigned char ClkDivider;
};

struct SPI_TransactionParams{
	unsigned char *pucBytesForTx; // wskaznik na tablice z bajtami do wyslania
	unsigned char ucNrOfBytesForTx; // ilosc bajt�w do wyslania
	unsigned char ucTxBytesOffset; // offset bajt�w do wyslania
	unsigned char *pucBytesForRx; // wskaznik na tablice na odebrane bajty
	unsigned char ucNrOfBytesForRx; // ilosc bajt�w do odebrania
	unsigned char ucRxBytesOffset; // offset bajt�w do odebrania
};

void SPI_ConfigMaster(struct SPI_FrameParams sFrameParams);
void SPI_ExecuteTransaction(struct SPI_TransactionParams* sTransactionParams);

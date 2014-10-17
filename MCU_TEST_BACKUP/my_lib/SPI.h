enum SPI_STATUS{_SPI_BUSY,_SPI_OK,_SPI_ERROR,_PARAMETER_ERROR};

struct SPI_DATA{
	volatile unsigned char *Status;
	unsigned char *TxDATA;
	unsigned char TxSIZE;
	unsigned char *RxBUFFER;
	unsigned char RxSIZE;
};

void SPI_INIT(void);
enum SPI_STATUS SPI_T(struct SPI_DATA data);
enum SPI_STATUS SPI_R(struct SPI_DATA data);
enum SPI_STATUS SPI_TR(struct SPI_DATA data);
//enum SPI_STATUS SPI_TR();



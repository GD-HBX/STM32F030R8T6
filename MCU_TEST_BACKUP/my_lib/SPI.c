#include "register.h"
#include "SPI.h"

#define USE_GPIO_PORT 1
#define FRAME_SIZE 7

enum _SPI_TM{_NONE=0,_T=1,_R=2,_TR=3,_RT=4};
enum PHASE{_ONE=0,_TWO};

static uc SPI1_INIT = 0;
static const ui TX_temp = 0xFF;//��ֵ�������ڽ���ʱ���͵��������ݶ�����ʱ���ź�
static ui RX_temp;//�����ڷ���ʱ��������DMA��buffer,ע��������͵Ĵ�СҪ�ͷ���FIFO�Ĵ�Сһ��
static struct SPI_DATA SPI_data;//�������淢�͹����е�����
static struct{
	uc Using;					 //�Ƿ�ʹ����
	enum _SPI_TM TM;	 //����ģʽ
	enum PHASE Communication_phase;//����׶Σ�0��1
} SPI_SF;						 //SPI status flag

/***************���ײ㺯����**********************/
//���ݽṹ����
static void _spi_data_clear(){
	SPI_data.Status 	= 0;
	SPI_data.RxBUFFER = 0;
	SPI_data.RxSIZE 	= 0;
	SPI_data.TxDATA 	= 0;
	SPI_data.TxSIZE 	= 0;
}
//״̬����
__inline static void _status_clear_and_update_ex_status(){
	if(SPI_data.Status != 0){*SPI_data.Status = 1;}
	_spi_data_clear();
	SPI_SF.Using = 0;
	SPI_SF.TM		 = _NONE;
	SPI_SF.Communication_phase = _ONE;
}
//״̬����
__inline static void _status_update(enum _SPI_TM tm,struct SPI_DATA data){
	SPI_SF.Using = 1;//!
	SPI_SF.TM = tm;//!
	SPI_data = data;//��Ҫ��������ݸ��Ƶ�Ū������//!
	*SPI_data.Status = 0;//����ⲿ״̬
}
//�������ݸ���
__inline void _spi_data_copy(struct SPI_DATA t_data){
	SPI_data = t_data;
}
//����RCC
__inline void _reset_and_enb_clock(void){
	//SPI��ʱ��
	RCC->RCC_APB2RSTR |= 1<<12;
	RCC->RCC_APB2RSTR &= ~(1<<12);
	RCC->RCC_APB2ENR	|= 1<<12;
	//GPIO��ʱ��
	RCC->RCC_AHBENR 	|= 1<<17;
	#if USE_GPIO_PORT > 1
	RCC->RCC_AHBENR		|= 1<<18;
	#endif
	//DMA��ʱ��
	RCC->RCC_AHBENR |= 1;
}
/*����spi�����GPIO��
������AF0
 1   2
PA4,PA15 = NSS
PA5,PB3	 = SCK
PA6,PB4	 = MISO
PA7,PB5	 = MOSI
*/
__inline void _spi_gpio_setting(void){
	#if USE_GPIO_PORT == 1
		//PA4,5,6,7
		GPIOA->GPIOx_MODER 	|= 0xAA << 8;//���Ϲ���
		GPIOA->GPIOx_OTYPER &= ~(ui)(0xF<<4);//�������
		//GPIOA->GPIOx_OSPEEDR |= 0xFF << 8;//����GPIO����ٶ�
		GPIOA->GPIOx_PUPDR 	|= 0x55 << 8;//���ó�����
		GPIOA->GPIOx_AFRL 	&= 0x00FF;//���ø��Ϲ���
	#elif USE_GPIO_PORT == 2
		//PA15,PB3,4,5
		GPIOA->GPIOx_MODER 	|= 0x80000000;
	  GPIOA->GPIOx_OTYPER |= ~(ui)(1<<15);
	  //GPIOA->GPIOx_OSPEEDR |= 0xc0000000;
	  GPIOA->GPIOx_PUPDR 	|= 0x40000000;
	  GPIOA->GPIOx_AFRH 	&= ~(0xF0000000);
		GPIOB->GPIOx_MODER 	|= 0x2A<<6;
	  GPIOB->GPIOx_OTYPER |= ~(ui)(0x7<<3);
	  //GPIOB->GPIOx_OSPEEDR |= 0x3F<<6;
	  GPIOB->GPIOx_PUPDR 	|= 0x15<<6;
	  GPIOB->GPIOx_AFRL 	&= 0xFF000FFF;
	#elif USE_GPIO_PORT == 3
		//������������������������GPIO�ڻ��ã������ҾͲ�д�ˣ��õ�ʱ����д
	#endif
}
#ifdef FRAME_SIZE
#if ((FRAME_SIZE < 2) || (FRAME_SIZE > 0xF))
#error "The FRAME_SIZE is not between 0x3(4-bit) and 0xF(16-bit)!"
#endif
#else 
#error "Please define a "FRAME_SIZE" macro!"
#endif

/*����spi�������*/
__inline void _spi_common_setting(){
	//2.
	SPI1->SPIx_CR1 &= 0x7<<3;//a.����������
	
	SPI1->SPIx_CR1 &= ~(ui)2;//b.CPOL
	SPI1->SPIx_CR1 &= ~(ui)1;//b.CPHA
													
	SPI1->SPIx_CR1 &= ~(ui)(1<<10);//c.RXONLY������Ϊȫ˫��
	SPI1->SPIx_CR1 &= ~(ui)(1<<15);//c.BIDIMODE
	SPI1->SPIx_CR1 &= ~(ui)(1<<14);//c.BIDIOE
	
	SPI1->SPIx_CR1 &= ~(ui)(1<<7);//d.LSBFIRST������Ϊ����MSB��
	
	SPI1->SPIx_CR1 &= ~(ui)(1<<11);//e.CRCL
	SPI1->SPIx_CR1 &= ~(ui)(1<<13);//e.CRCEN
	
	SPI1->SPIx_CR1 &= ~(ui)(1<<9);//f.SSM
	SPI1->SPIx_CR1 |= 1<<8;			//f.SSI
	
	SPI1->SPIx_CR1 |= 1<<2;			//g.MSTR
	
	//3.
	SPI1->SPIx_CR2 |= 0xF<<8;//a.DS//��0
	SPI1->SPIx_CR2 &= ~((ui)(0xF-FRAME_SIZE)<<8);//4bit 		//a.DS������DS
	
	SPI1->SPIx_CR2 |= 1<<2;//b.SSOE
	
	SPI1->SPIx_CR2 &= ~(ui)(1<<4);//c.FRF
	
	SPI1->SPIx_CR2 &= ~(ui)(1<<3);//d.NSSP
	
	SPI1->SPIx_CR2 |= 1<<12;//e.FRXTH
	
	// SPI1->SPIx_CR2 &= ~(ui)(1<<1);//f.LDMA_TX
	// SPI1->SPIx_CR2 &= ~(ui)1;//f.LDMA_RX
	
	SPI1->SPIx_CR2 |= 1;//my.RXDMAEN
	SPI1->SPIx_CR2 |= 1<<1;//my.TXDMAEN
}

//����ж�����
void _spi_irq_setting(){
	//DMA
	NVIC->NVIC_ISER |= 1<<10;
	//SPI1
	NVIC->NVIC_ISER |= 1<<25;
}
//DMA����_�Ӳ���
static void _DMA_SETTING_SUB(enum _SPI_TM tm){
	switch(tm){
		case _T:
			DMA->CCR2 = 0;						//������
			DMA->CCR2 &= ~(ui)(3<<12);//PL
			DMA->CCR2 &= ~(ui)(3<<10);//MSIZE
			DMA->CCR2 &= ~(ui)(3<<8); //PSIZE
			DMA->CCR2 &= ~(ui)(1<<7);				//MINC
			DMA->CCR2 &= ~(ui)(1<<6); //PINC
			DMA->CCR2 &= ~(ui)(1<<5); //CIRC
			DMA->CCR2 &= ~(ui)1<<4;		//DIR
			DMA->CCR2 &=~(ui)(1<<3);	//TEIE
			DMA->CCR2 &=~(ui)(1<<2);	//HTIE
			DMA->CCR2 |= 2;						//TCIE
			DMA->CPAR2 = (ui)&SPI1->SPIx_DR;//�����ַ
			DMA->CNDTR2 = SPI_data.TxSIZE;//������
			DMA->CMAR2 = (ui)&RX_temp;//SPI_data.RxBUFFER;//Ҫ�������ݵ�Buffer
		
			DMA->CCR3 = 0;						//������
			DMA->CCR3 &= ~(ui)(3<<12);//PL
			DMA->CCR3 &= ~(ui)(3<<10);//MSIZE
			DMA->CCR3 &= ~(ui)(3<<8); //PSIZE
			DMA->CCR3 |= 1<<7;				//MINC
			DMA->CCR3 &= ~(ui)(1<<6); //PINC
			DMA->CCR3 &= ~(ui)(1<<5); //CIRC
			DMA->CCR3 |= 1<<4;				//DIR
			DMA->CCR3 &=~(ui)(1<<3);	//TEIE
			DMA->CCR3 &=~(ui)(1<<2);	//HTIE
			DMA->CCR3 |= 2;						//TCIE
			DMA->CPAR3 = (ui)&SPI1->SPIx_DR;//�����ַ
			DMA->CNDTR3 = SPI_data.TxSIZE;//������
			DMA->CMAR3 = (ui)SPI_data.TxDATA;//Ҫ���͵�����
			
			//DMA->CCR2 |= 1;						//EN 
			//DMA->CCR3 |= 1;						//EN
			return;
		case _R:
			DMA->CCR2 = 0;						//������
			DMA->CCR2 &= ~(ui)(3<<12);//PL
			DMA->CCR2 &= ~(ui)(3<<10);//MSIZE
			DMA->CCR2 &= ~(ui)(3<<8); //PSIZE
			DMA->CCR2 |= 1<<7;				//MINC
			DMA->CCR2 &= ~(ui)(1<<6); //PINC
			DMA->CCR2 &= ~(ui)(1<<5); //CIRC
			DMA->CCR2 &= ~(ui)1<<4;		//DIR
			DMA->CCR2 &=~(ui)(1<<3);	//TEIE
			DMA->CCR2 &=~(ui)(1<<2);	//HTIE
			DMA->CCR2 |= 2;						//TCIE
			DMA->CPAR2 = (ui)&SPI1->SPIx_DR;//�����ַ
			DMA->CNDTR2 = SPI_data.RxSIZE;//������
			DMA->CMAR2 = (ui)SPI_data.RxBUFFER;//Ҫ�������ݵ�Buffer
			
			DMA->CCR3 = 0;						//������
			DMA->CCR3 &= ~(ui)(3<<12);//PL
			DMA->CCR3 &= ~(ui)(3<<10);//MSIZE
			DMA->CCR3 &= ~(ui)(3<<8); //PSIZE
			DMA->CCR3 &= ~(ui)(1<<7);				//MINC
			DMA->CCR3 &= ~(ui)(1<<6); //PINC
			DMA->CCR3 &= ~(ui)(1<<5); //CIRC
			DMA->CCR3 |= 1<<4;				//DIR
			DMA->CCR3 &=~(ui)(1<<3);	//TEIE
			DMA->CCR3 &=~(ui)(1<<2);	//HTIE
			DMA->CCR3 |= 2;						//TCIE
			DMA->CPAR3 = (ui)&SPI1->SPIx_DR;//�����ַ
			DMA->CNDTR3 = SPI_data.RxSIZE;//������
			DMA->CMAR3 = (ui)&TX_temp;//SPI_data.TxDATA;//Ҫ���͵�����
			
			//DMA->CCR2 |= 1;						//EN
			//DMA->CCR3 |= 1;						//EN
			return;
		default:
			return;
			
	}
} 
/*DMA����*/
/*static void _DMA_SETTING(enum _SPI_DIR dir,uc *TX_D,uc TX_S,uc *RX_D,uc RX_S){*/
static void _DMA_SETTING(enum _SPI_TM tm){
	switch(tm){
		case _T://DMA3
			_DMA_SETTING_SUB(_T);
			DMA->CCR2 |= 1;						//EN 
			DMA->CCR3 |= 1;						//EN
			return;
		case _R://DMA2
			_DMA_SETTING_SUB(_R);
			DMA->CCR2 |= 1;						//EN 
			DMA->CCR3 |= 1;						//EN
			return;
		case _TR:
			
			return;
		case _RT:
			
			return;
		default :
			
			return;
	}
}
/*SPI��*/
__inline void _spi_enb(){
	SPI1->SPIx_CR1 |= 1<<6;
}
/*SPI��*/
__inline void _spi_dis(){
	SPI1->SPIx_CR1 &= ~(ui)(1<<6);
}
/*CRC����ʽ����*/

/*���RXFIFO*/
static void _clear_rxfifo(){
	while(SPI1->SPIx_SR & (3<<9))RX_temp = SPI1->SPIx_DR;
}

//�ӻ�ѡ��
static void _slave_select(){//��ʱ��ʵ�֣�����ֻ��spi��NSS����
	
}
/***************���ײ㺯����**********************/
/***************���жϲ��֡�******************/
//DMA����
void DMA2_TC_FLAG_HANDLER_FOR_SPI(){//����
	DMA->CCR3 &= ~(ui)1;//�ر�DMA
	switch(SPI_SF.TM){
		case _T://
			_clear_rxfifo();
			_status_clear_and_update_ex_status();
			break;
		case _R://
			_clear_rxfifo();
			_status_clear_and_update_ex_status();
			break;
		case _TR://
			switch(SPI_SF.Communication_phase){
				case _ONE:
					SPI_SF.Communication_phase = _TWO;
					_DMA_SETTING_SUB(_R);
					DMA->CCR2 |= 1;						//EN 
					DMA->CCR3 |= 1;						//EN
					return;
				case _TWO:
					goto final;
					break;
			}
			break;
		case _RT://
			switch(SPI_SF.Communication_phase){
				case _ONE:
					SPI_SF.Communication_phase = _TWO;
					_DMA_SETTING_SUB(_T);
					DMA->CCR2 |= 1;						//EN 
					DMA->CCR3 |= 1;						//EN
					return;
				case _TWO:
					goto final;
					break;
			}
			break;
		default :
final:			_clear_rxfifo();
			_status_clear_and_update_ex_status();
			break;
	}
	DMA->IFCR |= 1<<4;
	while(SPI1->SPIx_SR & (1<<7));
	_spi_dis();
	
}
void DMA3_TC_FLAG_HANDLER_FOR_SPI(){//����
	DMA->CCR3 &= ~(ui)1;//�ر�DMA3
	DMA->IFCR |= 1<<8;//���־λ
}
//SPI����
void SPI1_Handler(){
	while(1);
}
/***************���жϲ��֡�******************/
/***************���ⲿ���ò��֡�******************/
void SPI_INIT(){
	_spi_dis();
	_reset_and_enb_clock();
	_spi_gpio_setting();
	_spi_common_setting();
	_spi_irq_setting();
	_status_clear_and_update_ex_status();
	_spi_data_clear();
	SPI1_INIT = 1;
}

enum SPI_STATUS SPI_T(struct SPI_DATA data){
	if(!(data.TxDATA != 0) || !(data.TxSIZE != 0) || !(data.Status != 0))return _PARAMETER_ERROR;//�������
	if(!SPI1_INIT)SPI_INIT();
	if(SPI_SF.Using)return _SPI_BUSY;//����Ƿ��Ѿ�����������ʹ����
	_spi_dis();
	_status_update(_T,data);
	_clear_rxfifo();
	_DMA_SETTING(_T);
	_spi_enb();
	return _SPI_OK;
}

enum SPI_STATUS SPI_R(struct SPI_DATA data){
	if(!(data.RxBUFFER != 0) || !(data.RxSIZE != 0) || !(data.Status != 0))return _PARAMETER_ERROR;//�������
	if(!SPI1_INIT)SPI_INIT();
	if(SPI_SF.Using)return _SPI_BUSY;//����Ƿ��Ѿ�����������ʹ����
	_spi_dis();
	_status_update(_R,data);
	_clear_rxfifo();
	_DMA_SETTING(_R);
	_spi_enb();
	return _SPI_OK;
}

enum SPI_STATUS SPI_TR(struct SPI_DATA data){
	if(!(data.TxDATA != 0) || !(data.TxSIZE != 0) || !(data.RxBUFFER != 0) || !(data.RxSIZE != 0) || !(data.Status != 0))return _PARAMETER_ERROR;//�������
	if(!SPI1_INIT)SPI_INIT();//����Ƿ��Ѿ���ʼ��
	if(SPI_SF.Using)return _SPI_BUSY;//����Ƿ��Ѿ�����������ʹ����
	_spi_dis();
	_status_update(_TR,data);
	_clear_rxfifo();
	_DMA_SETTING(_T);
	//�����⣺������Ϻ�ֻ���ý����ߵ�DMA���ж�������Ƿ���...
	_spi_enb();
	return _SPI_OK;
}
/***************���ⲿ���ò��֡�******************/



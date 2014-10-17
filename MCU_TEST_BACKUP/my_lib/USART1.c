//��� 2014-9-15 05:32:38
/*********************************************************************
ʹ��notepad++��̵Ļ���˫������key_*Ȼ������������ͻ��ҵ�������Ĳ���
key_1:���ղ�������ͬʱʹ����DMA��MoBus��Time_out���ܣ�������յ����ݳ�
	  �������趨��buffer�Ĵ�С�Ļ�����ͬʱ�������������ܵ��жϺ������֣�
	  Ȼ�����ڴ����USART1_RxDAT���ݽṹ��rx_state�ᱻ����������ͬʱ
	  ���£����������θ������״̬
key_2:����DMA3���жϱ�MoBus��Time_out�жϿ�ܶ࣬����ֱ����Time_out�ж�
	  �������յ������Ƿ������ϣ��������� key_1 ���ϣ�
key_3:overrunҲ�Ǳ�MoBus��Time_out�жϿ�ܶ࣬����Ҫ����timeout������ô
	  ��Ҫ����overrun��overrun�ı�־λ������timeout�ж������

*********************************************************************/



#include "register.h"
#include "USART1.h"
#include "DMA.h"

#define TIMEOUTVALUE 20

static volatile enum _STATUS Can_receive = _idle,Can_send = _idle;//RX��TXռ�����:_idle,_busy
// static volatile USART1_TxDAT* Tx_lock_object = 0;
static volatile USART1_RxDAT* Rx_lock_object = 0;//��¼����������Ķ���
volatile enum _RX_STATE* ex_rx_state;
static uc Error = 0;
static uc init = 0;
// uc rx_tc_before_rt = 0;


void test(){
	//USART1->USARTx_CR1 &= ~(1<<3);
	USART1->USARTx_CR1 |= 1<<3;
}

void _USART1_DMA_IRQ_ENB(){
	USART1->USARTx_CR3 |= 1;   //EIE
	USART1->USARTx_CR1 |= 1<<6;//TCIE
	USART1->USARTx_CR1 |= 1<<26;//RTOIE
//	USART1->USARTx_CR2 |= 1<<23;//RTOEN,��������жϿ��أ����ǹ��ܿ���
//	DMA->CCR3 				 |= 1<<2;//TCIE//���λ���������ж���
	NVIC->NVIC_ISER 	 |= 1<<27;//USART1
	NVIC->NVIC_ISER    |= 1<<10;//DMA2_3
	
}

void USART1_TC_FLAG_HANDLER(void);
void USART1_ORE_FLAG_HANDLER(void);
void Framing_Error_HANDLER(void);
void USART1_RTOF_FLAG_HANDLER(void);

void USART1_Handler(){
	if(USART1->USARTx_CR1 & 1<<6 && USART1->USARTx_ISR & 1<<6){//TC
		USART1_TC_FLAG_HANDLER();
	}
	if(USART1->USARTx_CR3 & 1){//EIE-NE.FE.ORE
		Framing_Error_HANDLER();//key_3
	}
	if(USART1->USARTx_CR1 & 1<<26 && USART1->USARTx_ISR & 1<<11){//RTOIE RTOF
			USART1_RTOF_FLAG_HANDLER();
	}
}

enum _STATUS __calculate_and_set_BRR(ui DIV){
	if(USART1->USARTx_CR1 & 1)return the_UE_is_set;
	if(USART1->USARTx_CR1 & 1<<15 ){//������Ϊ8
		USART1->USARTx_BRR = (DIV & 0xfff0) | ((DIV & 0xf) >> 1);
		return success_to_set;
	}else{
		USART1->USARTx_BRR = DIV;
		return success_to_set;
	}
}
enum _STATUS _baud_rate_set(enum _OVERSAMPING oversamp,enum _BAUD baud){
	uc temp = 0;
	ui bps  = 0;
	if(USART1->USARTx_CR1 & 1)return the_UE_is_set;//�ж�UE
	if(oversamp == _over8){//����ǹ�����8
		USART1->USARTx_CR1 |= 1<<15;
		temp = 1;
	}else{
		USART1->USARTx_CR1 &= ~(1<<15);
		temp = 0;
	}
	switch(baud){
		case _1200bps:    bps = 1200;	 break;
		case _2400bps:    bps = 2400;	 break;
		case _9600bps:    bps = 9600;	 break;
		case _19200bps:	  bps = 19200; break;
		case _38400bps:   bps = 38400; break;
		case _57600bps:   bps = 57600; break;
		case _115200bps:  bps = 115200;break;
		case _230400bps:  bps = 230400;break;
		case _460800bps:  bps = 460800;break;
		case _921600bps:  bps = 921600;break;
		//default: return Invalid_parameter;
	}
	return __calculate_and_set_BRR((temp ? 2*USART1_CLOCK : USART1_CLOCK) / bps);
}

enum _STATUS _frame_format_select(enum _DATA_LENGTH len,enum _Parity parity,enum _STOP_BIT stop){
	if(USART1->USARTx_CR1 & 1)return the_UE_is_set;//�ж�UE
	switch(len){//�������ݵĳ���
		case _7_Data_bits:
			USART1->USARTx_CR1 |= 1<<28;
			USART1->USARTx_CR1 &= ~(1<<12);
			break;
		case _8_Data_bits:
			USART1->USARTx_CR1 &= ~(1<<12 | 1<<28);
			break;
		case _9_Data_bits:
			USART1->USARTx_CR1 &= ~(1<<28);
			USART1->USARTx_CR1 |= 1<<12;
			break;
	}
	switch(parity){//��żλ
		case nonuse_parity:
			USART1->USARTx_CR1 &= ~(1<<10);
			break;
		case Even_parity:
			USART1->USARTx_CR1 |= 1<<10;
			USART1->USARTx_CR1 &= ~(1<<9);
			break;
		case Odd_parity:
			USART1->USARTx_CR1 |= 0x2<<9;
			break;
	}
	switch(stop){//ֹͣλ
		case _1_stop_bit:
			USART1->USARTx_CR2 &= ~(2<<12);
			break;
		case _1_point_5_stop_bit:
			USART1->USARTx_CR2 |= 2<<12;
			break;
		case _2_stop_bit:
			USART1->USARTx_CR2 |= 1<<13;
			USART1->USARTx_CR2 &= ~(1<<12);
			break;
	}
	return success_to_set;
}

void _usart_enb_or_dis(enum _ENB ed){
	switch(ed){
		case _set_UE:	USART1->USARTx_CR1 |=  1;break;
		case _SET_TE:	USART1->USARTx_CR1 |=  8;break;
		case _SET_RE:	USART1->USARTx_CR1 |=  4;break;
		case _clr_UE:	USART1->USARTx_CR1 &= ~1;break;
		case _clr_TE:	USART1->USARTx_CR1 &= ~8;break;
		case _clr_RE:	USART1->USARTx_CR1 &= ~4;break;
	}
}

#define rx_m2m 	 (0<<14)
#define rx_pl 	 (0<<12)
#define rx_msize (0<<10)
#define rx_psize (0<<8)//8bits
#define rx_minc  (1<<7)
#define rx_pinc  (0<<6)
#define rx_circ  (0<<5)
#define rx_dir   (0<<4)
#define rx_teie  (0<<3)
#define rx_htie	 (0<<2)
#define rx_tcie  (1<<1) //key_2,����DMA3���ж��ٶȱ�RTOF��ܶ࣬���Բ���DMA������Ƿ�����ֱ꣬����USART��TIMEOUT�ж����ж�

#define tx_m2m 	(0<<14)
#define tx_pl 	(0<<12)
#define tx_msize (0<<10)
#define tx_psize (1<<8)//8bits
#define tx_minc (1<<7)
#define tx_pinc (0<<6)
#define tx_circ (0<<5)
#define tx_dir 	(1<<4)
#define tx_teie (0<<3)
#define tx_htie	(0<<2)
#define tx_tcie (0<<1)

void _DMA_set(enum _Direction dir,us d_size,uc* p){//����Ĭ����ΪSYSCFG���λΪĬ��ֵ
	switch(dir){
		case _reception://DMA3,���ʹ��I2C�ӿڵĻ�����ô��DMAͨ�����г�ͻ
			DMA->CCR3 	 = 0;				 //���DMA��������ֵ
			DMA->CCR3		 = rx_m2m|rx_pl|rx_msize|rx_psize|rx_minc|rx_pinc|rx_circ|rx_dir|rx_teie|rx_htie|rx_tcie;
			DMA->CPAR3	 = (ui) &USART1->USARTx_RDR;
			DMA->CNDTR3  = d_size;
			DMA->CMAR3	 = (ui) p;
			DMA->CCR3		|= 1;
			break;
		case _send://DMA2,���ʹ��I2C�ӿڵĻ�����ô��DMAͨ�����г�ͻ
			DMA->CCR2 	 = 0;				 //���DMA��������ֵ
			DMA->CCR2		 = tx_m2m|tx_pl|tx_msize|tx_psize|tx_minc|tx_pinc|tx_circ|tx_dir|tx_teie|tx_htie|tx_tcie;
			DMA->CPAR2	 = (ui) &USART1->USARTx_TDR;
			DMA->CNDTR2  = d_size;
			DMA->CMAR2	 = (ui) p;
			DMA->CCR2		|= 1;
			break;
	}
}

void _usart_clock_set(){
	RCC->RCC_CFGR3	 |= 3;		//��ѡ
	RCC->RCC_APB2ENR |= 1<<14;//ʱ��Դ���ٿ���
}

void _GPIO_setting_for_USART1(){
	RCC->RCC_AHBENR |= 1<<17;
	GPIOA->GPIOx_MODER &= 0xffc3ffff;
	GPIOA->GPIOx_MODER |= 0xa << 18;//PA9��10
	GPIOA->GPIOx_AFRH  &= ~(0xee << 4);
	GPIOA->GPIOx_AFRH  |= 0x11 << 4;
//		RCC->RCC_AHBENR |= 1<<18;
//		GPIOB->GPIOx_MODER &= ~(0xf<<12);
//    GPIOB->GPIOx_MODER |= 0xa << 12;//PB6��7
//	  GPIOB->GPIOx_AFRL &= ~(0xff000000);
}

static void update_ex_state(enum _RX_STATE p){//�����ⲿ��־λ�����
	//USART1->USARTx_CR2 &= ~(1<<23);//RTOEN
	Error = ex_rx_state ? 0:1;
	if(!ex_rx_state)return;
	*ex_rx_state = p;
	ex_rx_state = 0;
}

void TIMEOUT_SET(){
	USART1->USARTx_RTOR = TIMEOUTVALUE;//max:0xFFFFFF
	//USART1->USARTx_CR2 |= 1<<23;//RTOEN,��������жϿ��أ����ǹ��ܿ���
	USART1->USARTx_CR1 |= 1<<26;//Bit 26 RTOIE: Receiver timeout interrupt enable
}

void USART1_INIT_ALL(){//һЩ�����ĳ�ʼ��������ʱ�����ã��������GPIO����
	USART1->USARTx_CR1 &= ~1;
	// Tx_lock_object = 0;
	Rx_lock_object = 0;
	DMA_clock();//����DMA��ʱ��Դ
	_usart_clock_set();
	TIMEOUT_SET();//���ճ�ʱ����
	_baud_rate_set(_over16,_9600bps);
	_frame_format_select(_8_Data_bits,nonuse_parity,_1_stop_bit);
	_usart_enb_or_dis(_set_UE);
	USART1->USARTx_ICR |= 1<<6;//clr TC flag
	USART1->USARTx_CR3 |= 3<<6;//ʹ��DMA���������ͺͽ�������
	_USART1_DMA_IRQ_ENB();//����USART1�ж�,!!!!!!��Ϊ�ж�TC��־λ������������UEλ������������޷����У������⺯�����������һ�����У�
	_GPIO_setting_for_USART1();
	init = 1;
}

// check_lock_object(){
	
// }



enum _LOCK USART1_RX_UNLOCK(USART1_RxDAT* p){
	if(Rx_lock_object == p){
		Rx_lock_object = 0;
		return _unlock;
	}else{
		return no_lock_before;
	}
}

/* enum _STATUS USART1_SEND(USART1_TxDAT* p){//���óɹ��Ļ��᷵��success_to_set�����ڷ������ݵĻ�����_busy
	// if(!Tx_lock_object){
		// Tx_lock_object = p;
	// }else if(Tx_lock_object = p){
		
	// }else if(p != Tx_lock_object) return _lock_error;//�ж϶���
	//while(SEND_BEFORE_RECEIVE == _busy);		 //TX��RXռ�ã�����
	if(Can_send == _busy)return _busy;
	Can_send = _busy;
	_DMA_set(_send,p->tx_size,p->tx_p);
	_usart_enb_or_dis(_SET_TE);
	return success_to_set;
} */

void USART1_SEND(USART1_TxDAT* p){
	if(!init){
		USART1_INIT_ALL();
	}
	// if(!Tx_lock_object){
		// Tx_lock_object = p;
	// }else if(Tx_lock_object = p){
		
	// }else if(p != Tx_lock_object) return _lock_error;//�ж϶���
	//while(SEND_BEFORE_RECEIVE == _busy);		 //TX��RXռ�ã�����
	while(Can_send == _busy);
	Can_send = _busy;
	_DMA_set(_send,p->tx_size,p->tx_p);
	_usart_enb_or_dis(_SET_TE);
	return;
}

//�˺�������ֵ�У�success_to_set��Invalid_parameter��_busy��_lock_error
enum _STATUS USART1_RECEIVE(USART1_RxDAT* p){
	if(!init){
		USART1_INIT_ALL();
	}
	if(p == 0)return Invalid_parameter;			//�������Ƿ���Ч
	if(!Rx_lock_object){						//Rx_lock_object == 0
		Rx_lock_object = p;						//��������
	}else if(Rx_lock_object != p){				//Rx_lock_object != 0�������������֮ǰ�������Ĳ������Ǻ�
		return _lock_error;
	}
//	while(SEND_BEFORE_RECEIVE == _busy);		 //TX��RXռ�ã�����
	ex_rx_state = &p->rx_state;
	*ex_rx_state = _ready;////
	switch(Can_receive){
		case _idle:
			Can_receive = _busy;
			p->rx_state = _receiving;
			USART1->USARTx_CR2 |= 1<<23;//RTOEN///////////////////////////////////////
			_DMA_set(_reception,p->rx_size,p->rx_p);
			_usart_enb_or_dis(_SET_RE);
			return success_to_set;
		case _busy:
			p->rx_state = _done;
			return _busy;
		default :Can_receive = _idle;
			p->rx_state = _done;
			return _busy;
	}
}
/*û�õĴ���Σ���ʱ����ɾ��

enum _STATUS USART1_SEND_BEFORE_RECEIVE(USART1_DAT* p){
	if(p != _lock_object) return _lock_error;//�ж϶���
	if(SEND_BEFORE_RECEIVE == _busy)return _busy;
	while(Can_send != _idle && Can_receive == _idle);//TX��RXռ�ã�����
	SEND_BEFORE_RECEIVE = _busy;
	//send
	_DMA_set(_send,p->tx_size,p->tx_p);
	_usart_enb_or_dis(_SET_TE);
	//receive
	p->rx_state = _receiving;
	_DMA_set(_reception,p->tx_size,p->rx_p);
	_usart_enb_or_dis(_SET_RE);
	USART1->USARTx_CR2 |= 1<<23;//RTOEN
	return success_to_set;
}

*/


//�������ԡ���

//�������ԡ���

__inline void close_RX_DMA(){
	DMA->CCR3 &= ~1;//�رս��ղ��ֵ�DMA3
}

__inline void close_TX_DMA(){
	DMA->CCR2 &= ~1;//�رս��ղ��ֵ�DMA3
}

//ʵ�ֱ�����ͱ��������жϴ�������
//!���Ͳ��ֵĴ���������
__weak void USART1_TC_FLAG_HANDLER(){
//	if(!(USART1->USARTx_CR1 & 1)){
//		USART1->USARTx_CR1 |= 1;//set UE 
//	}
	close_TX_DMA();
	DMA->IFCR |= 1<<4;//���DMA2�����б�־
	USART1->USARTx_ICR |= 1<<6;//clr TC flag
	Can_send = _idle;//��ΪSEND_BEFORE_RECEIVE����_busyʱCan_send�������_busy ///
	
}
//!���ղ��ֵĴ���������
	void DMA3_TC_FLAG_HANDLER_FOR_USART(){//DMA�������RX
	//USART1->USARTx_CR2 &= ~(1<<23);//RTOEN///////////////////////////////////////
	// rx_tc_before_rt = 1; //key_1,
	//_usart_enb_or_dis(_clr_RE);//�������ݵ�ָ��������Ϻ�رս���
	close_RX_DMA();//�ر�DMA
	DMA->IFCR |= 1<<8;//���DMA3�����б�־
	// Can_receive = _idle;
	// update_ex_state(_done);
}

__weak void USART1_RTOF_FLAG_HANDLER(){//RTOF
	//USART1->USARTx_CR2 &= ~(1<<23);//RTOEN///////////////////////////////////////
	USART1->USARTx_ICR |= (1<<11 | 8);//8����������־λ
	/* if(rx_tc_before_rt){//key_1,
		rx_tc_before_rt=0;
		return;
	} */
	_usart_enb_or_dis(_clr_RE);
	close_RX_DMA();//�ر�DMA
	update_ex_state(_done);
	
	Can_receive = _idle;
}

__weak void USART1_ORE_FLAG_HANDLER(){//ORE //key_3
	USART1->USARTx_ICR |= 8;
	close_RX_DMA();//�ر�DMA
	_usart_enb_or_dis(_clr_RE);
	Can_receive = _idle;
	update_ex_state(_done);
}

__weak void Framing_Error_HANDLER(){
	uc t = 0; 
	if(USART1->USARTx_ISR & 2){//FE
		USART1->USARTx_ICR |= 2;
		t=1;
	}
	if(USART1->USARTx_ISR & 4){//NF
		USART1->USARTx_ICR |= 4;
		t=1;
	}
	if(USART1->USARTx_ISR & 8){//ORE //key_3
		USART1->USARTx_ICR |= 8;
		//t=1;//��RTOF��ͻ�������־λ����RTOF�ж�
	}
	if(t){
		close_RX_DMA();//�ر�DMA
		_usart_enb_or_dis(_clr_RE);
		Can_receive = _idle;
		update_ex_state(_done);
	}
	
}
//ʵ�ֱ��������жϴ�������


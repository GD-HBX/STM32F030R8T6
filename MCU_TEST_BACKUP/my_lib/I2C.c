//2014-7-6 23:21:27 ���
#include "register.h"
#include "I2C.h"
#include "DMA.h"

#define PRESC  (1 		<< 28)//�ֲ�Table 66�е�ֵ
#define SCLL	 (0xc7	<<  0)//�ֲ�Table 66�е�ֵ
#define SCLH	 (0xc3	<<  8)//�ֲ�Table 66�е�ֵ
#define SDADEL (0x2		<< 16)//�ֲ�Table 66�е�ֵ
#define SCLDEL (0x4		<< 20)//�ֲ�Table 66�е�ֵ

//uc _IO _restart;
I2C_data _start_data,_restart_data;
void I2C1_INIT_ALL(){
  I2C1_CLOCK_ENB();
	I2C1_INIT();
	IRQ_ENB();
	I2C1_ENB();
	GPIOB->GPIOx_MODER |= 0x0000000a << 12;//GPIO:PB6��7
	GPIOB->GPIOx_AFRL  |= 0x11000000; 		 //GPIO:PB6��7
	DMA_FOR_I2C1_SETTING();
}

void I2C1_CLOCK_ENB(){
	RCC->RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->RCC_CFGR3	 |= RCC_CFGR3_I2C1SW;//ѡ������I2C1��ʱ��Դ
}

void I2C1_INIT(){
	//������Ŀǰû����ģ���������������
	
	I2C1->I2Cx_TIMINGR = PRESC | SCLL | SCLH | SDADEL | SCLDEL;//ʱ������
}

void I2C_TCR_FLAG_HANDLER(void);
void I2C_TC_FLAG_HANDLER(void);
void I2C_NACKF_FLAG_HANDLER(void);
void I2C_STOPF_FLAG_HANDLER(void);
void I2C_RXNE_FLAG_HANDLER(void);
void I2C_TXIS_FLAG_HANDLER(void);

void I2C1_Handler(){//�жϴ�����
	if(I2C1->I2Cx_ISR & 1<<2 && I2C1->I2Cx_CR1 & 1<<2){//RXNE
		I2C_RXNE_FLAG_HANDLER();
		return;
	}
	if(I2C1->I2Cx_ISR & 1<<1 && I2C1->I2Cx_CR1 & 1<<1){//TXIS
		I2C_TXIS_FLAG_HANDLER();
		return;
	}
	if(I2C1->I2Cx_CR1 & 1<<6){//TCIE
		if(I2C1->I2Cx_ISR & 1<<7){//TCR
			I2C_TCR_FLAG_HANDLER();
			//return;
		}
		if(I2C1->I2Cx_ISR & 1<<6){//TC
			I2C_TC_FLAG_HANDLER();
			//return;
		}
	}
	if(I2C1->I2Cx_ISR & 1<<4 && I2C1->I2Cx_CR1 & 1<<4){//NACK
		I2C_NACKF_FLAG_HANDLER();
	} 
	if(I2C1->I2Cx_ISR & 1<<5 && I2C1->I2Cx_CR1 & 1<<5){//STOP
		I2C_STOPF_FLAG_HANDLER();
	}
}

void IRQ_ENB(){
	NVIC->NVIC_ISER |= 0x00000001 << 23;
	I2C1->I2Cx_CR1 |= 0x00000001 << 6;
	I2C1->I2Cx_CR1 |= 0x00000001 << 4;
	I2C1->I2Cx_CR1 |= 0x00000001 << 5;
}

void DMA_FOR_I2C1_SETTING(){
	DMA_clock();
	
}

#define tx_m2m 	(0<<14)
#define tx_pl 	(0<<12)
#define tx_msize (0<<10)
#define tx_psize (2<<8)
#define tx_minc (1<<7)
#define tx_pinc (0<<6)
#define tx_circ (0<<5)
#define tx_dir 	(1<<4)
#define tx_teie (0<<3)
#define tx_htie	(0<<2)
#define tx_tcie (0<<1)

#define rx_m2m 	 (0<<14)
#define rx_pl 	 (0<<12)
#define rx_msize (0<<10)
#define rx_psize (2<<8)
#define rx_minc  (1<<7)
#define rx_pinc  (0<<6)
#define rx_circ  (0<<5)
#define rx_dir   (0<<4)
#define rx_teie  (0<<3)
#define rx_htie	 (0<<2)
#define rx_tcie  (0<<1)


void I2C1_SEND_RECEIVE_DATA_WITH_DMA(enum DIR _dir,uc d_size,uc *p){
	if(_dir == i2c_tx){			 //����Ƿ�������
		DMA->CCR2 	 = 0;				 //���DMA��������ֵ
		DMA->CCR2		= tx_m2m|tx_pl|tx_msize|tx_psize|tx_minc|tx_pinc|tx_circ|tx_dir|tx_teie|tx_htie|tx_tcie;
		DMA->CPAR2	 = (ui) &I2C1->I2Cx_TXDR;
		DMA->CNDTR2  = d_size;
		DMA->CMAR2	 = (ui) p;
		DMA->CCR2		|= 1;
	}else if(_dir == i2c_rx){//����ǽ�������
		DMA->CCR3 	 = 0;				 //���DMA��������ֵ
		DMA->CCR3		= rx_m2m|rx_pl|rx_msize|rx_psize|rx_minc|rx_pinc|rx_circ|rx_dir|rx_teie|rx_htie|rx_tcie;
		DMA->CPAR3	 = (ui) &I2C1->I2Cx_RXDR;
		DMA->CNDTR3  = d_size;
		DMA->CMAR3	 = (ui) p;
		DMA->CCR3		|= 1;
	}
}

void I2C1_MASTER_TRANSFER(I2C_data *start){//��������ģʽ	
	I2C1->I2Cx_CR2 &= ~(1<<24);//��ʱ��ʹ��reloadģʽ
	if(start->_autoend == yes_auto){//!!!!!!!!!!!!!!!!!!//�ر�ע�⣬NBYTESΪ0������¸ı�ΪAUTOENDģʽ����start�����Ļ�����startǰ������һ��STOP���������Ըı�AUTOENDģʽҪ������NBYTES�����
		I2C1->I2Cx_CR2 |= 1<<25;//����Ϊ�Զ�����ģʽ��AUTOEND��
	}else{
		I2C1->I2Cx_CR2 &= ~(1<<25);//�ر��Զ�����ģʽ��AUTOEND��
	}
	if(start->_dir == i2c_tx){
		I2C1_SEND_RECEIVE_DATA_WITH_DMA(start->_dir,start->d_size,start->p);
		I2C1->I2Cx_CR2 &= ~(0x00000001 << 10);//���ô��䷽��:д
	}else if(start->_dir == i2c_rx){
		I2C1_SEND_RECEIVE_DATA_WITH_DMA(start->_dir,start->d_size,start->p);
		I2C1->I2Cx_CR2 |= (0x00000001 << 10);//���ô��䷽��:��
	}
	I2C1->I2Cx_CR2 &= ~(0x7f << 1);				//����ӻ���ֵַ
	I2C1->I2Cx_CR2 |= (start->addr<<1);	 				//���ôӻ���ַ
	I2C1->I2Cx_CR2 &= ~(0xff<<16);				//���NBYTES�е�ֵ
	I2C1->I2Cx_CR2 |= ((ui)start->d_size & 0xff) << 16; //����I2C����Ҫ���͵��ֽ���
	I2C1->I2Cx_CR1 |= 0x3 << 14;					//���ý��պͷ���ʹ��DMA����
	I2C1->I2Cx_CR2 |= (1<<13);						//��ʼ����
}



void I2C1_SLAVE_TRANSFER(enum DIR _dir,uc addr,uc d_size,uc *p){//�ӻ�����ģʽ
	//δʵ�֣�����Ҫ��д
}


void I2C1_ENB(){
	I2C1->I2Cx_CR1 |= I2C_CR1_PE;
}

void I2C1_DIS(){
	I2C1->I2Cx_CR1 &= ~I2C_CR1_PE;
}

void I2C1_SOFTWARE_RESET(){
	if(!(I2C1->I2Cx_CR1 & 1)){return;}
	I2C1_DIS();
	I2C1_ENB();
}



/*�����������жϴ����֡���������*/


__weak void I2C_RXNE_FLAG_HANDLER(void){
	
}

__weak void I2C_TXIS_FLAG_HANDLER(void){
	
}

__weak  void I2C_TCR_FLAG_HANDLER(void){
	
}

__weak  void I2C_TC_FLAG_HANDLER(void){
	if(I2C1->I2Cx_ISR&0x8000 && _restart_data._autoend == yes_auto){			//restart
		_restart_data._dir = i2c_rx;								//restart���ֵķ���Ĭ��Ϊ�ȷ���
		_restart_data._autoend = no_auto;//Ҫ�ĳɷ��Զ�ģʽ��ԭ���ǣ�һ����NBYTES��RELOAD������0ʱ����TC��������Ǹı�AUTOENDΪ1�����STOP������
		I2C1_MASTER_TRANSFER(&_restart_data);
		I2C1->I2Cx_CR2 |= 1<<25;//����RESTART��������ֱ�Ӹı�AUTOEND��־λΪ1��
	}else{								   										//stop
		I2C1->I2Cx_CR2 |= 1 << 14;
	}
}

__weak  void I2C_NACKF_FLAG_HANDLER(void){
	I2C1->I2Cx_CR2 &= ~0x00ff0000;//��NBYTES
	I2C1->I2Cx_ICR |= 0x10;//��NACKF
	_restart_data._autoend = no_auto;
}


__weak  void I2C_STOPF_FLAG_HANDLER(void){
	I2C1->I2Cx_ICR |= 0x20;//��NACKF
	_restart_data._autoend = no_auto;
}


/*�����������жϴ����֡���������*/

/*�����������ⲿ�������ݴ��䲿�֡���������*/
uc e2prom_byte_write(uc addr,uc d_size,uc *p){
	if(I2C1->I2Cx_ISR & 1<<15){
		return 0;
	}
	_start_data.addr = addr;
	_start_data.d_size = d_size;
	_start_data.p = p;
	_start_data._autoend = yes_auto;
	_start_data._dir = i2c_tx;
	_restart_data._autoend = no_auto;
	I2C1_MASTER_TRANSFER(&_start_data);
	return 1;
}

uc e2prom_byte_read(uc addr,uc d_size[2],uc *p[2]){
	if(I2C1->I2Cx_ISR & 1<<15){
		return 0;
	}
	_start_data.addr = addr;
	_start_data.d_size = d_size[0];
	_start_data.p = p[0];
	_start_data._autoend = no_auto;
	_start_data._dir = i2c_tx;
	_restart_data.addr = addr;
	_restart_data.d_size = d_size[1];
	_restart_data.p = p[1];
	_restart_data._autoend = yes_auto;
	_restart_data._dir = i2c_rx;
	I2C1_MASTER_TRANSFER(&_start_data);
	return 1;
}

/*�����������ⲿ�������ݴ��䲿�֡���������*/





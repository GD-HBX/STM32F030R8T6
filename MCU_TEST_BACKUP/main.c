#include "my_lib/register.h"
#include <stdlib.h>
#include "my_lib/DMA.h"
#include "SYSCFG.h"
#include "EXTI.h"
#include "NVIC.h"
#include "TIMx.h"
#include "TEST.h"
#include "RTC.h"
#include "my_lib/I2C.h"
#include "my_lib/USART1.h"
#include "ASCII.h"
#include "my_lib/log.h"
#include "my_lib/SPI.h"
//#include "stm32f0xx.h"                  // Device header
void delay(int i);
ui temp=0xFFFFFFFF;
ui _IO result=0;
uc jieguo;
float flag;
extern uc turnoff;///
/////////////////////DMA_TEST///////////////////////////
#define DMA_DATA_SIZE 20
#define ADC_WENDU (float)(4096/165)
extern void DMA_chuanshu(int fangxiang,int ma,int pa,int data_size);
extern void DMA_clock(void);
extern void ADC_CLOCK_ENB(void);
extern void ADC_CFG(void);
int m_temp[DMA_DATA_SIZE],p_temp[DMA_DATA_SIZE];

void DMA_TEST_FUNC(int *p,int size,int first_value,int mode){
	int temp;
	
	switch(mode){
		case 1:for(temp = 0;temp < size;temp++){
						 *p = first_value + temp;
						 p++;
					 }
					 break;
		case 2:for(temp = 0;temp < size;temp++){
						*p = first_value;
						p++;
					 }
	}
}
/////////////////////DMA_TEST///////////////////////////




//void main_clean(USART1_DAT* clr){
//	uc t;
//	for(t=0;t<main_test_size;t++){
//		(clr->rx_p)[t] = 0;
//	}
//}


//测试发送部分的
// int main(){
	// USART1_TxDAT tmp={sizeof("Fuck you world!!\r\n"),"Fuck you world!!\r\n"},
  // tmp1={sizeof("hello world!!\r\n"),"hello world!!\r\n"};
	// USART1_INIT_ALL();
	// while(1){
		// USART1_SEND(&tmp);
		// delay(2000);
		// USART1_SEND(&tmp1);
		// delay(2000);
	// }
// }



//测试接收部分的_下
void tx_setting(uc* p,us size,USART1_TxDAT* tp){
	tp->tx_p = p;
	tp->tx_size = size;
}

uc check(uc* ps,uc* pc){
	uc temp = 0;
	while(pc[temp]!=0){
		if(ps[temp] != pc[temp])return 0;
		temp++;
	}
	return 1;
}


int main(){
	volatile uc ts;
	volatile ui dizhi;
	uc key = 1;
	volatile uc zhuangtai = 0;
	struct SPI_DATA data;
	//uc test[] = {0xAA,0x55};
	data.TxDATA = "Hello World!!";
	data.TxSIZE = sizeof "Hello World!!";
	data.Status = &zhuangtai;
	dizhi = (ui)&key;
again:	
	ts = 1; 
	key = 1;
	while(key++);
	key = 1;
	while(!SPI_T(data));
	//ts = SPI_T(data);
	while(key++);
	goto again;
}





















void delay(int i){
	int ii;
	for(;i>0;i--){
		ii = 500;
		while(--ii);
	}
}
/*1*/
void BUTTON_PA0_DOWN(){
	//GPIOC->GPIOx_BSRR = GPIOC->GPIOx_IDR & 0x100 ? 0x1000000:0x100;
	temp = ~temp;
	EXTI->EXTI_PR |= 1;
	//GPIOC->GPIOx_BSRR = 0x2000000;
}
void CUOWU(){
	char i;
	while(i++);
}



/*1*/
















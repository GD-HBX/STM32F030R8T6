//2014-7-6 23:21:27 恶搞
#include "register.h"
#include "DMA.h"

void DMA_chuanshu(int fangxiang,int ma,int pa,int data_size){           //这个函数目前只是用通道1
	DMA->CCR1   = 0;
	DMA->CMAR1  = ma;
	DMA->CPAR1  = pa;
	DMA->CNDTR1 = data_size;
	switch(fangxiang){
		case m2m:DMA->CCR1 = 0x4AC1;
						 break;
		case m2p:DMA->CCR1 = 0xAF1;
						 break;
		case p2m:DMA->CCR1 = 0xAE1;
						 break;
		case DMA_TEST:DMA->CCR1 = 0x921;//921:ADC
						 break;
		default :DMA->CCR1 = 0;
						 break;
	}
}

void DMA_clock(){
	RCC->RCC_AHBENR  |= 0x1;	//开启DMA的时钟
}
//中断处理部分↓↓↓↓


void DMA1_Handler(){
	
}
__weak void DMA3_TC_FLAG_HANDLER_FOR_USART(){
	
}

__weak void DMA3_TC_FLAG_HANDLER_FOR_SPI(){
	
}
__weak void DMA2_TC_FLAG_HANDLER_FOR_SPI(){
	
}

void DMA2_3_Handler(){
	if(DMA->ISR & 1<<4){//DMA2_Global
		if(DMA->ISR & 1<<5){//DMA2_TC_FLAG
			DMA2_TC_FLAG_HANDLER_FOR_SPI();
		}
	}
	if(DMA->ISR & 1<<8){//DMA3_Global
		if(DMA->ISR & 1<<9){//DMA3_TC_FLAG
			//DMA3_TC_FLAG_HANDLER_FOR_USART();
			DMA3_TC_FLAG_HANDLER_FOR_SPI();
		}
	}
}

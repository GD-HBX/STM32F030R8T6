#include "my_lib/register.h"
void RCC_INIT(void){
	RCC->RCC_CR |= 0x01;					//开启内部HSI
	while(!(RCC->RCC_CR & 0x2));	//检测HSI稳定否
	RCC->RCC_CFGR &= 0xFFFFFFFc;	//选择HSI为时钟源
	while(RCC->RCC_CFGR & 0xc0);	//如果时钟源是HSI才放行
	//RCC->RCC_AHBRSTR |= 0x80000;	//重置GPIOC所有寄存器，
	RCC->RCC_AHBENR  |= 0xE0000;	//开启GPIO A,B和C的时钟
}

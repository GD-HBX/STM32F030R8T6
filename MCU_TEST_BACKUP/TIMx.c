#include "my_lib/register.h"
#include "TIMx.h"

void TIM1_IO_CFG(){
	GPIOA->GPIOx_MODER  |= 2<<2*7 | 2<<2*8;
	GPIOA->GPIOx_OTYPER &= (1|2) << 7;
	GPIOA->GPIOx_OSPEEDR = 3<<2*7 | 3<<2*8;
	GPIOA->GPIOx_AFRH		|= 2;
	GPIOA->GPIOx_AFRL		|= 2 <<4*7;
}

void TIM1_CLOCK_ENB(){
	RCC->RCC_APB2RSTR |= RCC_TIM1_CLK_AND_RST;//把TIM1所有寄存器复位
	RCC->RCC_APB2RSTR &= ~RCC_TIM1_CLK_AND_RST;
	RCC->RCC_APB2ENR  |= RCC_TIM1_CLK_AND_RST;
	
}

void TIM3_CLOCK_ENB(){
	RCC->RCC_APB1RSTR |= RCC_TIM3_CLK_AND_RST;//把TIM1所有寄存器复位
	RCC->RCC_APB1RSTR &= ~RCC_TIM3_CLK_AND_RST;
	RCC->RCC_APB1ENR  |= RCC_TIM3_CLK_AND_RST;
	
}

void TIM1_CFG(){
	TIM1_IO_CFG();
	TIM1->TIMx_ARR    = 0x0050;		   //PWN 100kHZ = 80
	TIM1->TIMx_RCR    = 0;					 //设置重复寄存器
	TIM1->TIMx_SMCR  &= ~0x00000007; //禁用从模式
	TIM1->TIMx_PSC    = 0;					 //禁用分频
	TIM1->TIMx_CCR1   = 0x0028;			 //PWN 50%
	TIM1->TIMx_CCMR1 &= ~0x00000003; //设置为输出模式
	TIM1->TIMx_CCMR1 |= (6<<4);			 //设置输出模式
	TIM1->TIMx_CCER  |= 1|4;					 //设置通道oc1和oc1n输出PWN信号
	TIM1->TIMx_BDTR  |= 1<<15;			 //设置MOE
	TIM1->TIMx_EGR   |= 1;					 //更新计时器的基本单元
	TIM1->TIMx_CNT    = TIM1->TIMx_ARR - 1;
}

void TIM3_CFG(){

}

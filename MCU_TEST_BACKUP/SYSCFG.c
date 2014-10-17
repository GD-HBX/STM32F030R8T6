#include "my_lib/register.h"
void SYSCFG_CLOCK_ENB(){
	RCC->RCC_APB2ENR |= 1;
}

void SYSCFG_CFG(){
	SYSCFG->SYSCFG_EXTICR1 &= ~(ui)0xff;
}

#include "my_lib/register.h"
void EXTI_CFG(){
	EXTI->EXTI_IMR  |= 1;
	EXTI->EXTI_RTSR |= 1;
}


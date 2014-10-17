#include "my_lib/register.h"
void NVIC_ENB(ui value){
	NVIC->NVIC_ISER |= 0x20;//enb IRQ1
	//NVIC->NVIC_ISER |= 1 << 9;//enb DMA1;
}

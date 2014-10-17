#include "my_lib/register.h"
void GPIO_CFG(){
	/*GPIOCÉèÖÃ*/
	GPIOC->GPIOx_MODER |= 0x50000;
	GPIOC->GPIOx_OSPEEDR |= 0xF0000;
	GPIOC->GPIOx_PUPDR |= 0x50000;
	/*GPIOAÉèÖÃ*/
	GPIOA->GPIOx_MODER &= ~(ui)0x3;
	GPIOA->GPIOx_PUPDR |= 0xA6AAAAAA;//0x2;
	GPIOA->GPIOx_OSPEEDR |= 0x3;
}

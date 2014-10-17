#include "my_lib/register.h"
void CRC_ENB(){
	RCC->RCC_AHBENR |= 0x40;
}



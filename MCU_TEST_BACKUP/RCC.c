#include "my_lib/register.h"
void RCC_INIT(void){
	RCC->RCC_CR |= 0x01;					//�����ڲ�HSI
	while(!(RCC->RCC_CR & 0x2));	//���HSI�ȶ���
	RCC->RCC_CFGR &= 0xFFFFFFFc;	//ѡ��HSIΪʱ��Դ
	while(RCC->RCC_CFGR & 0xc0);	//���ʱ��Դ��HSI�ŷ���
	//RCC->RCC_AHBRSTR |= 0x80000;	//����GPIOC���мĴ�����
	RCC->RCC_AHBENR  |= 0xE0000;	//����GPIO A,B��C��ʱ��
}

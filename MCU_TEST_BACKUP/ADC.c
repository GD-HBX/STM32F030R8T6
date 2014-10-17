#include "my_lib/register.h"
#include "ADC.h"
ui ADC_TEST = 0;
void ADC_CLOCK_ENB(){//��ADC��ʱ�Ӻ͸����ϵ�
	RCC->RCC_APB2RSTR |= RCC_ADC_CLK_AND_RST;
	RCC->RCC_APB2RSTR &= ~RCC_ADC_CLK_AND_RST;
	RCC->RCC_APB2ENR  |= RCC_ADC_CLK_AND_RST;
	ADC->ADC_CFGR2		&= 0x3FFFFFFF;
}

void ADC_CFG(){
	ADC->ADC_CR |= ADC_CR_ADCAL;
	while(ADC->ADC_CR & 0x80000000);
	ADC->ADC_CR |= ADC_CR_ADEN;
	ADC->ADC_CHSELR |= ADC_CHL;
	ADC->ADC_CFGR1	|= ADC_CFGR1_CONT_Bit;
	ADC->ADC_CFGR1	&= ADC_CFGR1_EXTEN_Bit;
	ADC->ADC_CFGR1	&= ~ADC_CFGR1_DISCEN;
	ADC->ADC_SMPR		|= ADC_SMPR_SMP;
	ADC->ADC_CFGR1  |= (ADC_CFGR1_WAIT | ADC_CFGR1_AUTOFF);
	ADC->ADC_CFGR1  |= ADC_CFGR1_OVRMOD;
	ADC->ADC_CFGR1  &= ~ADC_CFGR1_RSE;
	ADC->ADC_CFGR1	|= ADC_CFGR1_DMACFG;
	ADC->ADC_CFGR1	|= ADC_CFGR1_DMAEN;
}

void  ADC_START(){
	ADC->ADC_CR |= ADC_CR_ADSTART;  
}
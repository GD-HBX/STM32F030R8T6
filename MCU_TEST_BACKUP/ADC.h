#define RCC_ADC_CLK_AND_RST (0x00000001 <<  9)

#define ADC_DR 							(0x40012400 + 0x40)

#define ADC_CHL             (0x00000001 <<  9)
#define ADC_CFGR1_CONT_Bit	(0x00000001 << 13)
#define ADC_CFGR1_EXTEN_Bit ~(0x00000003 << 10)
#define ADC_CFGR1_DISCEN		(0x00000001 << 16)
#define ADC_SMPR_SMP				(0x00000003 <<  0)
#define ADC_CFGR1_WAIT			(0x00000001 << 14)
#define ADC_CFGR1_AUTOFF		(0x00000001 << 15)
#define ADC_CFGR1_OVRMOD		(0x00000001 << 12)	
#define ADC_CFGR1_ALIGN			(0x00000001 <<  5)
#define ADC_CFGR1_RSE				(0x00000003	<<	3)
#define ADC_CFGR1_DMACFG		(0x00000001 <<  1)
#define ADC_CFGR1_DMAEN			(0x00000001 <<	0)

#define ADC_CR_ADEN					(0x00000001 <<	0)
#define ADC_CR_ADDIS				(0x00000001 << 	1)
#define ADC_CR_ADSTART			(0x00000001 <<	2)
#define ADC_CR_ADSTP				(0x00000001	<<	4)
#define ADC_CR_ADCAL				((ui)0x00000001	<< 31)

void ADC_CLOCK_ENB(void);
void ADC_CFG(void);
void  ADC_START(void);

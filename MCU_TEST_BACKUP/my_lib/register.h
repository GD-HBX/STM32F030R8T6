//2014-7-6 23:21:27 恶搞
#define _IO volatile

#define RTC_BASE 0x40002800

typedef unsigned int ui;
typedef unsigned short us;
typedef unsigned char uc;
typedef struct {					//已经检查过，寄存器定义没问题
	_IO ui RCC_CR;
	_IO ui RCC_CFGR;
	_IO ui RCC_CIR;
	_IO ui RCC_APB2RSTR;
	_IO ui RCC_APB1RSTR;
	_IO ui RCC_AHBENR;
	_IO ui RCC_APB2ENR;
	_IO ui RCC_APB1ENR;
	_IO ui RCC_BDCR;
	_IO ui RCC_CSR;
	_IO ui RCC_AHBRSTR;
	_IO ui RCC_CFGR2;
	_IO ui RCC_CFGR3;
	_IO ui RCC_CR2;
} RCC_TYPE;

typedef struct{
	_IO ui GPIOx_MODER;
	_IO ui GPIOx_OTYPER;
	_IO ui GPIOx_OSPEEDR;
	_IO ui GPIOx_PUPDR;
	_IO ui GPIOx_IDR;
	_IO ui GPIOx_ODR;
	_IO ui GPIOx_BSRR;
	_IO ui GPIOx_LCKR;
	_IO ui GPIOx_AFRL;
	_IO ui GPIOx_AFRH;
	_IO ui GPIOx_BRR;
} GPIOx_TYPE;

typedef struct{
	_IO ui DR;
	_IO ui IDR;
	_IO ui CR;
	    ui RESERVED2;
	_IO ui INIT;
} CRC_TYPE;

typedef struct{
	_IO ui PWR_CR;
	_IO ui PWR_CSR;
} PWR_TYPE;

typedef struct{					//已经检查过，寄存器定义没问题
	_IO ui ISR;
	_IO ui IFCR;
	_IO ui CCR1;
	_IO ui CNDTR1;
	_IO ui CPAR1;
	_IO ui CMAR1;
		ui RESERVED1;
	_IO ui CCR2;
	_IO ui CNDTR2;
	_IO ui CPAR2;
	_IO ui CMAR2;
		ui RESERVED2;
	_IO ui CCR3;
	_IO ui CNDTR3;
	_IO ui CPAR3;
	_IO ui CMAR3;
		ui RESERVED3;
	_IO ui CCR4;
	_IO ui CNDTR4;
	_IO ui CPAR4;
	_IO ui CMAR4;
	_IO ui CCR5;
	_IO ui CNDTR5;
	_IO ui CPAR5;
	_IO ui CMAR5;	
} DMA_TYPE;

typedef struct{					//已经检查过，寄存器定义没问题
	_IO ui SYSCFG_CFGR1;
		  ui RESERVED;
	_IO ui SYSCFG_EXTICR1;
	_IO ui SYSCFG_EXTICR2;
	_IO ui SYSCFG_EXTICR3;
	_IO ui SYSCFG_EXTICR4;
	_IO ui SYSCFG_CFGR2;
} SYSCFG_TYPE;

typedef struct{
	_IO ui EXTI_IMR;
	_IO ui EXTI_EMR;
	_IO ui EXTI_RTSR;
	_IO ui EXTI_FTSR;
	_IO ui EXTI_SWIER;
	_IO ui EXTI_PR;
} EXTI_TYPE;

typedef struct{		      //已经检查过，寄存器定义没问题
	_IO ui ADC_ISR;
	_IO ui ADC_IER;
	_IO ui ADC_CR;
	_IO ui ADC_CFGR1;
	_IO ui ADC_CFGR2;
	_IO ui ADC_SMPR;
			ui Reserved_1;
			ui Reserved_2;
	_IO ui ADC_TR;
			ui Reserved_3;
	_IO ui ADC_CHSELR;
			ui Reserved_4;
			ui Reserved_5;
			ui Reserved_6;
			ui Reserved_7;
			ui Reserved_8;
	_IO ui ADC_DR;
			ui Reserved_9[177];
	_IO ui ADC_CCR;
} ADC_TYPE;

typedef struct{				  //已经检查过，寄存器定义没问题
	_IO ui TIMx_CR1;
	_IO ui TIMx_CR2;
	_IO ui TIMx_SMCR;
	_IO ui TIMx_DIER;
	_IO ui TIMx_SR;
	_IO ui TIMx_EGR;
	_IO ui TIMx_CCMR1;
	_IO ui TIMx_CCMR2;
	_IO ui TIMx_CCER;
	_IO ui TIMx_CNT;
	_IO ui TIMx_PSC;
	_IO ui TIMx_ARR;
	_IO ui TIMx_RCR;
	_IO ui TIMx_CCR1;
	_IO ui TIMx_CCR2;
	_IO ui TIMx_CCR3;
	_IO ui TIMx_CCR4;
	_IO ui TIMx_BDTR;
	_IO ui TIMx_DCR;
	_IO ui TIMx_DMAR;
} TIMx;

typedef struct{				  //已经检查过，寄存器定义没问题
	_IO ui RTC_TR;
	_IO ui RTC_DR;
	_IO ui RTC_CR;
	_IO ui RTC_ISR;
	_IO ui RTC_PRER;
			ui Reserve_1;
			ui Reserve_2;
	_IO ui RTC_ALRMAR;
		  ui Reserve_3;
	_IO ui RTC_WPR;
	_IO ui RTC_SSR;
	_IO ui RTC_SHIFTR;
	_IO ui RTC_TSTR;
	_IO ui RTC_TSDR;
	_IO ui RTC_TSSSR;
	_IO ui RTC_CALR;
	_IO ui RTC_TAFCR;
	_IO ui RTC_ALRMASSR;
} RTC_TYPE;

typedef struct{				  //已经检查过，寄存器定义没问题
	_IO ui I2Cx_CR1;
	_IO ui I2Cx_CR2;
	_IO ui I2Cx_OAR1;
	_IO ui I2Cx_OAR2;
	_IO ui I2Cx_TIMINGR;
	_IO ui I2Cx_TIMEOUT;
	_IO ui I2Cx_ISR;
	_IO ui I2Cx_ICR;
	_IO ui I2Cx_PECR;
	_IO ui I2Cx_RXDR;
	_IO ui I2Cx_TXDR;
} I2C_TYPE;

typedef struct{				  //已经检查过，寄存器定义没问题
 	_IO ui USARTx_CR1;
 	_IO ui USARTx_CR2;
 	_IO ui USARTx_CR3;
 	_IO ui USARTx_BRR;
 	_IO ui USARTx_GTPR;
 	_IO ui USARTx_RTOR;
 	_IO ui USARTx_RQR;
 	_IO ui USARTx_ISR;
 	_IO ui USARTx_ICR;
 	_IO ui USARTx_RDR;
 	_IO ui USARTx_TDR;
} USART_TYPE;

typedef struct{				  //已经检查过，寄存器定义没问题
	_IO ui SPIx_CR1;
 	_IO ui SPIx_CR2;
 	_IO ui SPIx_SR;
 	_IO ui SPIx_DR;
 	_IO ui SPIx_CRCPR;
 	_IO ui SPIx_RXCRCR;
 	_IO ui SPIx_TXCRCR;
} SPI_TYPE;


#define CRC 	 ((CRC_TYPE*)0x40023000)
#define PWR		 ((PWR_TYPE *) 0x40007000)
#define RCC 	 ((RCC_TYPE*)0x40021000)
#define GPIOC  ((GPIOx_TYPE*)0x48000800)
#define GPIOB	 ((GPIOx_TYPE*)0x48000400)
#define GPIOA  ((GPIOx_TYPE*)0x48000000)
#define DMA 	 ((DMA_TYPE*)0x40020000)
#define SYSCFG ((SYSCFG_TYPE*)0x40010000)
#define EXTI 	 ((EXTI_TYPE*)0x40010400)
#define ADC 	 ((ADC_TYPE *)0x40012400)
#define TIM1 	 ((TIMx *) 0x40012C00)
#define TIM3 	 ((TIMx *) 0x40000400)
#define RTC 	 ((RTC_TYPE *) RTC_BASE)
#define I2C1	 ((I2C_TYPE *) 0x40005400)
#define USART1 ((USART_TYPE *) 0x40013800)
#define SPI1	 ((SPI_TYPE *) 0x40013000)

/*****************CORE外设***********************/
typedef struct{
	_IO ui NVIC_ISER;
			ui NVIC_reserve1[31];
	_IO ui NVIC_ICER;
			ui NVIC_reserve2[31];
	_IO ui NVIC_ISPR;
			ui NVIC_reserve3[31];
	_IO ui NVIC_ICPR;
			ui NVIC_reserve4[95];
	_IO ui NVIC_IPR[8];//0~7,一共八个
} NVIC_TYPEDEF;

#define NVIC ((NVIC_TYPEDEF *)0xE000E100)
/*****************CORE外设**********************/
///////////

///////////



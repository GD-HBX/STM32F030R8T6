#include "my_lib/register.h"
#include "RTC.h"
#include "ASCII.h"

RTC_TIME_STRUCT time_stamp_DT,default_DT = {"2014",Tuesday,"05","13",NOT_USE,"22","37","30"};

void RTC_CLK_ENB(){
	RCC->RCC_APB1ENR |= 0x00000001 << 28;//打开pwr的时钟
	PWR->PWR_CR 		 |= 0x00000001 <<  8;//DBP置1
	RCC->RCC_CSR		 |= 1;
	RCC->RCC_BDCR		 |= 0x00000002 <<	 8;//选择时钟为02模式
	RCC->RCC_BDCR 	 |= 0x00000001 << 15;//*必须开启时钟才能修改RTC的寄存器值
}

void RTC_UNLOCK(){
	PWR->PWR_CR   |= 0x00000001 <<  8;//DBP置1
	RTC->RTC_WPR	|= 0xCA;
	RTC->RTC_WPR  |= 0x53;
}

void RTC_LOCK(){
	PWR->PWR_CR  &= ~(0x00000001 <<  8);//DBP置0
	RTC->RTC_WPR  = 0;
}

void __TIME_HANDLE(RTC_TIME_STRUCT *p){//把日期时间数据结构转换成具体数据并写入寄存器中
	ui date=0,time=0,temp=0;
	temp = RTC->RTC_CR;
	RTC->RTC_CR = p->AP_flag ? 0x400000|temp : ~0x00400000&temp;//设置24小时或者12小时AM/PM模式
	date |= ASCII2CHAR(&p->year[2])  	 << 20;//年的十位
	date |= ASCII2CHAR(&p->year[3])  	 << 16;//年的个位
	date |= p->day 		 						 	 	 << 13;//星期
	date |= ASCII2CHAR(&p->month[0]) 	 << 12;//月十位
	date |= ASCII2CHAR(&p->month[1]) 	 <<  8;//月个位
	date |= ASCII2CHAR(&p->date[0])    <<  4;//日十位
	date |= ASCII2CHAR(&p->date[1])  	 <<  0;//日个位
	time |= p->AP_flag 							 	 << 22;//AM/PM标志位
	time |= ASCII2CHAR(&p->hour[0])	 	 << 20;//时十位
	time |= ASCII2CHAR(&p->hour[1])	 	 << 16;//时个位
	time |= ASCII2CHAR(&p->minute[0])	 << 12;//分十位
	time |= ASCII2CHAR(&p->minute[1])	 <<  8;//分个位
	time |= ASCII2CHAR(&p->second[0])	 <<  4;//秒十位
	time |= ASCII2CHAR(&p->second[1])	 <<  0;//秒个位
	RTC->RTC_DR = date;
	RTC->RTC_TR = time;
}

uc DATE_TIME_CHECK(RTC_TIME_STRUCT *p){//时间参数检测，没问题就返回0
	
	return 0;
}

void RTC_INIT(RTC_TIME_STRUCT *TIME_and_DATE ){//TIME_and_DATE[]列如：2014-04-25-20-32-00
	if(TIME_and_DATE==0){
		if((RTC->RTC_ISR & RTC_ISR_INITS) != RTC_ISR_INITS){//如果RTC没有被初始化过的话执行默认初始化
			TIME_and_DATE = &default_DT;
		}else{
			return;
		}
	}
	if(DATE_TIME_CHECK(TIME_and_DATE)){return;}
	RTC_UNLOCK();
	RTC->RTC_ISR |= RTC_ISR_INIT;
	while((RTC->RTC_ISR & RTC_ISR_INITF) != RTC_ISR_INITF);
	RTC->RTC_PRER = 0x0000007F << 16 | 0x138;//选择LSI时钟源，生成1HZ；此部分代码有待加强，例如自动检测时钟源，根据不同时钟源设置不同的值
	__TIME_HANDLE(TIME_and_DATE);
	RTC->RTC_ISR &= ~RTC_ISR_INIT;
	RTC_LOCK();
} 

void __read(RTC_TIME_STRUCT *p,ui *dr,ui *tr){
	ui t_dr = *dr,t_tr = *tr;
	p->second[0] = CHAR2ASCII((uc*)0,(t_tr & 	(0x00000007<<4))  >> 	4); 
	p->second[1] = CHAR2ASCII((uc*)0, t_tr & 							 0x0000000F);
	p->minute[0] = CHAR2ASCII((uc*)0,(t_tr & 	(0x00000007<<12)) >> 12);
	p->minute[1] = CHAR2ASCII((uc*)0,(t_tr & 	(0x0000000F<<8))  >> 	8);
	p->hour[0]   = CHAR2ASCII((uc*)0,(t_tr & 	(0x00000003<<20)) >> 20);
	p->hour[1]	 = CHAR2ASCII((uc*)0,(t_tr & 	(0x0000000F<<16)) >> 16);
	p->AP_flag	 = t_tr & (0x00000001 << 22) ?  1 : 0;
	p->date[0]	 = CHAR2ASCII((uc*)0,(t_dr & (0x00000003<<4))  >> 	4); 
	p->date[1]	 = CHAR2ASCII((uc*)0, t_dr & 							 0x0000000F);
	p->month[0]	 = CHAR2ASCII((uc*)0,(t_dr & 	(0x00000001<<12)) >> 12);
	p->month[1]	 = CHAR2ASCII((uc*)0,(t_dr & 	(0x0000000F<<8)) 	>> 	8);
	p->year[2]	 = CHAR2ASCII((uc*)0,(t_dr & 	(0x0000000F<<20)) >> 20);
	p->year[3]	 = CHAR2ASCII((uc*)0,(t_dr & 	(0x0000000F<<16)) >> 16);
	p->day			 = CHAR2ASCII((uc*)0,(t_dr & 	(0x00000007<<13)) >> 13);
}

void RTC_TIME_and_DATE_READ(RTC_TIME_STRUCT *p){		//时间读取
	if((RTC->RTC_CR & (0x00000001 << 5)) == 0){
		while((RTC->RTC_ISR & (0x00000001 << 5)) == 0);	//检测同步位
		__read(p,(ui*)RTC_BASE+0x04,(ui*)RTC_BASE);
		RTC->RTC_ISR &= ~(0x00000001 << 5);							//清除状态
	}else{
		__read(p,(ui*)RTC_BASE+0x04,(ui*)RTC_BASE);
	}
	
}

void time_stamp_time_read(){//时间戳读取
	__read(&time_stamp_DT,(ui*)RTC_BASE+0x34,(ui*)RTC_BASE+0x30);
}

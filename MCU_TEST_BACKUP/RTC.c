#include "my_lib/register.h"
#include "RTC.h"
#include "ASCII.h"

RTC_TIME_STRUCT time_stamp_DT,default_DT = {"2014",Tuesday,"05","13",NOT_USE,"22","37","30"};

void RTC_CLK_ENB(){
	RCC->RCC_APB1ENR |= 0x00000001 << 28;//��pwr��ʱ��
	PWR->PWR_CR 		 |= 0x00000001 <<  8;//DBP��1
	RCC->RCC_CSR		 |= 1;
	RCC->RCC_BDCR		 |= 0x00000002 <<	 8;//ѡ��ʱ��Ϊ02ģʽ
	RCC->RCC_BDCR 	 |= 0x00000001 << 15;//*���뿪��ʱ�Ӳ����޸�RTC�ļĴ���ֵ
}

void RTC_UNLOCK(){
	PWR->PWR_CR   |= 0x00000001 <<  8;//DBP��1
	RTC->RTC_WPR	|= 0xCA;
	RTC->RTC_WPR  |= 0x53;
}

void RTC_LOCK(){
	PWR->PWR_CR  &= ~(0x00000001 <<  8);//DBP��0
	RTC->RTC_WPR  = 0;
}

void __TIME_HANDLE(RTC_TIME_STRUCT *p){//������ʱ�����ݽṹת���ɾ������ݲ�д��Ĵ�����
	ui date=0,time=0,temp=0;
	temp = RTC->RTC_CR;
	RTC->RTC_CR = p->AP_flag ? 0x400000|temp : ~0x00400000&temp;//����24Сʱ����12СʱAM/PMģʽ
	date |= ASCII2CHAR(&p->year[2])  	 << 20;//���ʮλ
	date |= ASCII2CHAR(&p->year[3])  	 << 16;//��ĸ�λ
	date |= p->day 		 						 	 	 << 13;//����
	date |= ASCII2CHAR(&p->month[0]) 	 << 12;//��ʮλ
	date |= ASCII2CHAR(&p->month[1]) 	 <<  8;//�¸�λ
	date |= ASCII2CHAR(&p->date[0])    <<  4;//��ʮλ
	date |= ASCII2CHAR(&p->date[1])  	 <<  0;//�ո�λ
	time |= p->AP_flag 							 	 << 22;//AM/PM��־λ
	time |= ASCII2CHAR(&p->hour[0])	 	 << 20;//ʱʮλ
	time |= ASCII2CHAR(&p->hour[1])	 	 << 16;//ʱ��λ
	time |= ASCII2CHAR(&p->minute[0])	 << 12;//��ʮλ
	time |= ASCII2CHAR(&p->minute[1])	 <<  8;//�ָ�λ
	time |= ASCII2CHAR(&p->second[0])	 <<  4;//��ʮλ
	time |= ASCII2CHAR(&p->second[1])	 <<  0;//���λ
	RTC->RTC_DR = date;
	RTC->RTC_TR = time;
}

uc DATE_TIME_CHECK(RTC_TIME_STRUCT *p){//ʱ�������⣬û����ͷ���0
	
	return 0;
}

void RTC_INIT(RTC_TIME_STRUCT *TIME_and_DATE ){//TIME_and_DATE[]���磺2014-04-25-20-32-00
	if(TIME_and_DATE==0){
		if((RTC->RTC_ISR & RTC_ISR_INITS) != RTC_ISR_INITS){//���RTCû�б���ʼ�����Ļ�ִ��Ĭ�ϳ�ʼ��
			TIME_and_DATE = &default_DT;
		}else{
			return;
		}
	}
	if(DATE_TIME_CHECK(TIME_and_DATE)){return;}
	RTC_UNLOCK();
	RTC->RTC_ISR |= RTC_ISR_INIT;
	while((RTC->RTC_ISR & RTC_ISR_INITF) != RTC_ISR_INITF);
	RTC->RTC_PRER = 0x0000007F << 16 | 0x138;//ѡ��LSIʱ��Դ������1HZ���˲��ִ����д���ǿ�������Զ����ʱ��Դ�����ݲ�ͬʱ��Դ���ò�ͬ��ֵ
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

void RTC_TIME_and_DATE_READ(RTC_TIME_STRUCT *p){		//ʱ���ȡ
	if((RTC->RTC_CR & (0x00000001 << 5)) == 0){
		while((RTC->RTC_ISR & (0x00000001 << 5)) == 0);	//���ͬ��λ
		__read(p,(ui*)RTC_BASE+0x04,(ui*)RTC_BASE);
		RTC->RTC_ISR &= ~(0x00000001 << 5);							//���״̬
	}else{
		__read(p,(ui*)RTC_BASE+0x04,(ui*)RTC_BASE);
	}
	
}

void time_stamp_time_read(){//ʱ�����ȡ
	__read(&time_stamp_DT,(ui*)RTC_BASE+0x34,(ui*)RTC_BASE+0x30);
}

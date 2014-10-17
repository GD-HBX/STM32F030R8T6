#define RTC_CR_FMT    (0x00000001 << 6)
#define RTC_ISR_INIT  (0x00000001 << 7)
#define RTC_ISR_INITF (0x00000001 << 6)
#define RTC_ISR_INITS (0x00000001 << 4)

enum Week_Day{NOT_USE=0,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday,Sunday};

typedef struct DATE_TIME{
	uc year[4];
	uc day;
	uc month[2];
	uc date[2];
	uc AP_flag;
	uc hour[2];
	uc minute[2];
	uc second[2];
} RTC_TIME_STRUCT;

void RTC_CLK_ENB(void);	
void RTC_INIT(RTC_TIME_STRUCT *TIME_and_DATE);//TIME_and_DATE的值可以是指向RTC_TIME_STRUCT结构类型的对象或者是0；
void RTC_TIME_and_DATE_READ(RTC_TIME_STRUCT *p);
void time_stamp_time_read(void);//执行完这个函数后，结果储存在全局变量RTC_TIME_STRUCT time_stamp_DT中
